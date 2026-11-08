// SPDX-License-Identifier: GPL-2.0-only
//! 使用 CBOR 控制接口的命令行客户端。
use std::{io, time::Duration};
use vs680_hid::{
    client::Client,
    wire::{Message, Op, SOCKET},
};
fn number(s: Option<String>) -> io::Result<i32> {
    s.ok_or_else(|| io::Error::other("missing number"))?.parse().map_err(io::Error::other)
}
fn run() -> io::Result<()> {
    let mut args = std::env::args().skip(1);
    let mut path = SOCKET.to_owned();
    let mut action = args.next().unwrap_or_default();
    if action == "--socket" {
        path = args.next().ok_or_else(|| io::Error::other("missing socket"))?;
        action = args.next().unwrap_or_default();
    }
    if action.is_empty() || action == "--help" {
        println!(
            "hidctl [--socket PATH] caps TARGET | physical TARGET REPORT_ID | output TARGET REPORT_ID | watch SECONDS | move TARGET DX DY | force-key TARGET USAGE DOWN DURATION_MS | scale TARGET FACTOR DURATION_MS\n短命令退出时撤销其动作；force-key/scale会等待指定持续时间。程序接管请使用SDK。"
        );
        return Ok(());
    }
    let mut c = Client::connect(path, true)?;
    if action == "watch" {
        let seconds = number(args.next())?;
        c.subscribe(true)?;
        let start = std::time::Instant::now();
        while start.elapsed() < Duration::from_secs(seconds.max(1) as u64) {
            for m in c.poll(Duration::from_millis(100))? {
                println!("{m:?}");
            }
        }
        return Ok(());
    }
    let target = number(args.next())? as u32;
    let mut m = Message::new(match action.as_str() {
        "caps" => Op::Capabilities,
        "physical" => Op::Physical,
        "output" => Op::Output,
        "move" => Op::Move,
        "force-key" => Op::ForceKey,
        "scale" => Op::Scale,
        _ => return Err(io::Error::other("unknown command")),
    });
    m.target = target;
    match m.op {
        Op::Physical | Op::Output => m.args[0] = number(args.next())?,
        Op::Move => {
            m.args[0] = number(args.next())?;
            m.args[1] = number(args.next())?;
        }
        Op::ForceKey => {
            m.args[0] = number(args.next())?;
            m.args[1] = number(args.next())?;
            m.args[2] = number(args.next())?;
        }
        Op::Scale => {
            let f = args
                .next()
                .ok_or_else(|| io::Error::other("missing factor"))?
                .parse::<f64>()
                .map_err(io::Error::other)?;
            if !f.is_finite() || f.abs() > 64. {
                return Err(io::Error::other("factor outside -64..64"));
            }
            m.args = [(f * 65536.) as i32, (f * 65536.) as i32, number(args.next())?, 0];
        }
        _ => {}
    }
    println!("{:?}", c.command(m)?);
    if matches!(m.op, Op::ForceKey | Op::Scale) {
        std::thread::sleep(Duration::from_millis(m.args[2].max(1) as u64 + 20));
    }
    Ok(())
}
fn main() -> std::process::ExitCode {
    match run() {
        Ok(()) => std::process::ExitCode::SUCCESS,
        Err(e) => {
            eprintln!("{e}");
            std::process::ExitCode::FAILURE
        }
    }
}
