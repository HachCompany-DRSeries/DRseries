/*
 * UIOMux: a conflict manager for system resources, including UIO devices.
 * Copyright (C) 2010 Renesas Technology Corp.
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

#include <stdio.h>
#include <sys/time.h>

#include <uiomux/uiomux.h>

#include "uiomux_tests.h"

static UIOMux *uiomux;

int
main() {

	int ret;

	void *veu_register_base;
	unsigned long veu_phys_memory_base;
	int i;
	struct timeval tval;

	tval.tv_sec = 5;
	tval.tv_usec = 0;

	INFO ("Opening UIOMux for VEU");
	uiomux = uiomux_open();

	if (uiomux == NULL)
		FAIL ("Opening UIOMux");

	INFO ("uiomux_sleep-ing with a timeout of 5 secs");
	if (uiomux_sleep_timeout(uiomux, UIOMUX_SH_VEU, &tval) < 0) {
		INFO ("Woken up after timeout (or other error)");
	} else {
		INFO ("Woken up due to VEU event");
	}

	return 0;
}
