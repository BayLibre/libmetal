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
#include <metal/irq_controller.h>
#include <metal/sys.h>
#include <metal/utilities.h>

#include <xtensa/hal.h>
#include <xtensa/xtruntime.h>
#include <xtensa/tie/ipu_if2.h>
#include <xtensa/tie/xt_interrupt.h>

#define MAX_IRQS 25
static struct metal_irq irqs[MAX_IRQS];

static void mt8183_irq_enable(int32_t number)
{
	uint32_t value;

	value =  XT_RSR_INTENABLE();
	value |= (1 << number);
	xtos_interrupt_enable(value);
	_xtos_set_intlevel(0);

	value = mt8183_read_reg(VPU_INT_MASK_REG);
	value &= ~(1 << number);
	mt8183_write_reg(VPU_INT_MASK_REG, value);
}

static void mt8183_irq_disable(uint32_t number)
{
	uint32_t value;

	value = mt8183_read_reg(VPU_INT_MASK_REG);
	value |= (1 << number);
	mt8183_write_reg(VPU_INT_MASK_REG, value);

	value = XT_RSR_INTENABLE();
	value &= ~(1 << number);
	xtos_interrupt_disable(value);
}

static void mt8183_irq_clear(uint32_t number)
{
	mt8183_write_reg(VPU_INT_TO_XTENSA_CLR_REG, 1 << number);
}

static void mt8183_irq_handler(uint32_t number)
{
	if (irqs[number].hd)
		irqs[number].hd((int)number, irqs[number].arg);
	mt8183_irq_clear(number);
}

static void mt8183_irq_set_enable(struct metal_irq_controller *cntr,
				  int irq, unsigned int state)
{
	if (irq < cntr->irq_base ||
	    irq >= cntr->irq_base + cntr->irq_num) {
		metal_log(METAL_LOG_ERROR, "%s: invalid irq %d\n",
			  __func__, irq);
		return;
	} else if (state == METAL_IRQ_ENABLE) {
		mt8183_irq_enable(irq);
		sys_irq_enable((unsigned int)irq);
	} else {
		sys_irq_disable((unsigned int)irq);
		mt8183_irq_disable(irq);
	}
}

static int mt8183_irq_register(struct metal_irq_controller *cntr,
			       int irq, metal_irq_handler hd, void *arg)
{
	metal_unused(arg);

	if (irq < cntr->irq_base ||
	    irq >= cntr->irq_base + cntr->irq_num)
		return -EINVAL;

	if (!hd) {
		_xtos_set_interrupt_handler_arg(irq, NULL, NULL);
	} else {
		irqs[irq].hd = hd;
		irqs[irq].arg = arg;
		_xtos_set_interrupt_handler_arg(irq, mt8183_irq_handler,
						(void *)irq);
	}

	return 0;
}


static METAL_IRQ_CONTROLLER_DECLARE(mt8183_irq_controller, 0, MAX_IRQS, NULL,
				    mt8183_irq_set_enable, mt8183_irq_register,
				    irqs);

int mt8183_irq_init(void)
{
	mt8183_write_reg(VPU_INT_MASK_REG, 0xFFFF);

	return metal_irq_register_controller(&mt8183_irq_controller);
}
