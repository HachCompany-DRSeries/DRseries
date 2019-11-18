/*
 * UIOMux: a conflict manager for system resources, including UIO devices.
 * Copyright (C) 2010 IGEL Co.,Ltd.
 * Copyright (C) 2009 Renesas Technology Corp.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA  02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <uiomux/uiomux.h>

#include "uiomux_tests.h"

const char *uios[] = {
	"VEU1",
	"VEU2",
	"VEU3",
	"FOO",
	"BEU0",
	"BAR",
	"BEU1",
	NULL
};

int
main (int argc, char *argv[])
{
	UIOMux *uiomux;
	int i, ret, count;
	char **name, **name2;
	uiomux_resource_t val;

	INFO("List available devices");
	uiomux_list_device(&name, &count);

	for (i = 0; i < count; i++)
		printf(name[i]);

	uiomux_list_device(&name2, &count);

	INFO("1st uiomux_list_device() returned %08x\n", name);
	INFO("2nd uiomux_list_device() returned %08x\n", name2);

	INFO("Opening UIOMux");
	i = 0;
	while (uios[i])
		printf("%s ", uios[i++]);
	printf("\n");
	uiomux = uiomux_open_named(uios);
	if (uiomux == NULL)
		FAIL("Opening UIOMux");

	INFO("Check reserved UIO");
	val = uiomux_check_resource(uiomux, UIOMUX_ALL);
	INFO("%08x", val);

	INFO("Check names of given UIOs");
	for (i = 0; i < 16; i++) {
		if (val & (1 << i))
			printf("%s", uiomux_check_name(uiomux, 1 << i));
	}

	INFO("Calling UIOMux info");
	uiomux_info(uiomux);

	INFO("Closing UIOMux");
	ret = uiomux_close(uiomux);
	if (ret != 0)
		FAIL("Closing first UIOMux");

	return 0;
}
