/*
 * cpu/sh4a/displays.c
 *
 * Copyright (c) 2009 by emtrion GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Author:      Markus Pietrek
 * Revision:    $Revision$
 * Description: The setup is identical to Linux. Pictures can be copied from
 *              /dev/fb0 and used as bootlogos
 * References:
 *   [1] file://R:\8_renesas\HiCO.DIMM\HiCO.DIMM7723\HW\Baugruppen\Source\Test\DIMM7723_V3/hwinit.c 
 *   [2] file://linux-2.6.28/arch/sh/boards/mach-emtrion/hico7723/setup.c
 *   [3] file://linux-2.6.28/drivers/video/sh_mobile_lcdcfb.c
 *
 **/

#include <common.h>
#include <malloc.h>

#include <asm/io.h>
#include <asm/processor.h>

#include "display.h"

/* alignment of 8 is hardware requirement. But operating systems have it easier
 * to reuse our framebuffer when aligned on page boundary, which is at max 64KiB */
#define ALIGN_FB 65536

# define LCDDISP_PORTC	PHCR
# define LCDDISP_PORTD	PHDR
# define LCDDISP_PIN	4

void *disp_base;

int display_board_init(display_info_t *display)
{
        u32 timeout;
        u32 tmp;
        u32 miftyp;
        void *fbbuf_unaligned = NULL;
        char disp_base_str[15];
 
        // allocate framebuffer
        display->fbsize = display->mode.xres * display->mode.yres * display->bpp/8 + 0x4000;
	
	fbbuf_unaligned  = malloc(display->fbsize+ALIGN_FB);
	if (fbbuf_unaligned == NULL) {
		printf("not enough memory for framebuffer\n");
		goto error;
	}
	
        display->fbbuf  = (void*) __ALIGN_MASK((int) fbbuf_unaligned, ALIGN_FB-1);
        // place the framebuffer in the uncached area
        display->fbbuf  = (void*) (ADDR_TO_P2(display->fbbuf));
	disp_base = display->fbbuf;
	sprintf(disp_base_str, "0x%08x", (unsigned int)disp_base);
	setenv("disp_base", disp_base_str);
	
	// enable module clocks
	udelay(1); /* The LCD controller is clocked slower than the CPU. Ensure that the LCD controller had at least one clock received before writing to it */

        // module reset
        iowrite32(LDCNT2R_BR | LDCNT2R_ME, LDCNT2R);
        udelay(10000);
        iowrite32(LDCNT2R_ME, LDCNT2R);

        iowrite32(0, LDCNT1R); /* nothing to display yet */
        iowrite32(LDRCNTR_SIDEA, LDRCNTR);

        iowrite32(LDDCKSTPR_DCKSTP, LDDCKSTPR); /* no clock while configuring
                                                   * */
        display->board.ldmt2r = 0;

	/* external 26MHz Pixelclock coming from Hardware */
	iowrite16((ioread16(PSELA) & ~0x3000) | 0x1000, PSELA);
        iowrite32((1 << 6) | (2 << 16), LDDCKR);

	iowrite32(0, LDDCKSTPR); /* start clock */

        /* wait for clock to be running */
        timeout = 0;
        while (ioread32(LDDCKSTPR) & LDDCKSTPR_DCKSTS) {
                udelay(1);

                timeout++;
                if (timeout >= 1000000) {
                        printf("LCD clock not running\n");
                        goto error;
                }
        }

        iowrite32(MLDDFR_PKF_RGB16, MLDDFR);
        iowrite32(0, MLDSM1R);
        iowrite32(0, MLDWBCNTR);

        iowrite32(0, LDINTR); // disable interrupts

        iowrite32(LDDDSR_LS |
                  LDDDSR_WS, LDDDSR);

        /* write display parameters */
	switch (display->interface_type) {
            case RGB8:   miftyp = 0; break;
            case RGB9:   miftyp = 4; break;
            case RGB12A: miftyp = 5; break;
            case RGB12B: miftyp = 6; break;
            case RGB16:  miftyp = 7; break;
            case RGB18:  miftyp = 10; break;
            case RGB24:  miftyp = 11; break;
            case SYS8A:  miftyp = 0; break;
            case SYS8B:  miftyp = 1; break;
            case SYS8C:  miftyp = 2; break;
            case SYS8D:  miftyp = 3; break;
            case SYS9:   miftyp = 4; break;
            case SYS12:  miftyp = 5; break;
            case SYS16A: miftyp = 7; break;
            case SYS16B: miftyp = 8; break;
            case SYS16C: miftyp = 9; break;
            case SYS18:  miftyp = 10; break;
            case SYS24:  miftyp = 11; break;
            default:
                printf("Not supported display\n");
                goto error;
	}

        tmp = miftyp;
        tmp |= (display->mode.sync & FB_SYNC_VERT_HIGH_ACT) ? 0 : 1 << 28;
        tmp |= (display->mode.sync & FB_SYNC_HOR_HIGH_ACT) ? 0 : 1 << 27;
        tmp |= (display->mode.sync & FB_SYNC_LATCH_DATA_ON_RISING_DOT_CLK) ? 1 << 26 : 0;
	iowrite32(tmp, MLDMT1R);
        iowrite32(display->board.ldmt2r, MLDMT2R);

        iowrite32(display->mode.xres * (display->bpp/8), MLDMLSR);

        /* horizontal configuration */
        tmp = display->mode.xres + display->mode.hsync_len;
        tmp += display->mode.left_margin;
        tmp += display->mode.right_margin;
        tmp /= 8; /* HTCN */
        tmp |= (display->mode.xres / 8) << 16; /* HDCN */
        iowrite32(tmp, MLDHCNR);
        tmp = display->mode.xres;
        tmp += display->mode.right_margin;
        tmp /= 8; /* HSYNP */
        tmp |= (display->mode.hsync_len / 8) << 16; /* HSYNW */
        iowrite32(tmp, MLDHSYNR);

        /* vertical configuration */
        tmp = display->mode.yres + display->mode.vsync_len;
        tmp += display->mode.upper_margin;
        tmp += display->mode.lower_margin; /* VTLN */
        tmp |= display->mode.yres << 16; /* VDLN */
        iowrite32(tmp, MLDVLNR);

        tmp = display->mode.yres;
        tmp += display->mode.lower_margin; /* VSYNP */
        tmp |= display->mode.vsync_len << 16; /* VSYNW */
        iowrite32(tmp, MLDVSYNR);

	
	u8  data = ioread8(LCDDISP_PORTD) & ~(1<<LCDDISP_PIN);
	u16 ctrl = ioread16(LCDDISP_PORTC) & ~(0x3<<(LCDDISP_PIN*2));
	switch ( display->lcddisp )
	{
		// continuous high level
		case LCDDISP_HIGH:
		{
			data |= 1<<LCDDISP_PIN;
			iowrite8(data, LCDDISP_PORTD);
			ctrl |= PIN_MODE_OUT<<(LCDDISP_PIN*2);
			iowrite16(ctrl, LCDDISP_PORTC);
			break;
		}
		// continuous low level
		case LCDDISP_LOW:
		{
			iowrite8(data, LCDDISP_PORTD);
			ctrl |= PIN_MODE_OUT<<(LCDDISP_PIN*2);
			iowrite16(ctrl, LCDDISP_PORTC);
			break;
		}
		// controlled by lcdc active high
		case LCDDISP_AUTO_HIGH:
		{
			tmp = ioread32(MLDMT1R);
			tmp &= ~(1 << 25);
			iowrite32(tmp, MLDMT1R);
			break;
		}
		// controlled by lcdc active low
		case LCDDISP_AUTO_LOW:
		{
			tmp = ioread32(MLDMT1R);
			tmp |= 1 << 25;
			iowrite32(tmp, MLDMT1R);
			break;
		}
		// controlled by lcdc somehow or leaving default value (here active high)
		case LCDDISP_AUTO:
		{
			break;
		}
		default:
		{
			break;
		}
	}

        return 0;

error:
        if (fbbuf_unaligned != NULL) {
                free(fbbuf_unaligned);
                display->fbbuf = NULL;
        }

        return -1;
}

void display_board_flush(display_info_t* display)
{
        /* nothing to do. area is not cached */
}


void display_board_enable(display_info_t* display)
{
        iowrite32(ADDR_TO_PHY(display->fbbuf), MLDSA1R); /* physical address */
        iowrite32(LDCNT1R_DE, LDCNT1R);
        iowrite32(LDCNT2R_ME | LDCNT2R_DO, LDCNT2R);

        board_backlight_enable(display);
}
