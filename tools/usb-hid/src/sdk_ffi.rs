// SPDX-License-Identifier: GPL-2.0-only
//! 供 Python 调用的原生客户端和共享队列接口。
use crate::{client::Client, wire::{Message, MAX_MESSAGE}};
use std::{ffi::{CStr, c_char}, io, panic::{catch_unwind, AssertUnwindSafe}, ptr};

struct Handle { client: Client }
unsafe fn error_text(out:*mut c_char,capacity:usize,text:&str){
    if out.is_null()||capacity==0{return;}
    let n=text.len().min(capacity-1);
    unsafe{ptr::copy_nonoverlapping(text.as_ptr(),out.cast(),n);*out.add(n)=0;}
}
/// 单调用线程拥有句柄；心跳由Client维持。返回null并填入错误文本表示失败。
#[unsafe(no_mangle)]
pub unsafe extern "C" fn vhid_open(path:*const c_char,error:*mut c_char,capacity:usize)->*mut std::ffi::c_void{
    let result=catch_unwind(AssertUnwindSafe(||->io::Result<_>{
        if path.is_null(){return Err(io::Error::other("null path"));}
        let path=unsafe{CStr::from_ptr(path)}.to_str().map_err(io::Error::other)?;
        Client::connect(path,true).map(|client|Box::new(Handle{client}))
    }));
    match result{Ok(Ok(handle))=>Box::into_raw(handle).cast(),Ok(Err(e))=>{unsafe{error_text(error,capacity,&e.to_string());}ptr::null_mut()},
        Err(_)=>{unsafe{error_text(error,capacity,"native client panic");}ptr::null_mut()}}
}
/// 请求和响应都是原生CBOR v2消息；动作只通过现有共享队列提交。
#[unsafe(no_mangle)]
pub unsafe extern "C" fn vhid_command(handle:*mut std::ffi::c_void,input:*const u8,length:usize,
    output:*mut u8,capacity:usize,error:*mut c_char,error_capacity:usize)->isize{
    let result=catch_unwind(AssertUnwindSafe(||->io::Result<usize>{
        if handle.is_null()||input.is_null()||output.is_null()||length>MAX_MESSAGE{return Err(io::Error::other("invalid FFI arguments"));}
        let h=unsafe{&mut *handle.cast::<Handle>()};let input=unsafe{std::slice::from_raw_parts(input,length)};
        let message=Message::decode(input)?;let response=h.client.command(message)?.encode()?;
        if response.len()>capacity{return Err(io::Error::other("response capacity"));}
        unsafe{ptr::copy_nonoverlapping(response.as_ptr(),output,response.len());}Ok(response.len())
    }));
    match result{Ok(Ok(n))=>n as isize,Ok(Err(e))=>{unsafe{error_text(error,error_capacity,&e.to_string());}-1},
        Err(_)=>{unsafe{error_text(error,error_capacity,"native client panic");}-1}}
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn vhid_close(handle:*mut std::ffi::c_void){
    if !handle.is_null(){let _=catch_unwind(AssertUnwindSafe(||unsafe{drop(Box::from_raw(handle.cast::<Handle>()));}));}
}
