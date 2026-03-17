/*
 * HiCO_SH7723
 *
 * Copyright (C) 2016 by emtrion GmbH
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */


#include <linux/init.h>
#include <linux/string.h>
#include <linux/bootmem.h>
#include <linux/pfn.h>
#include <video/sh_mobile_lcdc.h>

#include "bootlogo.h"

static int keep = 1;

static int __init bootlogo_setup(char *options)
{
        if (options != NULL && !strcmp(options,"keep"))
                keep = 1;
        else
                keep = 0;

        return 0;
}
early_param("bootlogo", bootlogo_setup);

/**
 * bootlogo_init - Uses the framebuffer which the bootloader already
 * has initialized.
 *
 * Must be called in early phase before vmalloc is initialized to have access
 * to bootmem.
 */
bootlogo_e __init bootlogo_init(struct sh_mobile_lcdc_chan_cfg *ch, unsigned long smem_start, int size)
{
        void *fb = NULL;
        bootlogo_e status = BL_IGNORED;

        if (keep) {
                if (smem_start >= PFN_PHYS(max_pfn)) {
                        /* Framebuffer is beyond memory accessable to linux,
                         * e.g. directfb=hw or mem=120 and framebuffer placed at 125MB.
                         * Create a new buffer in linux memory and copy image */
                        fb = alloc_bootmem_pages_nopanic(size);
                        if (fb != NULL) {
                                memcpy(fb, phys_to_virt(smem_start), size);
                                status = BL_COPIED;
                        }
                } else if (!reserve_bootmem(smem_start, size, BOOTMEM_DEFAULT)) {
                        fb = phys_to_virt(smem_start);
                        status = BL_KEPT;
                }

                if (status != BL_IGNORED) {
                        ch->bootlogo.size        = size;
                        ch->bootlogo.smem_start  = virt_to_phys((unsigned long) fb);
			ch->bootlogo.screen_base = fb;
                }
        }

        pr_info("Bootlogo is %s\n", status == BL_IGNORED ? "ignored" : (status == BL_COPIED ? "copied" : "kept") );

        return status;
}
