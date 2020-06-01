/*
 * Copyright (c) 2020, BayLibre
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/mt8183_apu/atomic.c
 * @brief	machine specific atomic primitives implementation.
 */

#include <metal/atomic.h>
#include <metal/irq.h>

#include <stdint.h>

void __sync_synchronize(void) {}

uint32_t __sync_lock_test_and_set_4(uint32_t *ptr, uint32_t val)
{
	uint32_t tmp;
	unsigned int flags;

	flags = metal_irq_save_disable();
	tmp = *ptr;
	*ptr = val;
	metal_irq_restore_enable(flags);

	return tmp;
}

uint32_t __sync_fetch_and_add_4(uint32_t *ptr, uint32_t val)
{
	uint32_t tmp;
	unsigned int flags;

	flags = metal_irq_save_disable();
	tmp = *ptr;
	*ptr = tmp + val;
	metal_irq_restore_enable(flags);

	return tmp;
}

void __sync_lock_release_4(uint32_t *ptr)
{
	unsigned int flags;

	flags = metal_irq_save_disable();
	*ptr = 0;
	metal_irq_restore_enable(flags);
}
