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
	struct remote_resource_table *table;

	table = get_resource_table(0, &len);
	mt8183_trace_init((void *)table->logbuf_hdr.da, table->logbuf_hdr.len,
			  METAL_LOG_INFO);

	ret = mt8183_irq_init();
	if (ret)
		return ret;

	return 0;
}

void cleanup_system()
{
}
