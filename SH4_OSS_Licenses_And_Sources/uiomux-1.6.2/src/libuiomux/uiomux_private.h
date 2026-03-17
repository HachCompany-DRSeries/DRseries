/*
 * UIOMux: a conflict manager for system resources, including UIO devices.
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

#ifndef __UIOMUX_PRIVATE_H__
#define __UIOMUX_PRIVATE_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <uiomux/resource.h>

#include "uio.h"

/***********************************************************
 * Library-private defines
 */

#define UIOMUX_BLOCK_MAX 	UIO_DEVICE_MAX

/***********************************************************
 * Library-private Types
 */

struct uiomux {
  /* Locked resources */
  uiomux_resource_t locked_resources;

  struct uio * uios[UIOMUX_BLOCK_MAX];
};

struct uiomux_addr_block {
	void *virt;
	unsigned long phys;
	size_t size;
	struct uiomux_addr_block *next;
};

/***********************************************************
 * Library-private functions
 */

int
uiomux_close (struct uiomux * uiomux);

const char *
uiomux_name(uiomux_resource_t resource);

#endif /* __UIOMUX_PRIVATE_H__ */
