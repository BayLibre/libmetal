/*
 * Copyright (c) 2020, BayLibre
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <metal/irq.h>
#include <metal/sys.h>

#include "rsc_table.h"

int init_system(void)
{
	int len;
	int ret;

	/* Prevent the compiler from removing the resource table */
	get_resource_table(0, &len);

	ret = mt8183_irq_init();
	if (ret)
		return ret;

	return 0;
}

void cleanup_system()
{
}


/*
 * Implement vfprintf in order to replace default print functions.
 *
 * Remoteproc provide a generic way to get trace from a remote device.
 * This implements vfprintf to write traces in a buffer shared with the host.
 * The current implementation is very basic and will stop to write traces once
 * the buffer is full.
 */
int vfprintf(FILE *stream, const char *p, va_list args)
{
	int len;
	static char *trace_buffer = NULL;
	static ssize_t trace_buffer_size = 0;
	uint32_t level;
	(void)stream;

	level = XTOS_DISABLE_EXCM_INTERRUPTS;

	/* Initialize the trace buffer and disable the cache memory */
	if (!trace_buffer) {
		struct remote_resource_table *table;
		table = get_resource_table(0, &len);
		trace_buffer = (void *)table->logbuf_hdr.da;
		trace_buffer_size = table->logbuf_hdr.len;
		xthal_set_region_attribute((void *)trace_buffer,
					   trace_buffer_size,
					   XCHAL_CA_WRITETHRU, 0);
	}

	if (trace_buffer_size <= 0) {
		XTOS_RESTORE_INTLEVEL(level);
		return -ENOBUFS;
	}

	len = vsnprintf(trace_buffer, trace_buffer_size, p, args);
	trace_buffer += len;
	trace_buffer_size -= len;

	XTOS_RESTORE_INTLEVEL(level);

	return len;
}

int fprintf(FILE *stream, const char *p, ...)
{
	int len;
	va_list args;

	va_start(args, p);
	len = vfprintf(stream, p, args);
	va_end(args);

	return len;
}

int printf(const char *p, ...)
{
	int len;
	va_list args;

	va_start(args, p);
	len = vfprintf(stdout, p, args);
	va_end(args);

	return len;
}
