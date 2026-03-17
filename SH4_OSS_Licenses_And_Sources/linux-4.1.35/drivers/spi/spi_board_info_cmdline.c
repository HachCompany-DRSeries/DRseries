/*
 * spi_board_info_cmdline.c
 *
 * Copyright (c) 2010 by emtrion GmbH
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 **/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/string.h>

#define DRIVER_NAME "spi_board_info"

static char *cmdline;

/**
 * spi_board_info_setup - stores our kernel commandline options for later use
 */
static int __init spi_board_info_setup(char *opt)
{
  // printk("%s: %s\n",__FUNCTION__,opt);
	cmdline = opt;
	return 0;
}
__setup("spi_board_info=", spi_board_info_setup);


/**
 * spi_board_info_register_device - registers an SPI device defined by kernel argument line
 */
static int __init spi_board_info_register_device(char *dev)
{
	struct spi_board_info *info = NULL;
	int num = 0;
	int res = 0;
	char *opt = dev;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	info->irq = -1;		/* no irq */

	/* parse configuration and store in spi_board_info */
	while (opt && *opt) {
		char *next = strchr(opt, ':');

		if (next)
                  *next++ = '\0';

		if (strlen(opt) > 0) {
                  // printk("%s: %d.: %s\n",__FUNCTION__,num,opt);
			switch (num) {
			    case 0:
				strncpy(info->modalias, opt, ARRAY_SIZE(info->modalias));
				info->modalias[ARRAY_SIZE(info->modalias)-1]=0;
				break;
			    case 1:
                                opt += 13; // skip 'max_speed_hz='
				info->max_speed_hz = simple_strtoul(opt, NULL, 10);
                                // printk("%s: set max_speed_hz=%d\n",__FUNCTION__,info->max_speed_hz);
				break;
			    case 2:
				info->bus_num = simple_strtoul(opt, NULL, 10);
				break;
			    case 3:
				info->chip_select = simple_strtoul(opt, NULL, 10);
				break;
			    case 4:
				info->irq = simple_strtoul(opt, NULL, 10);
				break;
			    default:
				if (!strcmp(opt, "cpha"))
					info->mode |= SPI_CPHA;
				else if (!strcmp(opt, "cpol"))
					info->mode |= SPI_CPOL;
				else if (!strcmp(opt, "mode0"))
					info->mode |= SPI_MODE_0;
				else if (!strcmp(opt, "mode1"))
					info->mode |= SPI_MODE_1;
				else if (!strcmp(opt, "mode2"))
					info->mode |= SPI_MODE_2;
				else if (!strcmp(opt, "mode3"))
					info->mode |= SPI_MODE_3;
				else if (!strcmp(opt, "cs"))
					info->mode |= SPI_CS_HIGH;
				else if (!strcmp(opt, "lsb"))
					info->mode |= SPI_LSB_FIRST;
				else if (!strcmp(opt, "3wire"))
					info->mode |= SPI_3WIRE;
				else if (!strcmp(opt, "loop"))
					info->mode |= SPI_LOOP;
				else if (!strcmp(opt, "no_cs"))
					info->mode |= SPI_NO_CS;
				else if (!strcmp(opt, "ready"))
					info->mode |= SPI_READY;
				else {
					pr_err(DRIVER_NAME ": Don't know value %s\n", opt);
					kfree(info);
					return -1;
				}
				
				break;
			}
		}

		/* next option */
		opt = next;
		num++;
	}

	if (num > 0) {
		res = spi_register_board_info(info, 1);
		if (res < 0)
			pr_err(DRIVER_NAME ": registering failed for %s with error %i\n", info->modalias, res);
		else
			pr_info(DRIVER_NAME ": registered %s\n", info->modalias);
	}

	return res;
}

/**
 * spi_board_info_init - scans the command line option spi_board_info for devices
 */
static int __init spi_board_info_init(void)
{
	int res;
	char *dev = cmdline;

	/* split and register spi devices */
	while (dev && *dev) {
		char *next = strchr(dev, ',');

		if (next)
			*next++ = '\0';

		if ((res=spi_board_info_register_device(dev)) < 0)
			return res;

		/* next device */
		dev = next;
	}
	
	return 0;
}

/* additional devices should be registered after the architecture has added it's
 * own and before the SPI subsystem initializes itself */
arch_initcall_sync(spi_board_info_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Markus Pietrek");
MODULE_DESCRIPTION("Command line parser for SPI boardinfo");
