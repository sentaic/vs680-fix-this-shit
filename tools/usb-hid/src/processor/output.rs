// SPDX-License-Identifier: GPL-2.0-only
//! Endpoint backpressure preserves physical reports and bounds program submissions.
use super::*;

impl Processor {
    pub(super) fn emit(
        &self,
        t: usize,
        r: Report,
        at: Instant,
        ticket: Option<Message>,
        control: bool,
    ) -> io::Result<()> {
        self.submit(t, r, at, ticket, control, None)
    }
    pub(super) fn emit_merged(
        &self,
        t: usize,
        r: Report,
        raw: Report,
        at: Instant,
    ) -> io::Result<()> {
        self.submit(t, r, at, None, false, Some(raw))
    }
    pub(super) fn submit(
        &self,
        t: usize,
        r: Report,
        at: Instant,
        ticket: Option<Message>,
        control: bool,
        fallback: Option<Report>,
    ) -> io::Result<()> {
        let d = &self.devices[t];
        self.flush_output(t)?;
        let id = d.lane.physical.layout.input_id(r.bytes())?;
        let generation = d.lane.generation.load(Ordering::Acquire);
        let stationary = if Some(id) == d.lane.codec.mouse {
            let v = d.lane.codec.decode(id, r.bytes())?;
            v.x == 0 && v.y == 0 && v.wheel == 0 && v.pan == 0
        } else {
            true
        };
        // 合并相邻的相同静止控制报告，物理输入按顺序提交。
        if control && fallback.is_none() && ticket.is_none() && stationary {
            let pending = d.output_pending.borrow();
            if pending.back().is_some_and(|old| {
                old.fallback.is_none()
                    && old.ticket.is_none()
                    && old.generation == generation
                    && old.id == id
                    && old.report.bytes() == r.bytes()
            }) {
                return Ok(());
            }
        }
        if fallback.is_none() && d.output_pending.borrow().len() >= 256 {
            d.lane.stats.queue_full.fetch_add(1, Ordering::Relaxed);
            d.output_restore.set(true);
            if let Some(m) = ticket {
                self.reply(&m, Status::Full);
            }
            return Ok(());
        }
        d.lane.physical.cache_planned(id, r.bytes());
        let packet = Packet {
            processing_started: self
                .input_timing
                .filter(|timing| timing.received == at)
                .map(|timing| timing.started),
            fallback,
            mouse: Some(id) == d.lane.codec.mouse,
            report: r,
            id,
            at,
            generation,
            ticket: ticket.and_then(|m| self.session.as_ref().map(|s| (s.clone(), m))),
            enqueued: Instant::now(),
        };
        d.output_pending.borrow_mut().push_back(packet);
        self.flush_output(t)
    }
    fn flush_output(&self, t: usize) -> io::Result<()> {
        let d = &self.devices[t];
        while d.lane.queued.load(Ordering::Acquire) < 192 {
            let Some(packet) = d.output_pending.borrow_mut().pop_front() else {
                break;
            };
            d.lane.queued.fetch_add(1, Ordering::AcqRel);
            match d.lane.sender.try_send(packet) {
                Ok(()) => d.lane.wake.notify(),
                Err(mpsc::TrySendError::Full(packet)) => {
                    d.lane.queued.fetch_sub(1, Ordering::AcqRel);
                    d.lane.stats.queue_full.fetch_add(1, Ordering::Relaxed);
                    d.output_pending.borrow_mut().push_front(packet);
                    break;
                }
                Err(mpsc::TrySendError::Disconnected(_)) => {
                    d.lane.queued.fetch_sub(1, Ordering::AcqRel);
                    return Err(invalid("USB output channel closed"));
                }
            }
        }
        Ok(())
    }
    pub(super) fn drain_output(&mut self, t: usize) -> io::Result<()> {
        self.flush_output(t)?;
        if self.devices[t].output_pending.borrow().is_empty()
            && self.devices[t].lane.queued.load(Ordering::Acquire) < 192
            && self.devices[t].output_restore.replace(false)
        {
            self.restore(t, true, None)?;
        }
        Ok(())
    }
}
