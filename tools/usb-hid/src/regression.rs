// SPDX-License-Identifier: GPL-2.0-only
//! 验证 CBOR 编码、共享队列顺序和位移计算。
use crate::{
    shared::{CAPACITY, Shared},
    trajectory::*,
    wire::{Message, Op},
};
use std::{io, sync::Arc};
#[test]
fn transport_and_pointer_contracts() -> io::Result<()> {
    let mut golden = Message::new(Op::Move);
    golden.seq = 17;
    golden.args = [8, -2, 0, 0];
    golden.target = 1;
    let bytes = golden.encode()?;
    assert_eq!(bytes.as_ref(), &[0xa5, 0, 2, 1, 12, 2, 17, 6, 0x84, 8, 0x21, 0, 0, 7, 1]);
    for end in 0..bytes.len() {
        assert!(Message::decode(&bytes[..end]).is_err());
    }
    assert!(Message::decode(&[0xa3, 0, 2, 1, 12, 1, 12]).is_err());
    let mut header = Message::new(Op::Batch);
    header.seq = 99;
    let batch = crate::wire::encode_batch(&header, &[golden, golden])?;
    let (h, steps) = crate::wire::decode_packet(&batch)?;
    assert_eq!(h, header);
    assert_eq!(steps, vec![golden, golden]);
    assert!(crate::wire::encode_batch(&header, &[]).is_err());
    let mut m = Message::new(Op::Move);
    m.args = [-13, 31, 0, 0];
    m.seq = u64::MAX;
    m.data[63] = 0xfe;
    assert_eq!(Message::decode(&m.encode()?)?, m);
    assert!(Message::decode(&[0; 128]).is_err());
    let shared = Arc::new(Shared::create(91)?);
    for i in 0..CAPACITY {
        m.seq = i as u64;
        shared.push(0, &m)?;
    }
    assert!(shared.push(0, &m).is_err());
    for i in 0..CAPACITY {
        assert_eq!(shared.pop(0)?.map(|m| m.seq), Some(i as u64));
    }
    assert!(shared.pop(0)?.is_none());
    std::thread::scope(|s| -> io::Result<()> {
        let q = shared.clone();
        let producer = s.spawn(move || -> io::Result<()> {
            for i in 0..10000 {
                let mut m = Message::new(Op::Move);
                m.seq = i;
                loop {
                    match q.push(0, &m) {
                        Ok(()) => break,
                        Err(e) if e.kind() == io::ErrorKind::WouldBlock => std::thread::yield_now(),
                        Err(e) => return Err(e),
                    }
                }
            }
            Ok(())
        });
        for i in 0..10000 {
            loop {
                if let Some(m) = shared.pop(0)? {
                    assert_eq!(m.seq, i);
                    break;
                }
                std::thread::yield_now();
            }
        }
        producer.join().map_err(|_| io::Error::other("producer panicked"))??;
        Ok(())
    })?;
    let mut fraction = Remainder::default();
    let mut total = 0;
    for _ in 0..100 {
        total += fraction
            .quantize(Vector { x: 0.5, y: -0.5 })
            .ok_or_else(|| io::Error::other("quantize"))?
            .0;
    }
    assert_eq!(total, 50);
    let rotated = Transform::rotate(std::f64::consts::FRAC_PI_2)
        .ok_or_else(|| io::Error::other("rotate"))?
        .apply(Vector { x: 2., y: 0. });
    assert!(rotated.x.abs() < 1e-9 && (rotated.y - 2.).abs() < 1e-9);
    Ok(())
}
