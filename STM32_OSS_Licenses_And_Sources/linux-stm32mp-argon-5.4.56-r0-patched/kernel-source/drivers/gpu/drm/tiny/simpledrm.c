// SPDX-License-Identifier: GPL-2.0-only

#include <linux/platform_data/simplefb.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_clk.h>
#include <linux/clk.h>
#include <drm/drm_aperture.h>
#include <drm/drm_atomic.h>
#include <drm/drm_atomic_state_helper.h>
#include <drm/drm_connector.h>
#include <drm/drm_damage_helper.h>
#include <drm/drm_device.h>
#include <drm/drm_drv.h>
#include <drm/drm_format_helper.h>
#include <drm/drm_fb_helper.h>
//#include <drm/drm_gem_atomic_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_gem_shmem_helper.h>
//#include <drm/drm_managed.h>
#include <drm/drm_modeset_helper_vtables.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_simple_kms_helper.h>
#include <../drm_crtc_internal.h>

#define DRIVER_NAME	"simpledrm"
#define DRIVER_DESC	"DRM driver for simple-framebuffer platform devices"
#define DRIVER_DATE	"20200625"
#define DRIVER_MAJOR	1
#define DRIVER_MINOR	0

/*
 * Assume a monitor resolution of 96 dpi to
 * get a somewhat reasonable screen size.
 */
#define RES_MM(d)	\
	(((d) * 254ul) / (96ul * 10ul))

#define SIMPLEDRM_MODE(hd, vd)	\
	DRM_SIMPLE_MODE(hd, vd, RES_MM(hd), RES_MM(vd))

/**
 * DRM_FORMAT_MAX_PLANES - maximum number of planes a DRM format can have
 */
#define DRM_FORMAT_MAX_PLANES	4u

extern int drm_mode_create_standard_properties(struct drm_device *dev);

/**
 * drm_fb_blit_rect_dstclip - Copy parts of a framebuffer to display memory
 * @dst:	The display memory to copy to
 * @dst_pitch:	Number of bytes between two consecutive scanlines within dst
 * @dst_format:	FOURCC code of the display's color format
 * @vmap:	The framebuffer memory to copy from
 * @fb:		The framebuffer to copy from
 * @clip:	Clip rectangle area to copy
 *
 * This function copies parts of a framebuffer to display memory. If the
 * formats of the display and the framebuffer mismatch, the blit function
 * will attempt to convert between them.
 *
 * Use drm_fb_blit_dstclip() to copy the full framebuffer.
 *
 * Returns:
 * 0 on success, or
 * -EINVAL if the color-format conversion failed, or
 * a negative error code otherwise.
 */
int drm_fb_blit_rect_dstclip(void __iomem *dst, unsigned int dst_pitch,
			     uint32_t dst_format, void *vmap,
			     struct drm_framebuffer *fb,
			     struct drm_rect *clip)
{
	uint32_t fb_format = fb->format->format;
	/* treat alpha channel like filler bits */
	if (fb_format == DRM_FORMAT_ARGB8888)
		fb_format = DRM_FORMAT_XRGB8888;
	if (dst_format == DRM_FORMAT_ARGB8888)
		dst_format = DRM_FORMAT_XRGB8888;
	if (dst_format == fb_format) {
		drm_fb_memcpy_dstclip(dst, vmap, fb, clip);
		return 0;

	} else if (dst_format == DRM_FORMAT_RGB565) {
		if (fb_format == DRM_FORMAT_XRGB8888) {
			drm_fb_xrgb8888_to_rgb565_dstclip(dst, dst_pitch,
							  vmap, fb, clip,
							  false);
			return 0;
		}
	} else if (dst_format == DRM_FORMAT_RGB888) {
		if (fb_format == DRM_FORMAT_XRGB8888) {
			drm_fb_xrgb8888_to_rgb888_dstclip(dst, dst_pitch,
							  vmap, fb, clip);
			return 0;
		}
	}

