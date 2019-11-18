/*
 * extbsp/display.h
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
 * Description: Display initialization
 *
 **/

#ifndef _EXTBSP_DISPLAY_H
#define _EXTBSP_DISPLAY_H

#include <linux/fb.h>


// linx/include/video/sh_mobile_lcdc.h
enum { RGB8,   /* 24bpp, 8:8:8 */
       RGB9,   /* 18bpp, 9:9 */
       RGB12A, /* 24bpp, 12:12 */
       RGB12B, /* 12bpp */
       RGB16,  /* 16bpp */
       RGB18,  /* 18bpp */
       RGB24,  /* 24bpp */
       SYS8A,  /* 24bpp, 8:8:8 */
       SYS8B,  /* 18bpp, 8:8:2 */
       SYS8C,  /* 18bpp, 2:8:8 */
       SYS8D,  /* 16bpp, 8:8 */
       SYS9,   /* 18bpp, 9:9 */
       SYS12,  /* 24bpp, 12:12 */
       SYS16A, /* 16bpp */
       SYS16B, /* 18bpp, 16:2 */
       SYS16C, /* 18bpp, 2:16 */
       SYS18,  /* 18bpp */
       SYS24 };/* 24bpp */

// linux/include/linux/fb.h

#define FB_SYNC_HOR_HIGH_ACT	1	/* horizontal sync high active	*/
#define FB_SYNC_VERT_HIGH_ACT	2	/* vertical sync high active	*/
#define FB_SYNC_LATCH_DATA_ON_RISING_DOT_CLK	128

typedef enum {
	LCDDISP_AUTO = 0,
	LCDDISP_LOW  = 1,
	LCDDISP_HIGH = 2,
 	LCDDISP_AUTO_HIGH = 3,
  	LCDDISP_AUTO_LOW  = 4,
} lcddisp_e;

/* in convention with U-Boot returns -1 on failure and 0 on ok */

typedef struct display {
        // are defined by DISPLAY
        char name[32];

        u32    width;
        u32    height;
        u32    bpp;
        int    i2c_id;     // -1 if not available
        int    phys_width; // in [mm]
        int    phys_height; // in [mm]
        void   *priv;

        // are determined at runtime
        size_t fbsize;
        void   *fbbuf;

        int interface_type;
        int flags;
        u32 xres;
        u32 yres;
        u32 left_margin;
        u32 right_margin;
        u32 hsync_len;
        u32 upper_margin;
        u32 lower_margin;
        u32 vsync_len;
        u32 sync;

        u32 pixel_clock_min;
        u32 pixel_clock_typ;
        u32 pixel_clock_max;

        u32 vsync_min;
        u32 vsync_typ;
        u32 vsync_max;

	lcddisp_e lcddisp;

	int rotation;		/* in degree */

	int backlight_polarity;	/* 0: backlight is on with a low signal
				   1: backlight is on with a high signal */

        struct {
                u32 ldmt2r;
                u32 ckr;
                u32 ckpat1r;
                u32 ckpat2r;
        } board;
} display_t;



typedef struct display_info_t {
	int	bus;
	int	addr;
	int	pixfmt;
        u32    bpp;
        int     adapterid;
        int     adapteridmask;
	int	(*detect)(struct display_info_t const *dev);
	void	(*enable)(struct display_info_t const *dev);
	struct	fb_videomode mode;

	// are determined at runtime
        size_t fbsize;
        void   *fbbuf;
        int interface_type;
	lcddisp_e lcddisp;

	struct {
                u32 ldmt2r;
                u32 ckr;
                u32 ckpat1r;
                u32 ckpat2r;
        } board;
	
} display_info_t; 















extern int display_init(void);
extern const char* display_name(void);
extern int display_board_init(display_info_t *display);
extern void display_board_flush(display_info_t* display);
extern void display_board_enable(display_info_t* display);
extern void board_backlight_enable(display_info_t* display);




#endif /* _EXTBSP_DISPLAY_H */
