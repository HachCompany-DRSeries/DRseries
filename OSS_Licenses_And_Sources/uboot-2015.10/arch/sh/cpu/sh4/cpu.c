/*
 * (C) Copyright 2007
 * Nobuhiro Iwamatsu <iwamatsu@nigauri.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <netdev.h>
#include <asm/processor.h>
#include <asm/cache.h>
#include <asm/io.h>

#ifdef CONFIG_SH4A
# define CPU "SH4A"
#else
# define CPU "SH4"
#endif

#define MHZ    		1000000
#define TO_MHZ(clock) 	(((clock)+(MHZ/2))/MHZ)

extern void sh_clock_init( void ); 

int checkcpu(void)
{
        DECLARE_GLOBAL_DATA_PTR;

        u32 exp;

	printf("CPU:   " CPU " @ %u MHz\n", TO_MHZ( gd->bd->clocks.cpu ) );

        /* print reset cause, \see [1], p.122 */
        exp = ioread32( EXPEVT );
        if( exp ) {
                puts( "Reset: " );

                switch( ioread32( EXPEVT ) ) {
                    case 0x20:  puts( "Manual" ); break;
                    case 0x140: printf( "TLB Multiple Hit 0x%08x", ioread32( TRA ) ); break;
                    default:    printf( "Unknown 0x%08x", exp );
                }
                puts( "\n" );
        }

	return 0;
}

int cpu_init (void)
{
        sh_clock_init();
        
	return 0;
}

int cleanup_before_linux (void)
{
	disable_interrupts();
	return 0;
}

int do_reset (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	disable_interrupts();
	reset_cpu (0);
	return 0;
}

int cpu_eth_init(bd_t *bis)
{
#ifdef CONFIG_SH_ETHER
	sh_eth_initialize(bis);
#endif
	return 0;
}