	return -EINVAL;
}
EXPORT_SYMBOL(drm_fb_blit_rect_dstclip);

/**
 * drm_fb_blit_dstclip - Copy framebuffer to display memory
 * @dst:	The display memory to copy to
 * @dst_pitch:	Number of bytes between two consecutive scanlines within dst
 * @dst_format:	FOURCC code of the display's color format
 * @vmap:	The framebuffer memory to copy from
 * @fb:		The framebuffer to copy from
 *
 * This function copies a full framebuffer to display memory. If the formats
 * of the display and the framebuffer mismatch, the copy function will
 * attempt to convert between them.
 *
 * See drm_fb_blit_rect_dstclip() for more information.
 *
 * Returns:
 * 0 on success, or a negative error code otherwise.
 */
int drm_fb_blit_dstclip(void __iomem *dst, unsigned int dst_pitch,
			uint32_t dst_format, void *vmap,
			struct drm_framebuffer *fb)
{
	struct drm_rect fullscreen = {
		.x1 = 0,
		.x2 = fb->width,
		.y1 = 0,
		.y2 = fb->height,
	};
	return drm_fb_blit_rect_dstclip(dst, dst_pitch, dst_format, vmap, fb,
					&fullscreen);
}
EXPORT_SYMBOL(drm_fb_blit_dstclip);

/**
 * struct drm_format_conv_state - Stores format-conversion state
 *
 * DRM helpers for format conversion store temporary state in
 * struct drm_xfrm_buf. The buffer's resources can be reused
 * among multiple conversion operations.
 *
 * All fields are considered private.
 */
struct drm_format_conv_state {
	struct {
		void *mem;
		size_t size;
		bool preallocated;
	} tmp;
};

/**
 * struct iosys_map - Pointer to IO/system memory
 * @vaddr_iomem:	The buffer's address if in I/O memory
 * @vaddr:		The buffer's address if in system memory
 * @is_iomem:		True if the buffer is located in I/O memory, or false
 *			otherwise.
 */
struct iosys_map {
	union {
		void __iomem *vaddr_iomem;
		void *vaddr;
	};
	bool is_iomem;
};

/**
 * struct drm_shadow_plane_state - plane state for planes with shadow buffers
 *
 * For planes that use a shadow buffer, struct drm_shadow_plane_state
 * provides the regular plane state plus mappings of the shadow buffer
 * into kernel address space.
 */
struct drm_shadow_plane_state {
	/** @base: plane state */
	struct drm_plane_state base;

	/**
	 * @fmtcnv_state: Format-conversion state
	 *
	 * Per-plane state for format conversion.
	 * Flags for copying shadow buffers into backend storage. Also holds
	 * temporary storage for format conversion.
	 */
	struct drm_format_conv_state fmtcnv_state;

	/* Transitional state - do not export or duplicate */

	/**
	 * @map: Mappings of the plane's framebuffer BOs in to kernel address space
	 *
	 * The memory mappings stored in map should be established in the plane's
	 * prepare_fb callback and removed in the cleanup_fb callback.
	 */
	struct iosys_map map[DRM_FORMAT_MAX_PLANES];

	/**
	 * @data: Address of each framebuffer BO's data
	 *
	 * The address of the data stored in each mapping. This is different
	 * for framebuffers with non-zero offset fields.
	 */
	struct iosys_map data[DRM_FORMAT_MAX_PLANES];
};

/**
 * to_drm_shadow_plane_state - upcasts from struct drm_plane_state
 * @state: the plane state
 */
static inline struct drm_shadow_plane_state *
to_drm_shadow_plane_state(struct drm_plane_state *state)
{
	return container_of(state, struct drm_shadow_plane_state, base);
}

/*
 * Simple Framebuffer device
 */

struct simpledrm_device {
	struct drm_device dev;
	struct platform_device *pdev;

	unsigned int clk_count;
	struct clk **clks;

	/* simplefb settings */
	struct drm_display_mode mode;
	const struct drm_format_info *format;
	unsigned int pitch;

