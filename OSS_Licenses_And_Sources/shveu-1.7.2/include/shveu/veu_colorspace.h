/*
 * libshveu: A library for controlling SH-Mobile VEU
 * Copyright (C) 2009 Renesas Technology Corp.
 * Copyright (C) 2010 Renesas Electronics Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* Common information for Renesas video buffers */
#ifndef __REN_VIDEO_BUFFER_H__
#define __REN_VIDEO_BUFFER_H__

/* Notes on YUV/YCbCr:
 * YUV historically refers to analogue color space, and YCbCr to digital.
 * The formula used to convert to/from RGB is BT.601 or BT.709. HDTV specifies
 * BT.709, everything else BT.601.
 * MPEG standards use 'clamped' data with Y[16,235], CbCr[16,240]. JFIF file
 * format for JPEG specifies full-range data.
 * All YCbCr formats here are BT.601, Y[16,235], CbCr[16,240].
 */

/** Surface formats */
typedef enum {
	REN_UNKNOWN,
	REN_NV12,    /**< YCbCr420: Y plane, packed CbCr plane, optional alpha plane */
	REN_NV16,    /**< YCbCr422: Y plane, packed CbCr plane, optional alpha plane */
	REN_RGB565,  /**< Packed RGB565 */
	REN_RGB24,   /**< Packed RGB888 */
	REN_BGR24,   /**< Packed BGR888 */
	REN_RGB32,   /**< Packed XRGB8888 (most significant byte ignored) */
	REN_ARGB32,  /**< Packed ARGB8888 */
} ren_vid_format_t;


/** Bounding rectange */
struct ren_vid_rect {
	int x;      /**< Offset from left in pixels */
	int y;      /**< Offset from top in pixels */
	int w;      /**< Width of rectange in pixels */
	int h;      /**< Height of rectange in pixels */
};

/** Surface */
struct ren_vid_surface {
	ren_vid_format_t format; /**< Surface format */
	int w;      /**< Width of active surface in pixels */
	int h;      /**< Height of active surface in pixels */
	int pitch;  /**< Width of surface in pixels */
	void *py;   /**< Address of Y or RGB plane */
	void *pc;   /**< Address of CbCr plane (ignored for RGB) */
	void *pa;   /**< Address of Alpha plane (ignored) */
};

struct format_info {
	ren_vid_format_t fmt;    /**< surface format */
	int y_bpp;      /**< Luma numerator */
	int c_bpp;      /**< Chroma numerator */
	int c_bpp_n;    /**< Chroma numerator */
	int c_bpp_d;    /**< Chroma denominator */
	int c_ss_horz;  /**< Chroma horizontal sub-sampling */
	int c_ss_vert;  /**< Chroma vertical sub-sampling */
};

static const struct format_info fmts[] = {
	{ REN_UNKNOWN, 0, 0, 0, 1, 1, 1 },
	{ REN_NV12,    1, 2, 1, 2, 2, 2 },
	{ REN_NV16,    1, 2, 1, 1, 2, 1 },
	{ REN_RGB565,  2, 0, 0, 1, 1, 1 },
	{ REN_RGB24,   3, 0, 0, 1, 1, 1 },
	{ REN_BGR24,   3, 0, 0, 1, 1, 1 },
	{ REN_RGB32,   4, 0, 0, 1, 1, 1 },
	{ REN_ARGB32,  4, 0, 0, 1, 1, 1 },
};

static inline int is_ycbcr(ren_vid_format_t fmt)
{
	if (fmt >= REN_NV12 && fmt <= REN_NV16)
		return 1;
	return 0;
}

static inline int is_rgb(ren_vid_format_t fmt)
{
	if (fmt >= REN_RGB565 && fmt <= REN_ARGB32)
		return 1;
	return 0;
}

static inline int different_colorspace(ren_vid_format_t fmt1, ren_vid_format_t fmt2)
{
	if ((is_rgb(fmt1) && is_ycbcr(fmt2))
	    || (is_ycbcr(fmt1) && is_rgb(fmt2)))
		return 1;
	return 0;
}

static inline size_t size_y(ren_vid_format_t format, int nr_pixels)
{
	const struct format_info *fmt = &fmts[format];
	return (fmt->y_bpp * nr_pixels);
}

static inline size_t size_c(ren_vid_format_t format, int nr_pixels)
{
	const struct format_info *fmt = &fmts[format];
	return (fmt->c_bpp_n * nr_pixels) / fmt->c_bpp_d;
}

static inline size_t size_a(ren_vid_format_t format, int nr_pixels)
{
	/* Assume 1 byte per alpha pixel */
	return nr_pixels;
}

static inline size_t offset_y(ren_vid_format_t format, int w, int h, int pitch)
{
	const struct format_info *fmt = &fmts[format];
	return (fmt->y_bpp * ((h * pitch) + w));
}

static inline size_t offset_c(ren_vid_format_t format, int w, int h, int pitch)
{
	const struct format_info *fmt = &fmts[format];
	return (fmt->c_bpp * (((h/fmt->c_ss_vert) * pitch/fmt->c_ss_horz) + w/fmt->c_ss_horz));
}

static inline size_t offset_a(ren_vid_format_t format, int w, int h, int pitch)
{
	/* Assume 1 byte per alpha pixel */
	return ((h * pitch) + w);
}

