// SPDX-License-Identifier: GPL-2.0+
/*
 * Simplefb device tree support
 *
 * (C) Copyright 2015
 * Stephen Warren <swarren@wwwdotorg.org>
 */

#include <common.h>
#include <dm.h>
#include <lcd.h>
#include <fdt_support.h>
#include <linux/libfdt.h>
#include <video.h>

DECLARE_GLOBAL_DATA_PTR;

uint32_t handle=0;

static int lcd_dt_simplefb_configure_node(void *blob, int off)
{
	int xsize, ysize;
	int bpix; /* log2 of bits per pixel */
	const char *name;
	ulong fb_base;
#ifdef CONFIG_DM_VIDEO
	struct video_uc_platdata *plat;
	struct video_priv *uc_priv;
	struct udevice *dev;
	int ret;

	ret = uclass_first_device_err(UCLASS_VIDEO, &dev);
	//printf("%s:%d ret = %d\n", __func__, __LINE__, ret);
	if (ret < 0)
		return ret;
	uc_priv = dev_get_uclass_priv(dev);
	plat = dev_get_uclass_platdata(dev);
	xsize = uc_priv->xsize;
	ysize = uc_priv->ysize;
	bpix = uc_priv->bpix;
	fb_base = plat->base;
	//printf("%s:%d xsize = %d, ysize = %d, bpix = %d, fb_base = %x\n", __func__, __LINE__, xsize, ysize, bpix, fb_base);
#else
	xsize = lcd_get_pixel_width();
	ysize = lcd_get_pixel_height();
	bpix = LCD_BPP;
	fb_base = gd->fb_base;
#endif
	switch (bpix) {
	case 4: /* VIDEO_BPP16 */
		name = "r5g6b5";
		break;
	case 5: /* VIDEO_BPP32 */
		name = "a8r8g8b8";
		break;
	default:
		return -EINVAL;
	}

	return fdt_setup_simplefb_node(blob, off, fb_base, xsize, ysize,
				       xsize * (1 << bpix) / 8, name);
}

int lcd_dt_simplefb_add_node(void *blob)
{
	static const char compat[] = "simple-framebuffer";
	static const char okay[] = "okay";
	int off, ret;

	off = fdt_add_subnode(blob, 0, "framebuffer");
	if (off < 0)
		return -1;

	ret = fdt_setprop(blob, off, "status", okay, sizeof(okay));
	if (ret < 0)
		return -1;

	ret = fdt_setprop(blob, off, "compatible", compat, sizeof(compat));
	if (ret < 0)
		return -1;

	ret = fdt_setprop_cell(blob, off, "memory-region", handle);
	if (ret < 0)
		return -1;

	return lcd_dt_simplefb_configure_node(blob, off);
}

int lcd_dt_simplefb_enable_existing_node(void *blob)
{
	int off;

	off = fdt_node_offset_by_compatible(blob, -1, "simple-framebuffer");
	if (off < 0)
		return -1;

	int ret = fdt_setprop_cell(blob, off, "memory-region", handle);
	if (ret < 0)
		return -1;

	return lcd_dt_simplefb_configure_node(blob, off);
}

//#if CONFIG_IS_ENABLED(DM_VIDEO)
int lcd_dt_simplefb_enable_and_mem_rsv(void *blob)
{
	struct fdt_memory mem;
	int ret;
	//printf("%s, Try to do some device tree magic at address %x\n",__func__, blob);
	/* nothing to do when video is not active */
	if (!video_is_active())
	{
		//printf("%s, video is not active!\n",__func__);
		return 0;
	}

	/* nothing to do when the frame buffer is not defined */
	if (gd->video_bottom == gd->video_top)
	{
		//printf("%s, Frame Buffer is not defined, video_bottom = %d, video_to = %d\n",__func__, gd->video_bottom, gd->video_top);
		return 0;
	}

	/* reserved with no-map tag the video buffer */
	mem.start = gd->video_bottom;
	mem.end = gd->video_top - 1;

	ret = fdtdec_add_reserved_memory(blob, "framebuffer", &mem, &handle, true);
	if(ret)
	{
		//printf("%s, Add reserved menory failed\n", __func__);
	}

	ret = lcd_dt_simplefb_enable_existing_node(blob);
	if (ret)
	{
		//printf("%s, enable existing node failed\n", __func__);
		return ret;
	}

	return ret;
}
//#endif