	/* memory management */
	struct resource *mem;
	void __iomem *screen_base;

	/* modesetting */
	uint32_t formats[8];
	size_t nformats;
	struct drm_crtc crtc;
	struct drm_encoder encoder;
	struct drm_connector connector;
	struct drm_simple_display_pipe pipe;
};

///**
// * DRM_GEM_SIMPLE_DISPLAY_PIPE_SHADOW_PLANE_FUNCS -
// *	Initializes struct drm_simple_display_pipe_funcs for shadow-buffered planes
// *
// * Drivers may use GEM BOs as shadow buffers over the framebuffer memory. This
// * macro initializes struct drm_simple_display_pipe_funcs to use the rsp helper
// * functions.
// */
//#define DRM_GEM_SIMPLE_DISPLAY_PIPE_SHADOW_PLANE_FUNCS \
//	.prepare_fb = drm_gem_simple_kms_prepare_shadow_fb, \
//	.cleanup_fb = drm_gem_simple_kms_cleanup_shadow_fb, \
//	.reset_plane = drm_gem_simple_kms_reset_shadow_plane, \
//	.duplicate_plane_state = drm_gem_simple_kms_duplicate_shadow_plane_state, \
//	.destroy_plane_state = drm_gem_simple_kms_destroy_shadow_plane_state

/*
 * Helpers for simplefb
 */

static struct simpledrm_device *simpledrm_device_of_dev(struct drm_device *dev)
{
	return container_of(dev, struct simpledrm_device, dev);
}

static void simpledrm_device_release_clocks(void *res)
{
	struct simpledrm_device *sdev = simpledrm_device_of_dev(res);
	unsigned int i;
	for (i = 0; i < sdev->clk_count; ++i) {
		if (sdev->clks[i]) {
			clk_disable_unprepare(sdev->clks[i]);
			clk_put(sdev->clks[i]);
		}
	}
}

static int simpledrm_device_init_clocks(struct simpledrm_device *sdev)
{
	struct drm_device *dev;
	struct platform_device *pdev;
	struct device_node *of_node;
	struct clk *clock;
	unsigned int i;
	int ret;

	if (!sdev) {
		return -EINVAL;
	}

	dev = &sdev->dev;
	pdev = sdev->pdev;

	if (!pdev) {
		return -EINVAL;
	}

	of_node = pdev->dev.of_node;

	if (dev_get_platdata(&pdev->dev) || !of_node)
		return 0;
	
	sdev->clk_count = of_clk_get_parent_count(of_node);
	if (!sdev->clk_count)
		return 0;
	
	sdev->clks = devm_kzalloc(&pdev->dev, sdev->clk_count * sizeof(sdev->clks[0]),
				  GFP_KERNEL);
	if (!sdev->clks)
		return -ENOMEM;
	
	for (i = 0; i < sdev->clk_count; ++i) {
		clock = of_clk_get(of_node, i);
		if (IS_ERR(clock)) {
			ret = PTR_ERR(clock); 
			if (ret == -EPROBE_DEFER) {
				goto err;
			}
			drm_err("clock %u not found: %d\n", i, ret);
			continue;
		}
		ret = clk_prepare_enable(clock);
		if (ret) {
			drm_err("failed to enable clock %u: %d\n",
				i, ret);
			clk_put(clock);
			continue;
		}
		sdev->clks[i] = clock;
	}
	
	return devm_add_action_or_reset(&pdev->dev,
					simpledrm_device_release_clocks,
					sdev);

err:
	while (i) {
		--i;
		if (sdev->clks[i]) {
			clk_disable_unprepare(sdev->clks[i]);
			clk_put(sdev->clks[i]);
		}
	}
	return ret;
}

static int
simplefb_get_validated_int(struct drm_device *dev, const char *name,
			   uint32_t value)
{
	if (value > INT_MAX) {
		drm_err("simplefb: invalid framebuffer %s of %u\n",
			name, value);
		return -EINVAL;
	}
	return (int)value;
}

