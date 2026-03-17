/*
 * cpu/sh4/speed.c
 *
 * Copyright (c) 2008 by emtrion GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Author:      Markus Pietrek
 * Revision:    $Revision$
 * Description: Provides clock determination functions
 * References:
 *   [1] file://W:/Datenblaetter/Renesas/SH-Mobile/SH7723/SH7723_Rev1.00_Eng_edi1958.pdf
 *   [2] file://W:/Datenblaetter/Renesas/SH-Mobile/SH7722/rej09b0314_sh7722hm_v0200_edi1985.pdf
 **/

#include <common.h>

#include <asm/cpu_sh4.h>        /* FRQCR */
#include <asm/io.h>             /* ioread32 */
#include <div64.h>
#include <linux/math64.h>

#define FACTOR 1000000

static inline u32 sh_ratio_to_hz( u32 ratio1 )
{
        DECLARE_GLOBAL_DATA_PTR;

//        return ( (u64) ratio * gd->bd->clocks.core ) / FACTOR;
        return ( (u32) div_u64(((u64) ratio1 * gd->bd->clocks.core ), FACTOR));
}


void sh_clock_init( void )
{
        DECLARE_GLOBAL_DATA_PTR;
/* float variable assignments don't seem to work with
   sh4-unknown-linux-gnu-gcc 4.2.2 */
/* see [1] 14.5.1 and [2] 14.5.1. Multiplicatos can be merged */
#define MK(idx,a,b) ((a) * FACTOR ) / (b)
        static const u64 ratio[ 16 ] = {
#ifdef CONFIG_CPU_SH7723
                MK( 0x0,  1,  1 ),
                MK( 0x1,  2,  3 ),
                MK( 0x2,  1,  2 ),
                MK( 0x3,  2,  5 ),
                MK( 0x4,  1,  3 ),
                MK( 0x5,  1,  4 ),
                MK( 0x6,  1,  5 ),
                MK( 0x7,  1,  6 ),
                MK( 0x8,  1,  8 ),
                MK( 0x9,  1, 10 ),
                MK( 0xA,  1, 12 ),
                MK( 0xB,  1, 16 ),
                MK( 0xC,  1, 20 ),
                MK( 0xD,  0,  1 ),
                MK( 0xE,  0,  1 ),
                MK( 0xF,  0,  1 ),
#elif CONFIG_CPU_SH7724
                MK( 0x0,  1,  2 ),
                MK( 0x1,  1,  3 ),
                MK( 0x2,  0,  1 ),
                MK( 0x3,  1,  6 ),
                MK( 0x4,  1,  8 ),
                MK( 0x5,  1, 12 ),
                MK( 0x6,  1, 16 ),
                MK( 0x7,  0,  1 ),
                MK( 0x8,  1, 24 ),
                MK( 0x9,  1, 32 ),
                MK( 0xA,  1, 36 ),
                MK( 0xB,  1, 48 ),
                MK( 0xC,  0,  1 ),
                MK( 0xD,  1, 72 ),
                MK( 0xE,  0,  1 ),
                MK( 0xF,  0,  1 ),
#endif
        };
#undef MK
#ifdef CONFIG_CPU_SH7723
        u32 frqcr  = ioread32( FRQCR  );
	u32 flf     = ioread32( DLLFRQ ) & 0x7FF;
        u64 ratio_sdram = ratio[ ( frqcr >>  4 ) & 0xF ];
#elif CONFIG_CPU_SH7724
        u32 frqcr   = ioread32( FRQCRA );
	u32 flf     = ioread32( FLLFRQ ) & 0x7FF;
#endif
        u32 pllcr   = ioread32( PLLCR  );
        u64 ratio_cpu   = ratio[ ( frqcr >> 20 ) & 0xF ];
        u64 ratio_sh    = ratio[ ( frqcr >> 12 ) & 0xF ];
        u64 ratio_bus   = ratio[ ( frqcr >>  8 ) & 0xF ];
        u64 ratio_p0    = ratio[ ( frqcr >>  0 ) & 0xF ];

	gd->bd->clocks.pll = ( ( frqcr >> 24 ) & 0x1F ) + 1;
#ifdef CONFIG_CPU_SH7724
	gd->bd->clocks.pll *= 2;
	if ((ioread32(FLLFRQ) & FLLFRQ_SELXM_MASK) == FLLFRQ_SELXM_DIV2)
		flf /= 2;
#endif

        if( !(pllcr & PLLCR_PLLE) )
                gd->bd->clocks.core = CONFIG_SYS_CLK_FREQ * gd->bd->clocks.pll;
        else
                gd->bd->clocks.core = 32768 * gd->bd->clocks.pll * flf;

        gd->bd->clocks.cpu   = sh_ratio_to_hz( ratio_cpu );
        gd->bd->clocks.sh    = sh_ratio_to_hz( ratio_sh );
        gd->bd->clocks.bus   = sh_ratio_to_hz( ratio_bus );
#ifdef CONFIG_CPU_SH7723
        gd->bd->clocks.sdram = sh_ratio_to_hz( ratio_sdram );
#endif
        gd->bd->clocks.p0    = sh_ratio_to_hz( ratio_p0 );
}
