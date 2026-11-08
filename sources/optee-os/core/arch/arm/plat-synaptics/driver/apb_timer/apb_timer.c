// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2024, Synaptics Incorporated.
 */

#include <initcall.h>
#include <kernel/interrupt.h>
#include <trace.h>
#include <kernel/pm.h>
#include "io.h"
#include "soc.h"
#include "platform_config.h"
#include "apbRegBase.h"
#include "apb_timer_reg.h"
#include "apb_timer.h"

#define timer_readl		io_read32
#define timer_writel(v, a) do {			\
	io_write32(a, v);					\
} while(0)

#define APB_TIMER1_VIRTBASE (vaddr_t)phys_to_virt_io(APB_TIMER1_BASE, i_apbPerif_timers_1_AddressRange)

#define CONFIG_SW_TIMER_PERIOD	(CONFIG_TIMER_CLOCK * 20UL)
#define CYCLE_TO_US(cycles)	((cycles) / (CONFIG_TIMER_CLOCK / 1000000))

/*
 * start count down from 0xffff_ffff. this is done by toggling the enable bit
 * then load initial load count to ~0.
 */
void timer_start(uint32_t base, enum timer_mode mode, uint32_t period)
{
	uint32_t ctrl;

	/* disable the timer first */
	timer_stop(base);

	/* for free running, must set period as 0xffffffff */
	if (mode == TIMER_MODE_FREE_RUNNING)
		period = 0xffffffff;

	/* set the load count */
	timer_writel(period, base + TMR_LOAD_COUNT);

	/* enable, unmask interrupt, set to specified mode */
	ctrl = TMR_CONTROL_ENABLE;
	if (mode == TIMER_MODE_PERIODIC)
		ctrl |= TMR_CONTROL_MODE_PERIODIC;

	timer_writel(ctrl, base + TMR_CONTROL);
}

void timer_stop(uint32_t base)
{
	uint32_t ctrl = timer_readl(base + TMR_CONTROL);

	timer_readl(base + TMR_EOI);	/* clear any pending interrupt */
	ctrl &= ~TMR_CONTROL_ENABLE;
	timer_writel(ctrl, base + TMR_CONTROL);
}

uint32_t timer_read_count(uint32_t base)
{
	return 0xffffffff - timer_readl(base + TMR_CURRENT_VALUE);
}

bool timer_check_interrupt(uint32_t base)
{
	uint32_t status = timer_readl(base + TMR_INT_STATUS);
	return (status & TMR_INT_STATUS_PENDING);
}

void timer_clear_interrupt(uint32_t base)
{
	timer_readl(base + TMR_EOI);
}

void timer_enable_interrupt(uint32_t base)
{
	uint32_t ctrl = timer_readl(base + TMR_CONTROL);

	/* clear pending interrupt */
	timer_readl(base + TMR_EOI);
	ctrl &= ~TMR_CONTROL_INT_MASK;
	timer_writel(ctrl, base + TMR_CONTROL);
}

void timer_disable_interrupt(uint32_t base)
{
	uint32_t ctrl = timer_readl(base + TMR_CONTROL);

	ctrl |= TMR_CONTROL_INT_MASK;
	timer_writel(ctrl, base + TMR_CONTROL);
}

static uint64_t clock_cycles_base = 0;

static uint64_t clocksource_read_cycles(void)
{
	uint64_t cycles = 0;
	uint32_t current = 0;

	do {
		cycles = clock_cycles_base;
		dsb();
		current = timer_read_count(APB_TIMER1_VIRTBASE);
		dsb();
	} while (cycles != clock_cycles_base);

	cycles += current;

	return cycles;
}

void syna_get_secure_time(TEE_Time *time)
{
	uint64_t cycles = clocksource_read_cycles();
	uint64_t us = CYCLE_TO_US(cycles);

	if (time) {
		time->seconds = us / 1000000;
		time->millis = (us % 1000000) / 1000;
	}
}

static enum itr_return timer1_it_handler(struct itr_handler *handler __unused)
{
	timer_clear_interrupt(APB_TIMER1_VIRTBASE);
	clock_cycles_base += CONFIG_SW_TIMER_PERIOD;

	return ITRR_HANDLED;
}

static struct itr_handler *timer1_itr_handler;

TEE_Result secure_timer_stop(void)
{
	timer_stop(APB_TIMER1_VIRTBASE);
	clock_cycles_base += timer_read_count(APB_TIMER1_VIRTBASE);

	interrupt_disable(timer1_itr_handler->chip, timer1_itr_handler->it);
	interrupt_remove_free_handler(timer1_itr_handler);

	return TEE_SUCCESS;
}

TEE_Result secure_timer_configuration(void)
{
	TEE_Result res = TEE_ERROR_GENERIC;
	res = interrupt_alloc_add_conf_handler(interrupt_get_main_chip(),
					       CONFIG_TIMER1_ICTL_IRQ, timer1_it_handler, 0, NULL,
					       IRQ_TYPE_NONE, 0, &timer1_itr_handler);

	interrupt_enable(timer1_itr_handler->chip, timer1_itr_handler->it);

	timer_start(APB_TIMER1_VIRTBASE, TIMER_MODE_PERIODIC, CONFIG_SW_TIMER_PERIOD);

	return TEE_SUCCESS;
}

static TEE_Result
pm_enter_resume(enum pm_op op, uint32_t pm_hint __unused,
		const struct pm_callback_handle *pm_handle __unused)
{
	DMSG("op = 0x%x.\n", op);
	if (op == PM_OP_RESUME)
		return secure_timer_configuration();
	if(op == PM_OP_SUSPEND)
		return secure_timer_stop();

	return TEE_SUCCESS;
}

TEE_Result secure_timer_setup(void)
{
	secure_timer_configuration();

	register_pm_driver_cb(pm_enter_resume, NULL, "syna apb timer");

	return TEE_SUCCESS;
}

driver_init(secure_timer_setup);