static int
simplefb_get_validated_int0(struct drm_device *dev, const char *name,
			    uint32_t value)
{
	if (!value) {
		drm_err("simplefb: invalid framebuffer %s of %u\n",
			name, value);
		return -EINVAL;
	}
	return simplefb_get_validated_int(dev, name, value);
}

static const struct drm_format_info *
simplefb_get_validated_format(struct drm_device *dev, const char *format_name)
{
	static const struct simplefb_format formats[] = SIMPLEFB_FORMATS;
	const struct simplefb_format *fmt = formats;
	const struct simplefb_format *end = fmt + ARRAY_SIZE(formats);
	if (!format_name) {
		drm_err("simplefb: missing framebuffer format\n");
		return ERR_PTR(-EINVAL);
	}

	while (fmt < end) {
		if (!strcmp(format_name, fmt->name))
			return drm_format_info(fmt->fourcc);
		++fmt;
	}

	drm_err("simplefb: unknown framebuffer format %s\n",
		format_name);

	return ERR_PTR(-EINVAL);
}

static int
simplefb_get_width_pd(struct drm_device *dev,
		      const struct simplefb_platform_data *pd)
{
	return simplefb_get_validated_int0(dev, "width", pd->width);
}

static int
simplefb_get_height_pd(struct drm_device *dev,
		       const struct simplefb_platform_data *pd)
{
	return simplefb_get_validated_int0(dev, "height", pd->height);
}

static int
simplefb_get_stride_pd(struct drm_device *dev,
		       const struct simplefb_platform_data *pd)
{
	return simplefb_get_validated_int(dev, "stride", pd->stride);
}

static const struct drm_format_info *
simplefb_get_format_pd(struct drm_device *dev,
		       const struct simplefb_platform_data *pd)
{
	return simplefb_get_validated_format(dev, pd->format);
}

/*
 *  Simplefb settings
 */

static struct drm_display_mode simpledrm_mode(unsigned int width,
					      unsigned int height)
{
	struct drm_display_mode mode = { SIMPLEDRM_MODE(width, height) };
	mode.clock = 60 /* Hz */ * mode.hdisplay * mode.vdisplay;
	drm_mode_set_name(&mode);

	return mode;
}

static int
simplefb_read_u32_of(struct drm_device *dev, struct device_node *of_node, const char *name, u32 *value)
{
	int ret = of_property_read_u32(of_node, name, value);
	if (ret)
		drm_err("simplefb: cannot parse framebuffer %s: error %d\n",
			name, ret);
	return ret;
}

static int
simplefb_read_string_of(struct drm_device *dev, struct device_node *of_node, const char *name, const char **value)
{
	int ret = of_property_read_string(of_node, name, value);
	if (ret)
		drm_err("simplefb: cannot parse framebuffer %s: error %d\n",
			name, ret);
	return ret;
}

static int
simplefb_get_width_of(struct drm_device *dev, struct device_node *of_node)
{
	u32 width;
	int ret = simplefb_read_u32_of(dev, of_node, "width", &width);
	if (ret)
		return ret;
	return simplefb_get_validated_int0(dev, "width", width);
}

static int
simplefb_get_height_of(struct drm_device *dev, struct device_node *of_node)
{
	u32 height;
	int ret = simplefb_read_u32_of(dev, of_node, "height", &height);
	if (ret)
		return ret;
	return simplefb_get_validated_int0(dev, "height", height);
}

static int
simplefb_get_stride_of(struct drm_device *dev, struct device_node *of_node)
{
	u32 stride;
	int ret = simplefb_read_u32_of(dev, of_node, "stride", &stride);
	if (ret)
		return ret;
	return simplefb_get_validated_int(dev, "stride", stride);
}

static const struct drm_format_info *
simplefb_get_format_of(struct drm_device *dev, struct device_node *of_node)
{
	const char *format;
	int ret = simplefb_read_string_of(dev, of_node, "format", &format);
	if (ret)
		return ERR_PTR(ret);
	return simplefb_get_validated_format(dev, format);
}

