/*
 * (C) Copyright 2007
 * Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <asm/cache.h>

/*
 * Jump to P2 area.
 * When handling TLB or caches, we need to do it from P2 area.
 */
#define jump_to_P2()			\
  do {					\
    unsigned long __dummy;		\
    __asm__ __volatile__(		\
		"mov.l	1f, %0\n\t"	\
		"or	%1, %0\n\t"	\
		"jmp	@%0\n\t"	\
		" nop\n\t"		\
		".balign 4\n"		\
		"1:	.long 2f\n"	\
		"2:"			\
		: "=&r" (__dummy)	\
		: "r" (0x20000000));	\
  } while (0)

/*
 * Back to P1 area.
 */
#define back_to_P1()					\
  do {							\
    unsigned long __dummy;				\
    __asm__ __volatile__(				\
		"nop;nop;nop;nop;nop;nop;nop\n\t"	\
		"mov.l	1f, %0\n\t"			\
		"jmp	@%0\n\t"			\
		" nop\n\t"				\
		".balign 4\n"				\
		"1:	.long 2f\n"			\
		"2:"					\
		: "=&r" (__dummy));			\
  } while (0)

#define CACHE_VALID       1
#define CACHE_UPDATED     2

#define CACHE_MODE	  ( CCR_WT | CCR_CB )

static inline void cache_wback_all(void)
{
	unsigned long addr, data, i, j;

	jump_to_P2();
	for (i = 0; i < CACHE_OC_NUM_ENTRIES; i++){
		for (j = 0; j < CACHE_OC_NUM_WAYS; j++) {
			addr = CACHE_OC_ADDRESS_ARRAY | (j << CACHE_OC_WAY_SHIFT)
				| (i << CACHE_OC_ENTRY_SHIFT);
			data = inl(addr);
			if (data & CACHE_UPDATED) {
				data &= ~CACHE_UPDATED;
				outl(data, addr);
			}
		}
	}
	back_to_P1();
}


#define CACHE_ENABLE      0
#define CACHE_DISABLE     1

#if 0
int cache_control(unsigned int cmd)
{
	unsigned long ccr;

	jump_to_P2();
	ccr = inl(CCR);

	if (ccr & CCR_CACHE_ENABLE)
		cache_wback_all();

	if (cmd == CACHE_DISABLE)
		outl(CCR_CACHE_STOP, CCR);
	else
		outl(CCR_CACHE_INIT, CCR);
	back_to_P1();

	return 0;
}

void flush_dcache_range(unsigned long start, unsigned long end)
{
	u32 v;

	start &= ~(L1_CACHE_BYTES - 1);
	for (v = start; v < end; v += L1_CACHE_BYTES) {
		asm volatile ("ocbwb     %0" :	/* no output */
			      : "m" (__m(v)));
	}
}

void invalidate_dcache_range(unsigned long start, unsigned long end)
{
	u32 v;

	start &= ~(L1_CACHE_BYTES - 1);
	for (v = start; v < end; v += L1_CACHE_BYTES) {
		asm volatile ("ocbi     %0" :	/* no output */
			      : "m" (__m(v)));
	}
}
#endif

int icache_control(unsigned int cmd)
{
	unsigned long ccr;

	jump_to_P2();
	ccr = ioread32( CCR );

	if (cmd)
		iowrite32( ccr & ~CCR_ICE, CCR );
	else
		iowrite32( ccr | ( CCR_ICE | CCR_ICI | CACHE_MODE ), CCR );
	back_to_P1();

	return 0;
}


void flush_cache (unsigned long addr, unsigned long size)
{
	dcache_wback_range( addr , addr + size );
}

int icache_status (void)
{
	return (ioread32( CCR ) & CCR_ICE) ? 1 : 0;
}

void icache_enable (void)
{
        if( icache_status() )
                // already enabled, e.g. in lowlevel_init. no need to do an invalidate
                return;

	icache_control(0);
}

void icache_disable (void)
{
        if( !icache_status() )
                return;

	icache_control(1);
}

int dcache_status (void)
{
        return ( ioread32( CCR ) & CCR_OCE ) ? 1 : 0;
}

void dcache_enable (void)
{
        if( dcache_status() )
                return;

	jump_to_P2();
        iowrite32( ioread32( CCR ) | ( CCR_OCE | CCR_OCI | CACHE_MODE ), CCR );
	back_to_P1();
}

void dcache_disable (void)
{
        if( !dcache_status() )
                return;

        cache_wback_all();
	jump_to_P2();
        iowrite32( ioread32( CCR ) & ~CCR_OCE, CCR );
	back_to_P1();
}

void flush_dcache_all(void)
{
	dcache_disable();
	dcache_enable();
}
