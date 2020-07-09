/*
 * Copyright (c) 2020, BayLibre
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/mt8183_apu/sys.h
 * @brief	generic mt8183_apu system primitives for libmetal.
 */

#ifndef __METAL_GENERIC_SYS__H__
#error "Include metal/sys.h instead of metal/system/generic/@PROJECT_MACHINE@/sys.h"
#endif

#include <xtensa/xtruntime.h>

#ifndef __METAL_MT8183_APU_SYS__H__
#define __METAL_MT8183_APU_SYS__H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef METAL_INTERNAL

void sys_irq_enable(unsigned int vector);

void sys_irq_disable(unsigned int vector);

static inline void sys_irq_restore_enable(unsigned int flags)
{
	XTOS_RESTORE_INTLEVEL(flags);
}

static inline unsigned int sys_irq_save_disable(void)
{
	return XTOS_DISABLE_EXCM_INTERRUPTS;
}

#endif /* METAL_INTERNAL */

#define VPU_CORE_XTENSA_INT		0x00000114
#define VPU_CORE_CTL_XTENSA_INT		0x00000118
#define VPU_INT_TO_XTENSA_CLR_REG	0x0000011C
#define VPU_INT_MASK_REG		0x0000012C

void mt8183_write_reg(uint32_t addr, uint32_t value);
uint32_t mt8183_read_reg(uint32_t addr);

int mt8183_irq_init(void);
void mt8183_trace_init(void *ptr, ssize_t size, enum metal_log_level level);
void mt8183_log_handler(enum metal_log_level level, const char *format, ...);
int mt8183_vprintf(const char *p, va_list args);

#ifndef METAL_INIT_DEFAULTS
#define METAL_INIT_DEFAULTS				\
{							\
	.log_handler	= mt8183_log_handler,		\
	.log_level	= METAL_LOG_INFO,		\
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* __METAL_MT8183_APU_SYS__H__ */
