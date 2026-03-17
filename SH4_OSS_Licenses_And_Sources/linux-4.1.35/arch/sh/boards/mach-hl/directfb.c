/*
 * HiCO_SH7723
 *
 * Copyright (C) 2016 by emtrion GmbH
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/init.h>
#include <asm/page.h>

static int __init early_directfb_setup(char *options)
{
        if ((options != NULL) && !strcmp (options, "hw")) {
                pr_notice("DirectFB HW Acceleration enabled, reserving 8MB RAM\n");
                memory_limit = CONFIG_MEMORY_SIZE - 8 * 1024 * 1024;
        }

        return 0;
}

early_param("directfb", early_directfb_setup);