static struct resource *
simplefb_get_memory_of(struct drm_device *dev, struct device_node *of_node)
{
	struct device_node *np;
	struct property *loop_prop;
	struct resource *res;
	int err;
	loop_prop = of_node->properties;

	np = of_parse_phandle(of_node, "memory-region", 0);
	if (!np)
		return NULL;

	res = devm_kzalloc(dev->dev, sizeof(*res), GFP_KERNEL);
	
	if (!res)
		return ERR_PTR(-ENOMEM);

	err = of_address_to_resource(np, 0, res);
	if (err)
		return ERR_PTR(err);

	if (of_property_read_bool(of_node, "reg"))
		drm_err("preferring \"memory-region\" over \"reg\" property\n");

	return res;
}

static int simpledrm_device_init_fb(struct simpledrm_device *sdev)
{
	int width, height, stride;
	int width_mm = 0, height_mm = 0;
	const struct drm_format_info *format;
	struct drm_format_name_buf buf;
	struct drm_device *dev = &sdev->dev;
	struct platform_device *pdev = sdev->pdev;
	struct device_node *of_node = pdev->dev.of_node;
	const struct simplefb_platform_data *pd = dev_get_platdata(&pdev->dev);
	if (pd) {
		width = simplefb_get_width_pd(dev, pd);
		if (width < 0)
			return width;
		height = simplefb_get_height_pd(dev, pd);
		if (height < 0)
			return height;
		stride = simplefb_get_stride_pd(dev, pd);
		if (stride < 0)
			return stride;
		format = simplefb_get_format_pd(dev, pd);
		if (IS_ERR(format))
			return PTR_ERR(format);
	} else if (of_node) {
		width = simplefb_get_width_of(dev, of_node);
		if (width < 0)
			return ERR_PTR(width);
		height = simplefb_get_height_of(dev, of_node);
		if (height < 0)
			return ERR_PTR(height);
		stride = simplefb_get_stride_of(dev, of_node);
		if (stride < 0)
			return ERR_PTR(stride);
		format = simplefb_get_format_of(dev, of_node);
		if (IS_ERR(format))
			return ERR_CAST(format);
	} else {
		drm_err("no simplefb configuration found\n");
		return -ENODEV;
	}

	sdev->mode = simpledrm_mode(width, height);
	sdev->format = format;
	sdev->pitch = stride;

	drm_dbg(DRM_UT_KMS, "display mode={" DRM_MODE_FMT "}\n",
		    DRM_MODE_ARG(&sdev->mode));

	return 0;
}

/*
 * Memory management
 */

static int simpledrm_device_init_mm(struct simpledrm_device *sdev)
{
	struct drm_device *dev = &sdev->dev;
	struct platform_device *pdev = sdev->pdev;
	struct device_node *of_node = pdev->dev.of_node;
	struct resource *mem;
	void __iomem *screen_base;
	int ret;

	mem = simplefb_get_memory_of(dev, of_node);
	if(!mem)
	{

		mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
		if (!mem)
			return -EINVAL;
	}

	ret = devm_aperture_acquire_from_firmware(dev, mem->start, resource_size(mem));
	if (ret) {
		drm_err("could not acquire memory range %pr: error %d\n",
			mem, ret);
		return ret;
	}

	screen_base = devm_ioremap_wc(&pdev->dev, mem->start,
				      resource_size(mem));
	if (!screen_base)
		return -ENOMEM;

	sdev->mem = mem;
	sdev->screen_base = screen_base;

	return 0;
}

/*
 * Modesetting
 */

/*
 * Support all formats of simplefb and maybe more; in order
 * of preference. The display's update function will do any
 * conversion necessary.
 *
 * TODO: Add blit helpers for remaining formats and uncomment
 *       constants.
 */
