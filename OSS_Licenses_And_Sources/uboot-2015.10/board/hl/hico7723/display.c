/*
 * Copyright (C) 2013i-2015 emtrion GmbH.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 */

#include <common.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <linux/fb.h>
#include <stdlib.h>
#include "vscanf.h"
#include <image.h>
#include <jffs2/load_kernel.h>
#include <linux/mtd/mtd.h>

#include <i2c.h>

#include "display.h"

DECLARE_GLOBAL_DATA_PTR;

display_info_t *display_glob;
extern int warmboot;

#if defined(CONFIG_DISPLAY_SH4A)


static struct display_info_t conf_display = {
        .bus    = 2,
        .addr   = 0x3A,
        .pixfmt = 1,
        .adapterid = 0x10,
        .adapteridmask = 0xFF,
	.bpp = 16,
	.interface_type = RGB18,
	.lcddisp = LCDDISP_AUTO,
	.mode   = {
		      .name           = "CONF",
		      .refresh        = 50,
		      .xres           = 800,
		      .yres           = 480,
		      .pixclock       = 33000,
		      .left_margin    = 40,
		      .right_margin   = 40,
		      .upper_margin   = 29,
		      .lower_margin   = 13,
		      .hsync_len      = 48,
		      .vsync_len      = 3,
		      .sync           = 0,
		      .vmode          = FB_VMODE_NONINTERLACED
		  }
};


static struct display_info_t displays[] = {{
        .bus    = 2,
        .addr   = 0x3A,
        .pixfmt = 1,
        .adapterid = 0x10,
        .adapteridmask = 0xFF,
	.bpp = 16,
	.interface_type = RGB18,
	.lcddisp = LCDDISP_AUTO,
	.mode   = {
		      .name           = "UMSH",
		      .refresh        = 50,
		      .xres           = 800,
		      .yres           = 480,
		      .pixclock       = 38461,
		      .left_margin    = 40,
		      .right_margin   = 40,
		      .upper_margin   = 29,
		      .lower_margin   = 13,
		      .hsync_len      = 48,
		      .vsync_len      = 3,
		      .sync           = 0,
		      .vmode          = FB_VMODE_NONINTERLACED
		  }
	}, 
};



static int display_read_bootlogo_partition(display_info_t *display)
{
    u8 pnum;
    struct mtd_device *dev;
    struct part_info *part;
    image_header_t tmp_header;
    unsigned char *splash_zip;
    unsigned long lenp;
    unsigned char *logo_base;

    mtdparts_init();
    if (find_dev_and_part("bootlogo", &dev, &pnum, &part) == 0) {
	printf("partition found: device = %s%d, partition %d = (%s) 0x%08llx@0x%08llx\n",
			MTD_DEV_TYPE(dev->id->type), dev->id->num, pnum,
			part->name, part->size, part->offset);
/* read header information of image */

	logo_base = (unsigned char *)HICO7723_FLASH_BASE_1;
	memcpy(&tmp_header, (unsigned char *)(logo_base + part->offset), sizeof(tmp_header));
	
	/* plausibility check if there is a valid uzb file */
	if ( (ntohl(tmp_header.ih_size) + sizeof(tmp_header)) < display->fbsize ) {

/* prepare some memory and copy image to this memory space */
	    splash_zip = (unsigned char *)malloc(ntohl(tmp_header.ih_size) + sizeof(tmp_header));
	    memcpy(splash_zip, (unsigned char *)(logo_base + part->offset), ntohl(tmp_header.ih_size) + sizeof(tmp_header));
	    
	    lenp = ntohl(tmp_header.ih_size);
	    gunzip(display->fbbuf, display->fbsize, splash_zip + sizeof(tmp_header), &lenp);
	    return 1;
	}
	else {
	    printf("No bootlogo present\n");
	    return 0;
	}
    }
    else {
	printf("No bootlogo present\n");
	return 0;
    }
}

