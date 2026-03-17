/*
 * Copyright (C) 2008 Renesas Solutions Corp.
 * Copyright (C) 2008 Nobuhiro Iwamatsu <iwamatsu.nobuhiro@renesas.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <netdev.h>
#include <asm/io.h>
#include <asm/processor.h>
#include <usb.h>
#include <i2c.h> 
#include <rtc.h> 
#include "display.h"

extern int dm9000_eth_initialize(bd_t *bis);

extern void setup_display(void);


int warmboot = 0;

int get_board_cfg(void)
{
        return (ioread8(PXDR) & 0xF);
}

int get_board_cfg_boot_immediately(void)
{
        return ((get_board_cfg() & BOARD_CFG_BOOT_IMMEDIATELY) != 0);
}

int get_board_cfg_base_access_allowed( void )
{
        return ((get_board_cfg() & BOARD_CFG_BASE_ACCESS_ALLOWED) == 0);
}

void board_set_usbh_pen( int on_off ) {
	u8 pfdr = ioread8(PFDR);
	
	pfdr = on_off ?
                (pfdr & ~PIN_DATA(USBH_PEN)) :
                (pfdr | PIN_DATA(USBH_PEN));
  
        iowrite8(pfdr, PFDR);
}

void board_set_nand_wp(int protected)
{
        u8 pkdr = ioread8(PKDR);

        pkdr = protected ?
                (pkdr & ~PIN_DATA(NAND_WP)) :
                (pkdr | PIN_DATA(NAND_WP));

        iowrite8(pkdr, PKDR);
}

void board_set_nor_wp(int protected)
{
        u8 ptdr = ioread8(PTDR);
	extern flash_info_t flash_info[];

        ptdr = protected ?
                (ptdr & ~PIN_DATA(NOR_WP)) :
                (ptdr | PIN_DATA(NOR_WP));

        iowrite8(ptdr, PTDR);

	if (protected) {
		/* setting WP# to low is not enough. All blocks need to be locked down for protection to take effect */
		flash_info_t* chip = &flash_info[0];
		flash_protect(FLAG_PROTECT_SET, chip->start[0], chip->start[0] + chip->size - 1, chip);
	}
	else {
		/* setting WP# to low is not enough. All blocks need to be locked down for protection to take effect */
		flash_info_t* chip = &flash_info[0];
		flash_protect(FLAG_PROTECT_CLEAR, chip->start[0], chip->start[0] + chip->size - 1, chip);
	}
}

int checkboard(void)
{
	if (ioread8(RWTCSR) & RWTCSR_WOVF) {
		/* The watchdog did overrun and issued a CPU reset. While not documented,
		   Renesas support has stated that this flag is not reset by a watchdog reset. */
		printf("Doing hard reset due to former watchdog reset\n");
		warmboot = 1;
	}
	else {
		warmboot = 0;
	}

	/* Disable watchdog. As the watchdog is active after reset,
	   we have to disable the watchdog before it triggers after 32s */
	iowrite16(RWTCSR_MAGIC, RWTCSR);

	printf("BOARD: HiCO.DIMM7723 (SW1=0x%02x)\n", get_board_cfg());

#ifdef CONFIG_BOOT_PROFILE
        printf("Boot profiling enabled on J17-10. Stop with \"mw.b 0xa405016c 0x0\"\n");
#endif

	return 0;
}

int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

        if (get_board_cfg_boot_immediately())
                gd->flags |= GD_FLG_SILENT;

        flash_set_verbose(0);


	return 0;
}

int board_eth_init(bd_t *bis)
{
        dm9000_eth_initialize(bis);
        return dm9000_initialize(bis);
}

int dram_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

        /* \todo read sdram settings */
	gd->bd->bi_memstart = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_memsize  = CONFIG_SYS_SDRAM_SIZE;

	printf("SDRAM: ");
        print_size(gd->bd->bi_memsize, "\n");
	return 0;
}