static const uint32_t simpledrm_default_formats[] = {
	DRM_FORMAT_XRGB8888,
	DRM_FORMAT_ARGB8888,
	DRM_FORMAT_RGB565,
	//DRM_FORMAT_XRGB1555,
	//DRM_FORMAT_ARGB1555,
	DRM_FORMAT_RGB888,
	//DRM_FORMAT_XRGB2101010,
	//DRM_FORMAT_ARGB2101010,
};

static const uint64_t simpledrm_format_modifiers[] = {
	DRM_FORMAT_MOD_LINEAR,
	DRM_FORMAT_MOD_INVALID
};

static int simpledrm_connector_helper_get_modes(struct drm_connector *connector)
{
	struct simpledrm_device *sdev = simpledrm_device_of_dev(connector->dev);
	struct drm_display_mode *mode;
	mode = drm_mode_duplicate(connector->dev, &sdev->mode);
	if (!mode)
		return 0;

	if (mode->name[0] == '\0')
		drm_mode_set_name(mode);

	mode->type |= DRM_MODE_TYPE_PREFERRED;
	drm_mode_probed_add(connector, mode);

	if (mode->width_mm)
		connector->display_info.width_mm = mode->width_mm;
	if (mode->height_mm)
		connector->display_info.height_mm = mode->height_mm;

	return 1;
}

static const struct drm_connector_helper_funcs simpledrm_connector_helper_funcs = {
	.get_modes = simpledrm_connector_helper_get_modes,
};

static const struct drm_connector_funcs simpledrm_connector_funcs = {
	.reset = drm_atomic_helper_connector_reset,
	.fill_modes = drm_helper_probe_single_connector_modes,
	.destroy = drm_connector_cleanup,
	.atomic_duplicate_state = drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_connector_destroy_state,
};

static int
simpledrm_simple_display_pipe_mode_valid(struct drm_crtc *crtc,
				    const struct drm_display_mode *mode)
{
	struct simpledrm_device *sdev = simpledrm_device_of_dev(crtc->dev);
	if (mode->hdisplay != sdev->mode.hdisplay &&
	    mode->vdisplay != sdev->mode.vdisplay)
		return MODE_ONE_SIZE;
	else if (mode->hdisplay != sdev->mode.hdisplay)
		return MODE_ONE_WIDTH;
	else if (mode->vdisplay != sdev->mode.vdisplay)
		return MODE_ONE_HEIGHT;

	return MODE_OK;
}

static void
simpledrm_simple_display_pipe_enable(struct drm_simple_display_pipe *pipe,
				     struct drm_crtc_state *crtc_state,
				     struct drm_plane_state *plane_state)
{
	struct simpledrm_device *sdev = simpledrm_device_of_dev(pipe->crtc.dev);
	struct drm_shadow_plane_state *shadow_plane_state = to_drm_shadow_plane_state(plane_state);
	struct drm_framebuffer *fb = plane_state->fb;
	void *vmap = shadow_plane_state->map[0].vaddr; /* TODO: Use mapping abstraction properly */
	if (!fb)
		return;

	drm_fb_blit_dstclip(sdev->screen_base, sdev->pitch,
			    sdev->format->format, vmap, fb);
}

static void
simpledrm_simple_display_pipe_disable(struct drm_simple_display_pipe *pipe)
{
	struct simpledrm_device *sdev = simpledrm_device_of_dev(pipe->crtc.dev);
	struct drm_device *dev = &sdev->dev;
	int idx;
	if (!drm_dev_enter(dev, &idx))
		return;

	/* Clear screen to black if disabled */
	memset_io(sdev->screen_base, 0, sdev->pitch * sdev->mode.vdisplay);

	drm_dev_exit(idx);
}

static void
simpledrm_simple_display_pipe_update(struct drm_simple_display_pipe *pipe,
				     struct drm_plane_state *old_plane_state)
{
	struct simpledrm_device *sdev = simpledrm_device_of_dev(pipe->crtc.dev);
	struct drm_plane_state *plane_state = pipe->plane.state;
	struct drm_shadow_plane_state *shadow_plane_state = to_drm_shadow_plane_state(plane_state);
	void *vmap = shadow_plane_state->map[0].vaddr; /* TODO: Use mapping abstraction properly */
	struct drm_framebuffer *fb = plane_state->fb;
	struct drm_rect clip;
	if (!fb)
		return;

