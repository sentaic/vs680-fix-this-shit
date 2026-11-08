/* SPDX-License-Identifier: GPL-2.0 */
/*
 * drivers/staging/android/ion/ion-trace.h
 *
 * Copyright (C) 2020 Google, Inc.
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM dmabuf_heap

#if !defined(_DMABUF_HEAP_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define _DMABUF_HEAP_TRACE_H

#include <linux/tracepoint.h>

TRACE_EVENT(sys_cust_alloc,
	    TP_PROTO(bool uncached, long len, int rsv_sz, int total_alloc, unsigned long dur),
	    TP_ARGS(uncached, len, rsv_sz, total_alloc, dur),

	    TP_STRUCT__entry(
		__field(bool, uncached)
		__field(long, len)
		__field(int, rsv_sz)
		__field(int, total_alloc)
		__field(unsigned long, dur)
	    ),
	    TP_fast_assign(
		__entry->uncached = uncached;
		__entry->len = len;
		__entry->rsv_sz = rsv_sz;
		__entry->total_alloc = total_alloc;
		__entry->dur = dur;
	    ),
	    TP_printk("%-8s len %5ldKB rsv_sz %3dMB total_alloc %3dMB dur %6lu us",
			__entry->uncached ? "uncached" : "cached",
		    __entry->len,
			__entry->rsv_sz,
			__entry->total_alloc,
		    __entry->dur)
	    );

TRACE_EVENT(sys_cust_free,
	    TP_PROTO(bool uncached, long len, int rsv_sz, int total_alloc, unsigned long dur),
	    TP_ARGS(uncached, len, rsv_sz, total_alloc, dur),

	    TP_STRUCT__entry(
		__field(bool, uncached)
		__field(long, len)
		__field(int, rsv_sz)
		__field(int, total_alloc)
		__field(unsigned long, dur)
	    ),
	    TP_fast_assign(
		__entry->uncached = uncached;
		__entry->len = len;
		__entry->rsv_sz = rsv_sz;
		__entry->total_alloc = total_alloc;
		__entry->dur = dur;
	    ),
	    TP_printk(" %-8s len %5ldKB rsv_sz %3dMB total_alloc %3dMB dur %6lu us",
			__entry->uncached ? "uncached" : "cached",
			__entry->len,
			__entry->rsv_sz,
			__entry->total_alloc,
		    __entry->dur)
	    );
#endif /* _DMABUF_HEAP_TRACE_H */

/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH .
#define TRACE_INCLUDE_FILE dmabuf_heap_trace
#include <trace/define_trace.h>
