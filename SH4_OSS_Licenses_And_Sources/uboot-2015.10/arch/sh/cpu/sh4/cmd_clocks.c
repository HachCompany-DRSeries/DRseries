/*
 * cpu/sh4/cmd_clocks.c
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
 * Description: Provides clock output for SH772x
 * References:
 *   [1] file://W:/Datenblaetter/Renesas/SH-Mobile/SH7723/SH7723_Rev1.00_Eng_edi1958.pdf
 *
 **/

#include <common.h>
#include <command.h>            /* U_BOOT_CMD */

#include <asm/cpu_sh4.h>        /* FRQCR */
#include <asm/io.h>             /* ioread32 */

#define MHZ    1000000
#define TO_MHZ(clock) (((clock)+(MHZ/2))/MHZ)

static int do_clocks( cmd_tbl_t* cmdtp, int flag, int argc, char* const argv[] )
{
        DECLARE_GLOBAL_DATA_PTR;

        puts( "Clock configuration:\n" );

        if( gd->bd->clocks.pll )
                printf( "PLL Ratio:  %u\n", gd->bd->clocks.pll );
        else
                printf( "DLL Mode\n" );

        printf( "CPU:   %4u MHz\n", TO_MHZ( gd->bd->clocks.cpu   ) );
        printf( "SH:    %4u MHz\n", TO_MHZ( gd->bd->clocks.sh    ) );
        printf( "Bus:   %4u MHz\n", TO_MHZ( gd->bd->clocks.bus   ) );
#ifdef CONFIG_CPU_SH7723
        printf( "SDRAM: %4u MHz\n", TO_MHZ( gd->bd->clocks.sdram ) );
#endif
        printf( "P0:    %4u MHz\n", TO_MHZ( gd->bd->clocks.p0    ) );

        return 0;
}

U_BOOT_CMD(
	clocks,	1,	1,	do_clocks,
	"prints the CPU clocks",
        NULL
);