	if (!drm_atomic_helper_damage_merged(old_plane_state, plane_state, &clip))
		return;

	drm_fb_blit_rect_dstclip(sdev->screen_base, sdev->pitch,
				 sdev->format->format, vmap, fb, &clip);
}

static const struct drm_simple_display_pipe_funcs
simpledrm_simple_display_pipe_funcs = {
	.mode_valid = simpledrm_simple_display_pipe_mode_valid,
	.enable = simpledrm_simple_display_pipe_enable,
	.disable = simpledrm_simple_display_pipe_disable,
	.update = simpledrm_simple_display_pipe_update,
	//DRM_GEM_SIMPLE_DISPLAY_PIPE_SHADOW_PLANE_FUNCS
};

static const struct drm_mode_config_funcs simpledrm_mode_config_funcs = {
	.fb_create = drm_gem_fb_create_with_dirty,
	.atomic_check = drm_atomic_helper_check,
	.atomic_commit = drm_atomic_helper_commit,
};

static const uint32_t *simpledrm_device_formats(struct simpledrm_device *sdev,
						size_t *nformats_out)
{
	struct drm_device *dev = &sdev->dev;
	size_t i;
	if (sdev->nformats)
		goto out; /* don't rebuild list on recurring calls */

	/* native format goes first */
	sdev->formats[0] = sdev->format->format;
	sdev->nformats = 1;

	/* default formats go second */
	for (i = 0; i < ARRAY_SIZE(simpledrm_default_formats); ++i) {
		if (simpledrm_default_formats[i] == sdev->format->format)
			continue; /* native format already went first */
		sdev->formats[sdev->nformats] = simpledrm_default_formats[i];
		sdev->nformats++;
	}

	/*
	 * TODO: The simpledrm driver converts framebuffers to the native
	 * format when copying them to device memory. If there are more
	 * formats listed than supported by the driver, the native format
	 * is not supported by the conversion helpers. Therefore *only*
	 * support the native format and add a conversion helper ASAP.
	 */
	if (DRM_WARN_ONCE(
			  "format conversion helpers required for %p4cc",
			  &sdev->format->format)) {
		sdev->nformats = 1;
	}

out:
	*nformats_out = sdev->nformats;
	return sdev->formats;
}

static int simpledrm_device_init_modeset(struct simpledrm_device *sdev)
{
	struct drm_device *dev = &sdev->dev;
	struct drm_display_mode *mode = &sdev->mode;
	struct drm_connector *connector = &sdev->connector;
	struct drm_simple_display_pipe *pipe = &sdev->pipe;
	const uint32_t *formats;
	size_t nformats;
	int ret;

	drm_mode_config_init(dev);
	dev->mode_config.min_width = mode->hdisplay;
	dev->mode_config.max_width = mode->hdisplay;
	dev->mode_config.min_height = mode->vdisplay;
	dev->mode_config.max_height = mode->vdisplay;
	dev->mode_config.prefer_shadow_fbdev = true;
	dev->mode_config.preferred_depth = sdev->format->cpp[0] * 8;
	dev->mode_config.funcs = &simpledrm_mode_config_funcs;

	ret = drm_connector_init(dev, connector, &simpledrm_connector_funcs,
				 DRM_MODE_CONNECTOR_Unknown);
	if (ret)
		return ret;
	drm_connector_helper_add(connector, &simpledrm_connector_helper_funcs);

	formats = simpledrm_device_formats(sdev, &nformats);

	ret = drm_simple_display_pipe_init(dev, pipe, &simpledrm_simple_display_pipe_funcs,
					   formats, nformats, simpledrm_format_modifiers,
					   connector);
	if (ret)
		return ret;

	drm_mode_config_reset(dev);

	return 0;
}

/*
 * Init / Cleanup
 */