static int horz_increment(ren_vid_format_t format)
{
	/* Only restriction is caused by chroma sub-sampling */
	return fmts[format].c_ss_horz;
}

static int vert_increment(ren_vid_format_t format)
{
	/* Only restriction is caused by chroma sub-sampling */
	return fmts[format].c_ss_vert;
}

/* Get a new surface descriptor based on a selection */
static inline void get_sel_surface(
	struct ren_vid_surface *out,
	const struct ren_vid_surface *in,
	const struct ren_vid_rect *sel)
{
	int x = sel->x & ~horz_increment(in->format);
	int y = sel->y & ~vert_increment(in->format);

	*out = *in;
	out->w = sel->w & ~horz_increment(in->format);
	out->h = sel->h & ~vert_increment(in->format);

	if (in->py) out->py += offset_y(in->format, x, y, in->pitch);
	if (in->pc) out->pc += offset_c(in->format, x, y, in->pitch);
	if (in->pa) out->pa += offset_a(in->format, x, y, in->pitch);
}

#endif /* __REN_VIDEO_BUFFER_H__ */


/** \file
 * Image/Video processing: Scale, rotate, crop, color conversion
 */

#ifndef __VEU_COLORSPACE_H__
#define __VEU_COLORSPACE_H__


/** Rotation */
typedef enum {
	SHVEU_NO_ROT=0,	/**< No rotation */
	SHVEU_ROT_90,	/**< Rotate 90 degrees clockwise */
} shveu_rotation_t;

/** Setup a (scale|rotate) & crop between YCbCr & RGB surfaces
 * The scaling factor is calculated from the surface sizes.
 *
 * \param veu VEU handle
 * \param src_surface Input surface
 * \param dst_surface Output surface
 * \param rotate Rotation to apply
 * \retval 0 Success
 * \retval -1 Error: Attempt to perform simultaneous scaling and rotation
 */
int
shveu_setup(
	SHVEU *veu,
	const struct ren_vid_surface *src_surface,
	const struct ren_vid_surface *dst_surface,
	shveu_rotation_t rotate);


/** Set the source addresses. This is typically used for bundle mode.
 * \param veu VEU handle
 * \param src_py Address of Y or RGB plane of source image
 * \param src_pc Address of CbCr plane of source image (ignored for RGB)
 */
void
shveu_set_src(
	SHVEU *veu,
	void *src_py,
	void *src_pc);

/** Set the destination addresses. This is typically used for bundle mode.
 * \param veu VEU handle
 * \param dst_py Address of Y or RGB plane of destination image
 * \param dst_pc Address of CbCr plane of destination image (ignored for RGB)
 */
void
shveu_set_dst(
	SHVEU *veu,
	void *dst_py,
	void *dst_pc);

/** Set the source addresses. This is typically used for bundle mode.
 * \param veu VEU handle
 * \param src_py Address of Y or RGB plane of source image
 * \param src_pc Address of CbCr plane of source image (ignored for RGB)
 */
void
shveu_set_src_phys(
	SHVEU *veu,
	unsigned int src_py,
	unsigned int src_pc);

/** Set the destination addresses. This is typically used for bundle mode.
 * \param veu VEU handle
 * \param dst_py Address of Y or RGB plane of destination image
 * \param dst_pc Address of CbCr plane of destination image (ignored for RGB)
 */
void
shveu_set_dst_phys(
	SHVEU *veu,
	unsigned int dst_py,
	unsigned int dst_pc);

/** Set the colour space conversion attributes.
 * \param veu VEU handle
 * \param bt709 If true use ITU-R BT709, otherwise use ITU-R BT.601 (default)
 * \param full_range If true use YCbCr[0,255], otherwise use Y[16,235], CbCr[16,240] (default)
 */
void
shveu_set_color_conversion(
	SHVEU *veu,
	int bt709,
	int full_range);

/** Start a VEU operation (non-bundle mode).
 * \param veu VEU handle
 */
void
shveu_start(
	SHVEU *veu);

/** Start a VEU operation (bundle mode).
 * \param veu VEU handle
 * \param bundle_lines Number of lines to process
 */
void
shveu_start_bundle(
	SHVEU *veu,
	int bundle_lines);

/** Wait for a VEU operation to complete. The operation is started by a call to shveu_start.
 * \param veu VEU handle
 */
int
shveu_wait(SHVEU *veu);


/** Perform scale between YCbCr & RGB surfaces.
 * This operates on entire surfaces and blocks until completion.
 *
 * \param veu VEU handle
 * \param src_surface Input surface
 * \param dst_surface Output surface
 * \retval 0 Success
 * \retval -1 Error: Unsupported parameters
 */
int
shveu_resize(
	SHVEU *veu,
	const struct ren_vid_surface *src_surface,
	const struct ren_vid_surface *dst_surface);

/** Perform rotate between YCbCr & RGB surfaces
 * This operates on entire surfaces and blocks until completion.
 *
 * \param veu VEU handle
 * \param src_surface Input surface
 * \param dst_surface Output surface
 * \param rotate Rotation to apply
 * \retval 0 Success
 * \retval -1 Error: Unsupported parameters
 */
int
shveu_rotate(
	SHVEU *veu,
	const struct ren_vid_surface *src_surface,
	const struct ren_vid_surface *dst_surface,
	shveu_rotation_t rotate);

#endif				/* __VEU_COLORSPACE_H__ */
