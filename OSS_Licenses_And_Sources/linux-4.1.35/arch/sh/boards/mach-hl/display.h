/*
 * HiCO_SH7723
 *
 * Copyright (C) 2016 by emtrion GmbH
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ARCH_SH_BOARDS_EMTRION_DISPLAY_H
#define _ARCH_SH_BOARDS_EMTRION_DISPLAY_H

#define DISPLAY_PARA_ANZ 12

extern const char* display_configuration;

extern int __init display_configured(void);
//void __init display_read_parameters(struct sh_mobile_lcdc_info *info);
//void display_configure_data_pins(void *board_data, int for_lcdc);

#define LCDC_BASE       0xfe940000
#define LCDC_END        0xfe942fff

#endif /* _ARCH_SH_BOARDS_EMTRION_DISPLAY_H */
