/*
 * Copyright (c) 2020, BayLibre
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/mt8183_apu/sys.c
 * @brief	machine specific system primitives implementation.
 */

#include <metal/irq.h>
#include <metal/log.h>
#include <metal/sys.h>

static char *trace_buffer = NULL;
static ssize_t trace_buffer_size = 0;

static int mt8183_vprintf(const char *p, va_list args)
{
	int len;
	unsigned int flags;

	flags = metal_irq_save_disable();

	if (!trace_buffer || trace_buffer_size <= 0) {
		metal_irq_restore_enable(flags);
		return -ENOBUFS;
	}

	len = vsnprintf(trace_buffer, trace_buffer_size, p, args);
	trace_buffer += len;
	trace_buffer_size -= len;

	metal_irq_restore_enable(flags);

	return len;
}

static int mt8183_printf(const char *p, ...)
{
	int len;
	va_list args;
	(void)

	va_start(args, p);
	len = mt8183_vprintf(p, args);
	va_end(args);

	return len;
}

void mt8183_log_handler(enum metal_log_level level, const char *format, ...)
{

	va_list args;
	static const char *level_strs[] = {
		"metal: emergency: ",
		"metal: alert:     ",
		"metal: critical:  ",
		"metal: error:     ",
		"metal: warning:   ",
		"metal: notice:    ",
		"metal: info:      ",
		"metal: debug:     ",
	};

	if (level <= METAL_LOG_EMERGENCY || level > METAL_LOG_DEBUG)
		level = METAL_LOG_EMERGENCY;

	mt8183_printf("%s", level_strs[level]);

	va_start(args, format);
	mt8183_vprintf(format, args);
	va_end(args);
}

void mt8183_trace_init(void *ptr, ssize_t size, enum metal_log_level level)
{
	trace_buffer = ptr;
	trace_buffer_size = size;

	xthal_set_region_attribute((void *)trace_buffer,
				   trace_buffer_size,
				   XCHAL_CA_WRITETHRU, 0);

	metal_set_log_handler(mt8183_log_handler);
	metal_set_log_level(level);
}
