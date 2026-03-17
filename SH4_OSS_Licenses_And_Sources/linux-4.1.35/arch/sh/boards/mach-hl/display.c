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
#include <video/sh_mobile_lcdc.h>

#include "display.h"
#include "bootlogo.h"
#include "hico7723/sh7723.h"

const char* display_configuration;
/*
int display_phys_width;
int display_phys_height;
*/
static int __init display_config(char *options)
{
        display_configuration = options;
        return 0;
}
early_param("display_configuration", display_config);


int __init display_configured(void)
{
        if (display_configuration == NULL || !strlen(display_configuration) )
                return -2;
	else
	  return 0;
}
