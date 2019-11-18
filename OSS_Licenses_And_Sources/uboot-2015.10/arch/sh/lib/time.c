/*
 * (C) Copyright 2009
 * Jean-Christophe PLAGNIOL-VILLARD <plagnioj@jcrosoft.com>
 *
 * (C) Copyright 2007-2012
 * Nobobuhiro Iwamatsu <iwamatsu@nigauri.org>
 *
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <sh_tmu.h>
#include <div64.h>
#include <linux/math64.h>

#define TCR_TPSC 0x07

#define TICKS_PER_SEC  (CONFIG_SYS_CLK_FREQ / CONFIG_SYS_TMU_CLK_DIV)
#define TICKS_PER_MSEC (((TICKS_PER_SEC+500)/1000))

static struct tmu_regs *tmu = (struct tmu_regs *)TMU_BASE;

unsigned long get_tbclk(void)
{
	u16 tmu_bit = (ffs(CONFIG_SYS_TMU_CLK_DIV) >> 1) - 1;
	return get_tmu0_clk_rate() >> ((tmu_bit + 1) * 2);
}

unsigned long timer_read_counter(void)
{
	return ~readl(&tmu->tcnt0);
}

static void tmu_timer_start(unsigned int timer)
{
	if (timer > 2)
		return;
	writeb(readb(&tmu->tstr) | (1 << timer), &tmu->tstr);
}

static void tmu_timer_stop(unsigned int timer)
{
	if (timer > 2)
		return;
	writeb(readb(&tmu->tstr) & ~(1 << timer), &tmu->tstr);
}

int timer_init(void)
{

	u16 tmu_bit = (ffs(CONFIG_SYS_TMU_CLK_DIV) >> 1) - 1;
	writew((readw(&tmu->tcr0) & ~TCR_TPSC) | tmu_bit, &tmu->tcr0);

	/* Divide clock by 4 */
//	*(volatile u16 *)TCR0 = 0;

	tmu_timer_stop(0);
	tmu_timer_start(0);

	return 0;
}





/*
   In theory we should return a true 64bit value (ie something that doesn't
   overflow). However, we don't. Therefore if TMU runs at fastest rate of
   6.75 MHz this value will wrap after u-boot has been running for approx
   10 minutes.
*/
unsigned long long get_ticks (void)
{
	return (0 - *((volatile u32 *) TCNT0));
}

unsigned long get_timer (unsigned long base)
{
        return (div_u64(get_ticks(), TICKS_PER_MSEC))-base;
}

void set_timer (unsigned long t)
{
	*((volatile unsigned int *) TCNT0) = (0 - t);
}

void reset_timer (void)
{
	tmu_timer_stop(0);
	set_timer (0);
	tmu_timer_start(0);
}
 
void udelay (unsigned long usec)
{
	unsigned int start = get_ticks();
	unsigned int end;

        end = start + (unsigned int)div_u64((((unsigned long long) usec * TICKS_PER_SEC) + 500000), 1000000);

	while ((unsigned int)get_ticks () < end)
		continue;
}
