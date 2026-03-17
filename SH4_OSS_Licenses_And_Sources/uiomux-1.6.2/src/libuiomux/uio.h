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

#ifndef __UIOMUX_UIO_H__
#define __UIOMUX_UIO_H__

#define UIO_DEVICE_MAX		16
#define UIO_BUFFER_MAX		(64 * 1024 * 1024) /* 64MB, depends on
						      get_free_pages() */

/* max length of UIO names found in /sys/class/uio/uioNN/name */
#define UIO_DEVICE_NAME_MAX    32

/* max path length of UIO directories /sys/class/uio/uioNN */
#define UIO_DEVICE_PATH_MAX    32

struct uio_device {
  char name[UIO_DEVICE_NAME_MAX];
  char path[UIO_DEVICE_PATH_MAX];
  int fd;
};

struct uio_map {
  unsigned long address;
  unsigned long size;
  void *iomem;
};

struct uio {
  struct uio_device dev;
  struct uio_map mmio;
  struct uio_map mem;
  int exit_sleep_pipe[2];
  int device_index;
};

struct uio *
uio_open (const char * name);

int
uio_close (struct uio * uio);

int
uio_sleep(struct uio *uio, struct timeval *timeout);

int
uio_read_nonblocking(struct uio *uio);

void *
uio_malloc (struct uio * uio, size_t size, int align, int shared);

void
uio_free (struct uio * uio, void * address, size_t size);

void
uio_meminfo (struct uio * uio);

int
uio_list_device (char *** names, int * count);

int
uio_device_index (struct uio * uio);

#endif /* __UIOMUX_UIO_H__ */