void reset_cpu(unsigned long ignored)
{
        serial_tx_flush();

        /* do a reset by the reset controller */
        iormw8(PFDR, | PIN_DATA( SOFTRESET));
}



void board_backlight_enable(display_info_t* display)
{
      /* enable clock for TPU module */
      iowrite32((ioread32(MSTPCR2) & ~( 1 << 25 )), MSTPCR2); 	

      iowrite16(0x0000, TPU_TSTR); /* stopping all timer */
      iowrite16(0x0043, TPU_TCR0); /* for TPU0 internal clock / 64, TPU_TCNT cleared by TPU_TGRA compare match */
      iowrite16(0x0005, TPU_TIOR0); /* initial output 1, 0 on compare match */
      iowrite16(0x0000, TPU_TIER0); /* no interrupts */
      iowrite16(0x0003, TPU_TSR0); /* clearing flags of compare C, D, V */

      /* setting general and control register for Timer 0 */
      iowrite16(0x0035, TPU_TGR0A);
      iowrite16(0x0034, TPU_TGR0B);
      iowrite16(0xffff, TPU_TGR0C);
      iowrite16(0xffff, TPU_TGR0D);
      iowrite16(0x0002, TPU_TMDR0); /* running in PWM mode */

      /* setting PTG Multiplex to TPU function! */
      iowrite16((ioread16(PSELA) & ~0xc) | 0x4, PSELA); 	

      iowrite16(0x0001, TPU_TSTR); /* starting timer 0 */


/* switching on display */
      iormw8(PHDR, | PIN_DATA(LCD_DON));
} 



int board_late_init(void)
{
//	const char *base_board_code;

	DECLARE_GLOBAL_DATA_PTR;

	board_set_nor_wp(0);
	board_set_nand_wp(0);
	
	/* I2C is not available before board_late_init */
#if 0
	base_board_code = getenv("hw_base_board_code");
	if (base_board_code && strlen(base_board_code))
		printf("Base:  %s\n", base_board_code);
#endif
	
 	u32 timer = (CONFIG_SYS_HZ * 20) / 1000;
 	u32 time_start;
	int i;

        char boot_reason[15];

	if ( warmboot ) {
		sprintf(boot_reason, "wdt");
	}
	else {
		sprintf(boot_reason, "por");
	}
	setenv("boot_reason", boot_reason);

	for ( i = 0; i < 10; i++ ) {
	  time_start = get_timer(0);
	  while (get_timer(time_start) < timer) {
	    status_led_set(STATUS_LED_BOARD_UP, STATUS_LED_ON);
	  }

	  time_start = get_timer(0);
	  while (get_timer(time_start) < timer) {
	    status_led_set(STATUS_LED_BOARD_UP, STATUS_LED_OFF);
	  }
	}
fdtdec_setup();
	
        /* we should have a prompt now or start booting */
        status_led_set(STATUS_LED_BOARD_UP, STATUS_LED_ON);

        if (!(gd->flags & (GD_FLG_POSTFAIL)))
                status_led_set(STATUS_LED_FAILURE, STATUS_LED_OFF);

        /* LCD has priority */
        {
                /* see [1], Figure 1.1 and Figure D.1
                   Both, ICB and LMB needs to be increased to get the LCD
                   flickering free. */
                u32 prio = GPR_PRI_VSET(0xF) | GPR_PRI_EN_VALUE;
                iowrite32(prio, GPR_PRI_ICB);
                iowrite32(prio, GPR_PRI_LMB);
        }

#if defined(CONFIG_DISPLAY)
	setup_display();
#endif
//        board_backlight_enable();
        
        return 0;
}


int board_early_init_f(void)
{
	return 0;
}

int board_usb_init(int index, enum usb_init_type init)
{
	printf("USB: power enable\n");
	board_set_usbh_pen( 0 );
	
	return 0;
}