static struct simpledrm_device *
simpledrm_device_create(struct drm_driver *drv, struct platform_device *pdev)
{
	struct simpledrm_device *sdev;
	struct drm_device *ddev;
	struct device *dev = &pdev->dev;
	int ret;
	void* container;

	container = kzalloc(sizeof(struct simpledrm_device), GFP_KERNEL);
	if (!container)
		return -ENOMEM;
	ddev = container + offsetof(struct simpledrm_device, dev);

	//return -EPROBE_DEFER;
	ret = devm_drm_dev_init(dev, ddev, drv);
	if (ret)
	{
		kfree(container);
		return ERR_PTR(ret);
	}
	sdev = container;
	sdev->pdev = pdev;
	platform_set_drvdata(pdev, sdev);

	//ret = simpledrm_device_init_clocks(sdev);
	//if (ret) {
	//	drm_dev_put(ddev);
	//	kfree(sdev);
	//	return ERR_PTR(ret);
	//}

	ret = simpledrm_device_init_fb(sdev);
	if (ret)
		return ERR_PTR(ret);
	ret = simpledrm_device_init_mm(sdev);
	if (ret)
		return ERR_PTR(ret);
	ret = simpledrm_device_init_modeset(sdev);
	if (ret)
		return ERR_PTR(ret);

	return sdev;
}

static struct simpledrm_device *drm_device_get_priv(struct drm_device *drm)
{
	return container_of(drm, struct simpledrm_device, dev);
}

static void simpledrm_device_release(struct drm_device *drm)
{
	struct simpledrm_device *priv = drm_device_get_priv(drm);
	drm_mode_config_cleanup(drm);
	drm_dev_fini(drm);
	kfree(priv);
}

/*
 * DRM driver
 */

DEFINE_DRM_GEM_FOPS(simpledrm_fops);

static struct drm_driver simpledrm_driver = {
	DRM_GEM_SHMEM_DRIVER_OPS,
	.name			= DRIVER_NAME,
	.desc			= DRIVER_DESC,
	.date			= DRIVER_DATE,
	.major			= DRIVER_MAJOR,
	.minor			= DRIVER_MINOR,
	.driver_features	= DRIVER_ATOMIC | DRIVER_GEM | DRIVER_MODESET,
	.fops			= &simpledrm_fops,
	.release 		= simpledrm_device_release
};

static const struct of_device_id simpledrm_of_match[] = {
	{ .compatible = "simple-framebuffer", },
	{ },
};

MODULE_DEVICE_TABLE(of, simpledrm_of_match);

/*
 * Platform driver
 */

static int simpledrm_probe(struct platform_device *pdev)
{
	struct simpledrm_device *sdev;
	struct drm_device *dev;
	int ret;

	sdev = simpledrm_device_create(&simpledrm_driver, pdev);
	if (IS_ERR(sdev))
	{
		return PTR_ERR(sdev);
	}
	dev = &sdev->dev;

	ret = drm_dev_register(dev, 0);
	if (ret)
	{
		return ret;
	}

	drm_fbdev_generic_setup(dev, 16);

	return 0;
}

static int simpledrm_remove(struct platform_device *pdev)
{
	struct simpledrm_device *sdev = platform_get_drvdata(pdev);
	struct drm_device *dev = &sdev->dev;
	drm_dev_unregister(dev);

	return 0;
}

static struct platform_driver simpledrm_platform_driver = {
	.driver = {
		.name = "simple-framebuffer", /* connect to sysfb */
		.of_match_table = simpledrm_of_match,
	},
	.probe = simpledrm_probe,
	.remove = simpledrm_remove,
};

module_platform_driver(simpledrm_platform_driver);

//static struct platform_device simpledrm_pdev = {
//	.name		= "simple-framebuffer",
//	.id		= -1,
//};
//static int __init simpledrm_init(void)
//{
//	platform_device_register(&simpledrm_pdev);
//}
//
//module_init(simpledrm_init);

MODULE_DESCRIPTION(DRIVER_DESC);
