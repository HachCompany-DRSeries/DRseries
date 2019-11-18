/*
 * HiCO_SH7723
 *
 * Copyright (C) 2016 by emtrion GmbH
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ARCH_SH_BOARDS_EMTRION_BOOTLOGO_H
#define _ARCH_SH_BOARDS_EMTRION_BOOTLOGO_H

typedef enum {
        BL_IGNORED,
        BL_COPIED,
        BL_KEPT,
} bootlogo_e;

bootlogo_e bootlogo_init(struct sh_mobile_lcdc_chan_cfg *ch, unsigned long smem_start, int size);

void bootlogo_overlay_with_linux_logo(void *board_data);

#endif /* _ARCH_SH_BOARDS_EMTRION_BOOTLOGO_H */
