/*
 * Copyright (c) 2020, BayLibre
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	xtensa/mt8183_apu/irq.h
 * @brief	mt8183_apu irq primitives for libmetal.
 */

#ifndef __METAL_GENERIC_IRQ__H__
#error "Include metal/irq.h instead of metal/system/generic/@PROJECT_MACHINE@/irq.h"
#endif

#include <xtensa/xtruntime.h>

#ifndef __METAL_MT8183_APU_IRQ__H__
#define __METAL_MT8183_APU_IRQ__H__

#ifdef __cplusplus
extern "C" {
#endif

int mt8183_irq_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __METAL_MT8183_APU_IRQ__H__ */
