// SPDX-License-Identifier: GPL-2.0-only
//! CBOR v2 消息编解码，使用定长缓冲区保存字段。
use minicbor::{
    Decoder, Encoder,
    encode::{Write, write::Cursor},
};
use std::io;
pub const VERSION: u8 = 2;
pub const MAX_MESSAGE: usize = 192;
pub const MAX_BATCH: usize = 64;
pub const MAX_PACKET: usize = MAX_MESSAGE * (MAX_BATCH + 1) + 8;
pub const REPLY: u32 = 1;
pub const SHARED: u32 = 2;
pub const SOCKET: &str = "/run/vs680-hid/control.sock";
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u16)]
pub enum Op {
    Hello = 1,
    Ping = 2,
    Subscribe = 3,
    Physical = 4,
    Output = 5,
    Capabilities = 6,
    Takeover = 7,
    Release = 8,
    Raw = 9,
    Key = 10,
    Button = 11,
    Move = 12,
    Scroll = 13,
    Scale = 14,
    Rotate = 15,
    Velocity = 16,
    Offset = 17,
    ForceKey = 18,
    ForceButton = 19,
    Cancel = 20,
    CancelAll = 21,
    Trigger = 22,
    RemoveTrigger = 23,
    Action = 24,
    Decision = 25,
    Keys = 26,
    Buttons = 27,
    Batch = 28,
    Goodbye = 29,
    Variable = 30,
    PhysicalState = 32,
    CompletedState = 33,
    Input = 0x8001,
    Gap = 0x8002,
    Response = 0x8003,
    MotionInput = 0x8004,
}
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(i32)]
pub enum Status {
    Applied = 0,
    Queued = 1,
    Completed = 2,
    Invalid = 3,
    Unsupported = 4,
    Busy = 5,
    Expired = 6,
    Stale = 7,
    Full = 8,
    Unknown = 9,
    Cancelled = 10,
    Failed = 11,
}
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Message {
    pub op: Op,
    pub seq: u64,
    pub at: u64,
    pub expires: u64,
    pub id: u64,
    pub args: [i32; 4],
    pub target: u32,
    pub flags: u32,
    pub data: [u8; 64],
}
#[derive(Debug)]
pub struct Encoded {
    bytes: [u8; MAX_MESSAGE],
    len: usize,
}
impl AsRef<[u8]> for Encoded {
    fn as_ref(&self) -> &[u8] {
        &self.bytes[..self.len]
    }
}
impl std::ops::Deref for Encoded {
    type Target = [u8];
    fn deref(&self) -> &[u8] {
        self.as_ref()
    }
}
impl Message {
    pub fn new(op: Op) -> Self {
        Self {
            op,
            seq: 0,
            at: 0,
            expires: 0,
            id: 0,
            args: [0; 4],
            target: 0,
            flags: REPLY,
            data: [0; 64],
        }
    }
    pub fn response(&self, status: Status) -> Self {
        let mut r = Self::new(Op::Response);
        r.seq = self.seq;
        r.id = self.id;
        r.target = self.target;
        r.args[0] = status as i32;
        r
    }
    pub fn encode(&self) -> io::Result<Encoded> {
        if self.op == Op::Batch {
            return Err(invalid("batch requires steps"));
        }
        let mut bytes = [0; MAX_MESSAGE];
        let mut e = Encoder::new(Cursor::new(bytes.as_mut_slice()));
        encode_map(&mut e, self, None)?;
        let len = e.into_writer().position();
        Ok(Encoded { bytes, len })
    }
    pub fn decode(bytes: &[u8]) -> io::Result<Self> {
        if bytes.is_empty() || bytes.len() > MAX_MESSAGE {
            return Err(invalid("CBOR message length"));
        }
        let mut d = Decoder::new(bytes);
        let (m, steps) = decode_map(&mut d, false)?;
        if d.position() != bytes.len() || !steps.is_empty() {
            return Err(invalid("trailing CBOR data"));
        }
        Ok(m)
    }
}
fn payload_len(m: &Message) -> io::Result<usize> {
    let n = match m.op {
        Op::Raw | Op::Input => usize::try_from(m.args[0]).map_err(|_| invalid("report length"))?,
        Op::Decision if m.args[0] == 1 => {
            usize::try_from(m.args[1]).map_err(|_| invalid("replacement length"))?
        }
        Op::Keys => 32,
        Op::Trigger => {
            if m.data[16..].iter().any(|b| *b != 0) {
                return Err(invalid("trigger padding"));
            }
            return Ok(0);
        }
        Op::Response if (1..=64).contains(&m.args[1]) => 64,
        _ => {
            if m.data.iter().any(|b| *b != 0) {
                64
            } else {
                0
            }
        }
    };
    if n > 64 || m.data[n..].iter().any(|b| *b != 0) {
        return Err(invalid("payload length/padding"));
    }
    Ok(n)
}
fn encode_map<W: Write>(
    e: &mut Encoder<W>,
    m: &Message,
    steps: Option<&[Message]>,
) -> io::Result<()> {
    let len = payload_len(m)?;
    let count = 2
        + usize::from(m.seq != 0)
        + usize::from(m.at != 0)
        + usize::from(m.expires != 0)
        + usize::from(m.id != 0)
        + usize::from(m.args != [0; 4])
        + usize::from(m.target != 0)
        + usize::from(m.flags != REPLY)
        + usize::from(len > 0)
        + usize::from(m.op == Op::Trigger)
        + usize::from(steps.is_some());
    let result = (|| -> Result<(), minicbor::encode::Error<W::Error>> {
        e.map(count as u64)?.u8(0)?.u8(VERSION)?.u8(1)?.u16(m.op as u16)?;
        for (key, value) in [(2, m.seq), (3, m.at), (4, m.expires), (5, m.id)] {
            if value != 0 {
                e.u8(key)?.u64(value)?;
            }
        }
        if m.args != [0; 4] {
            e.u8(6)?.array(4)?;
            for a in m.args {
                e.i32(a)?;
            }
        }
        if m.target != 0 {
            e.u8(7)?.u32(m.target)?;
        }
        if m.flags != REPLY {
            e.u8(8)?.u32(m.flags)?;
        }
        if len > 0 {
            e.u8(9)?.bytes(&m.data[..len])?;
        }
        if m.op == Op::Trigger {
            e.u8(10)?.array(4)?;
            for b in m.data[..16].chunks_exact(4) {
                let mut a = [0; 4];
                a.copy_from_slice(b);
                e.i32(i32::from_le_bytes(a))?;
            }
        }
        Ok(())
    })();
    result.map_err(|_| invalid("CBOR output buffer"))?;
    if let Some(steps) = steps {
        e.u8(11)
            .and_then(|e| e.array(steps.len() as u64))
            .map_err(|_| invalid("CBOR batch buffer"))?;
        for s in steps {
            encode_map(e, s, None)?;
        }
    }
    Ok(())
}
pub fn encode_batch(header: &Message, steps: &[Message]) -> io::Result<Vec<u8>> {
    if header.op != Op::Batch
        || steps.is_empty()
        || steps.len() > MAX_BATCH
        || steps.iter().any(|s| s.op == Op::Batch)
    {
        return Err(invalid("batch shape"));
    }
    let mut b = vec![0; MAX_PACKET];
    let mut e = Encoder::new(Cursor::new(b.as_mut_slice()));
    encode_map(&mut e, header, Some(steps))?;
    let len = e.into_writer().position();
    b.truncate(len);
    Ok(b)
}
pub fn decode_packet(bytes: &[u8]) -> io::Result<(Message, Vec<Message>)> {
    if bytes.is_empty() || bytes.len() > MAX_PACKET {
        return Err(invalid("CBOR packet length"));
    }
    let mut d = Decoder::new(bytes);
    let (m, steps) = decode_map(&mut d, true)?;
    if d.position() != bytes.len() {
        return Err(invalid("trailing CBOR packet data"));
    }
    if m.op != Op::Batch && bytes.len() > MAX_MESSAGE {
        return Err(invalid("message exceeds limit"));
    }
    Ok((m, steps))
}
fn argument_len(n: u64) -> usize {
    match n {
        0..=23 => 1,
        24..=255 => 2,
        256..=65535 => 3,
        65536..=0xffff_ffff => 5,
        _ => 9,
    }
}
fn unsigned(d: &mut Decoder<'_>) -> io::Result<u64> {
    let start = d.position();
    let v = d.u64().map_err(|_| invalid("CBOR unsigned integer"))?;
    if d.position() - start != argument_len(v) {
        return Err(invalid("nonminimal CBOR integer"));
    }
    Ok(v)
}
fn signed(d: &mut Decoder<'_>) -> io::Result<i32> {
    let start = d.position();
    let v = d.i32().map_err(|_| invalid("CBOR i32"))?;
    let n = if v >= 0 { v as u64 } else { (-1 - i64::from(v)) as u64 };
    if d.position() - start != argument_len(n) {
        return Err(invalid("nonminimal CBOR integer"));
    }
    Ok(v)
}
fn array(d: &mut Decoder<'_>, max: u64) -> io::Result<u64> {
    let start = d.position();
    let n = d
        .array()
        .map_err(|_| invalid("CBOR array"))?
        .ok_or_else(|| invalid("indefinite array forbidden"))?;
    if n > max || d.position() - start != argument_len(n) {
        return Err(invalid("CBOR array size/encoding"));
    }
    Ok(n)
}
fn decode_map(d: &mut Decoder<'_>, allow_batch: bool) -> io::Result<(Message, Vec<Message>)> {
    let start = d.position();
    let count = d
        .map()
        .map_err(|_| invalid("CBOR map"))?
        .ok_or_else(|| invalid("indefinite map forbidden"))?;
    if !(2..=12).contains(&count) || d.position() - start != 1 {
        return Err(invalid("CBOR map size"));
    }
    let mut m = Message::new(Op::Hello);
    let mut seen = 0u16;
    let mut previous = None;
    let mut data_len = 0;
    let mut steps = Vec::new();
    for _ in 0..count {
        let key = unsigned(d)?;
        if key > 11 || previous.is_some_and(|p| key <= p) {
            return Err(invalid("unknown, duplicate or unordered CBOR key"));
        }
        previous = Some(key);
        seen |= 1 << key;
        match key {
            0 => {
                if unsigned(d)? != u64::from(VERSION) {
                    return Err(invalid("protocol version"));
                }
            }
            1 => {
                m.op = Op::try_from(
                    u16::try_from(unsigned(d)?).map_err(|_| invalid("operation range"))?,
                )?
            }
            2 => m.seq = unsigned(d)?,
            3 => m.at = unsigned(d)?,
            4 => m.expires = unsigned(d)?,
            5 => m.id = unsigned(d)?,
            6 => {
                if array(d, 4)? != 4 {
                    return Err(invalid("argument count"));
                }
                for v in &mut m.args {
                    *v = signed(d)?;
                }
            }
            7 => m.target = u32::try_from(unsigned(d)?).map_err(|_| invalid("target range"))?,
            8 => m.flags = u32::try_from(unsigned(d)?).map_err(|_| invalid("flags range"))?,
            9 => {
                let before = d.position();
                let b = d.bytes().map_err(|_| invalid("CBOR byte string"))?;
                if b.len() > 64 || d.position() - before != argument_len(b.len() as u64) + b.len() {
                    return Err(invalid("payload size/encoding"));
                }
                data_len = b.len();
                m.data[..data_len].copy_from_slice(b);
            }
            10 => {
                if m.op != Op::Trigger || seen & (1 << 9) != 0 || array(d, 4)? != 4 {
                    return Err(invalid("trigger arguments"));
                }
                for b in m.data[..16].chunks_exact_mut(4) {
                    b.copy_from_slice(&signed(d)?.to_le_bytes());
                }
            }
            11 => {
                if !allow_batch || m.op != Op::Batch {
                    return Err(invalid("nested/unexpected batch"));
                }
                let n = array(d, MAX_BATCH as u64)?;
                if n == 0 {
                    return Err(invalid("empty batch"));
                }
                steps.reserve(n as usize);
                for _ in 0..n {
                    steps.push(decode_map(d, false)?.0);
                }
            }
            _ => return Err(invalid("CBOR key")),
        }
    }
    if seen & 3 != 3 {
        return Err(invalid("missing version or operation"));
    }
    if (m.op == Op::Batch) != !steps.is_empty() {
        return Err(invalid("missing/unexpected batch steps"));
    }
    if m.op == Op::Trigger && seen & (1 << 10) == 0 {
        return Err(invalid("missing trigger arguments"));
    }
    if matches!(m.op, Op::Raw | Op::Input)
        || m.op == Op::Keys
        || (m.op == Op::Decision && m.args[0] == 1)
    {
        if payload_len(&m)? != data_len {
            return Err(invalid("declared report length differs from CBOR bytes"));
        }
    }
    if m.op != Op::Batch && d.position() - start > MAX_MESSAGE {
        return Err(invalid("CBOR message exceeds limit"));
    }
    Ok((m, steps))
}
impl TryFrom<u16> for Op {
    type Error = io::Error;
    fn try_from(value: u16) -> io::Result<Self> {
        let op = match value {
            1 => Op::Hello,
            2 => Op::Ping,
            3 => Op::Subscribe,
            4 => Op::Physical,
            5 => Op::Output,
            6 => Op::Capabilities,
            7 => Op::Takeover,
            8 => Op::Release,
            9 => Op::Raw,
            10 => Op::Key,
            11 => Op::Button,
            12 => Op::Move,
            13 => Op::Scroll,
            14 => Op::Scale,
            15 => Op::Rotate,
            16 => Op::Velocity,
            17 => Op::Offset,
            18 => Op::ForceKey,
            19 => Op::ForceButton,
            20 => Op::Cancel,
            21 => Op::CancelAll,
            22 => Op::Trigger,
            23 => Op::RemoveTrigger,
            24 => Op::Action,
            25 => Op::Decision,
            26 => Op::Keys,
            27 => Op::Buttons,
            28 => Op::Batch,
            29 => Op::Goodbye,
            30 => Op::Variable,
            32 => Op::PhysicalState,
            33 => Op::CompletedState,
            0x8001 => Op::Input,
            0x8002 => Op::Gap,
            0x8003 => Op::Response,
            0x8004 => Op::MotionInput,
            _ => return Err(invalid("operation")),
        };
        Ok(op)
    }
}
pub fn invalid(s: &str) -> io::Error {
    io::Error::new(io::ErrorKind::InvalidData, s)
}
pub fn now_ns() -> u64 {
    let mut t = libc::timespec { tv_sec: 0, tv_nsec: 0 };
    // SAFETY：输出指针有效，时钟由Linux提供。
    if unsafe { libc::clock_gettime(libc::CLOCK_MONOTONIC, &raw mut t) } != 0 {
        return 0;
    }
    (t.tv_sec as u64).saturating_mul(1_000_000_000).saturating_add(t.tv_nsec as u64)
}
