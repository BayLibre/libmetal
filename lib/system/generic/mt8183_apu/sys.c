/*
 * Copyright (c) 2020, BayLibre
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/mt8183_apu/sys.c
 * @brief	machine specific system primitives implementation.
 */

#include <metal/compiler.h>
#include <metal/io.h>
#include <metal/sys.h>
#include <metal/utilities.h>
#include <stdint.h>

#include <xtensa/hal.h>
#include <xtensa/xtruntime.h>
#include <xtensa/tie/ipu_if2.h>
#include <xtensa/tie/xt_interrupt.h>

/* TODO Set up it from Linux kernel remoteproc / virtio driver */
static uint32_t base_addr = 0x19180000;

void mt8183_write_reg(uint32_t addr, uint32_t value)
{
	uint32_t level;

	if (!base_addr) {
		metal_log(METAL_LOG_ERROR, "MMIO address not set!\n");
		return;
	}

	level = XTOS_DISABLE_EXCM_INTERRUPTS;
	IPU_SendAPBWrite(value, base_addr + addr);
	XTOS_RESTORE_INTLEVEL(level);
}

uint32_t mt8183_read_reg(uint32_t addr)
{
	uint32_t value;

	if (!base_addr) {
		metal_log(METAL_LOG_ERROR, "MMIO address not set!\n");
		return 0;
	}

	IPU_SendAPBRead(value, base_addr + addr);
	value = IPU_ReadAPB();

	return value;
}

void sys_irq_enable(unsigned int vector)
{
	_xtos_interrupt_enable(vector);
}

void sys_irq_disable(unsigned int vector)
{
	_xtos_interrupt_disable(vector);
}

void metal_machine_cache_flush(void *addr, unsigned int len)
{
	xthal_dcache_block_writeback(addr, len);
}

void metal_machine_cache_invalidate(void *addr, unsigned int len)
{
	xthal_dcache_block_invalidate(addr, len);
}

void metal_generic_default_poll(void)
{
	XT_WAITI(0);
}

void *metal_machine_io_mem_map(void *va, metal_phys_addr_t pa,
			       size_t size, unsigned int flags)
{
	metal_unused(pa);
	metal_unused(size);
	metal_unused(flags);

	return va;
}