static int display_read_bootlogo_wdt_partition(display_info_t *display)
{
    u8 pnum;
    struct mtd_device *dev;
    struct part_info *part;
    image_header_t tmp_header;
    unsigned char *splash_zip;
    unsigned long lenp;
    unsigned char *logo_base;

    mtdparts_init();
    if (find_dev_and_part("bootlogo", &dev, &pnum, &part) == 0) {
	printf("partition found: device = %s%d, partition %d = (%s) 0x%08llx@0x%08llx\n",
			MTD_DEV_TYPE(dev->id->type), dev->id->num, pnum,
			part->name, part->size, part->offset);
/* read header information of image */

	logo_base = (unsigned char *)HICO7723_FLASH_BASE_1 + 0x20000; /* adding offset of 128kBytes for wdt bootlogo */
	memcpy(&tmp_header, (unsigned char *)(logo_base + part->offset), sizeof(tmp_header));
	
	/* plausibility check if there is a valid uzb file */
	if ( (ntohl(tmp_header.ih_size) + sizeof(tmp_header)) < display->fbsize ) {

/* prepare some memory and copy image to this memory space */
	    splash_zip = (unsigned char *)malloc(ntohl(tmp_header.ih_size) + sizeof(tmp_header));
	    memcpy(splash_zip, (unsigned char *)(logo_base + part->offset), ntohl(tmp_header.ih_size) + sizeof(tmp_header));
	    
	    lenp = ntohl(tmp_header.ih_size);
	    gunzip(display->fbbuf, display->fbsize, splash_zip + sizeof(tmp_header), &lenp);
	    return 1;
	}
	else {
	    printf("No wdt bootlogo present\n");
	    return 0;
	}
    }
    else {
	printf("No wdt bootlogo present\n");
	return 0;
    }
}

#define DISPLAY_PARA_ANZ 12

void setup_display(void)
{
    char *display_conf;
    char *display_conf_tmp;
    unsigned long params[12];
    struct fb_videomode *fb;
    char *tok, *end;
    int anz_param = 0;
 
    display_info_t *display;
    
    if ( (display_conf = getenv("display_configuration")) == NULL )
    {
      printf("display_configuration not found, using default values now\n");
      display = &displays[0];
    }
    else {
      display_conf_tmp = (char *)malloc(strlen(display_conf) + 1); /* string length plus zero */
      memcpy(display_conf_tmp, display_conf, strlen(display_conf));
      display_conf_tmp[strlen(display_conf)] = 0x00;
//      printf("Display configuration found: %s\n", display_conf_tmp);
      
      fb = (struct fb_videomode *)&(conf_display.mode);
      
      tok = display_conf_tmp;
      end = tok;
      while ( tok != NULL ) {
	strsep(&end, ",");
	if ( (anz_param < DISPLAY_PARA_ANZ) ) {
	    sscanf(tok, "%d", &params[anz_param]);
	}
	else {
	  anz_param = 0; /* parameter list to big, signalling with setting the count val to a unexpected value */
	  break;
	}
	anz_param++;
	tok = end;
      }
/* when the number of the parameters is ok, the default settings for the display are overwritten now */
      if ( (anz_param == DISPLAY_PARA_ANZ) && (params[0] != 0) && (params[1] != 0) ) {
	fb->name         = "Conf";
	fb->xres         = params[0];
	fb->yres         = params[1];
	fb->left_margin  = params[3];
	fb->right_margin = params[4];
	fb->hsync_len    = params[5];
	fb->upper_margin = params[6];
	fb->lower_margin = params[7];
	fb->vsync_len    = params[8];
	fb->sync         = params[9];
	fb->pixclock     = params[10]; /* not used since this is hardcoded to 26MHz by the hardware */
	                               /* Note: this value conforms to Linux, psec and not Hz!!!    */
	conf_display.lcddisp = params[11];
	display          = &conf_display;
      } 
      else {
	display = &displays[0];
      }
    }
/*      printf("Display configuration found: %s\n", display_conf);*/
    display_board_init(display);
    display_glob = display;
    
    if ( warmboot ) {
    // display the contents
	if (!display_read_bootlogo_wdt_partition(display))
    // don't display random data
	    memset(display->fbbuf, 0, display->fbsize);
    }
    else {
    // display the contents
	if (!display_read_bootlogo_partition(display))
    // don't display random data
	    memset(display->fbbuf, 0, display->fbsize);
    }
	
    display_board_enable(display);
}
#endif
