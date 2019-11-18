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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>

#include "uiomux/uiomux.h"
#include "uiomux_private.h"
#include "uio.h"

/* #define DEBUG */

static int init_done = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t uio_mutex[UIOMUX_BLOCK_MAX];

static struct uiomux_addr_block *g_mem_regions = NULL;

/* Simple single list */
static void add_mem_block(struct uiomux_addr_block **start, struct uiomux_addr_block *new)
{
	struct uiomux_addr_block *curr = *start;

	new->next = NULL;

	if (curr == NULL) {
		*start = new;
		return;
	}
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new;
}

/* Remove old from the list, doesn't free old */
static int rm_mem_block(struct uiomux_addr_block **start, struct uiomux_addr_block *old)
{
	struct uiomux_addr_block *curr = *start;

	if (curr == NULL)
		return -1;

	if (curr == old) {
		*start = curr->next;
		return 0;
	}
	while (curr->next != old)
		curr = curr->next;
	curr->next = old->next;

	return 0;
}

/* Find a mem block */
static struct uiomux_addr_block *
find_mem_block(struct uiomux_addr_block **start, void *virt)
{
	struct uiomux_addr_block *curr = *start;

	while (curr != NULL) {
		if ((virt >= curr->virt) && (virt < curr->virt+curr->size))
			return curr;
		curr = curr->next;
	}

	return NULL;
}

/* Dump all mem blocks */
static void dump_mem_blocks(struct uiomux_addr_block **start)
{
	struct uiomux_addr_block *curr = *start;

	while (curr != NULL) {
		fprintf(stderr, "virt=%p, phys=0x%lX, size=%d\n", curr->virt, curr->phys, curr->size);
		curr = curr->next;
	}
}

struct uiomux *uiomux_open_named(const char * name[])
{
	struct uiomux *uiomux;
	struct uiomux_addr_block *mem;
	int i;

	pthread_mutex_lock(&mutex);
	if (!init_done) {
		pthread_mutex_t temp_mutex = PTHREAD_MUTEX_INITIALIZER;

		for (i = 0; i < UIOMUX_BLOCK_MAX; i++)
			memcpy(&uio_mutex[i], &temp_mutex, sizeof(temp_mutex));

		init_done = 1;
	}
	pthread_mutex_unlock(&mutex);

	uiomux = (struct uiomux *) calloc(1, sizeof(*uiomux));
	if (!uiomux)
		return NULL;

	/* Open handles to all hardware blocks */
	for (i = 0; i < UIOMUX_BLOCK_MAX; i++) {
		if (!name[i])
			break;
		uiomux->uios[i] = uio_open(name[i]);
	}

	return uiomux;
}

struct uiomux *uiomux_open_blocks(uiomux_resource_t blocks)
{
	struct uiomux *uiomux;
	struct uiomux_addr_block *mem;
	const char *name = NULL;
	int i, bit;

	pthread_mutex_lock(&mutex);
	if (!init_done) {
		pthread_mutex_t temp_mutex = PTHREAD_MUTEX_INITIALIZER;

		for (i = 0; i < UIOMUX_BLOCK_MAX; i++)
			memcpy(&uio_mutex[i], &temp_mutex, sizeof(temp_mutex));

		init_done = 1;
	}
	pthread_mutex_unlock(&mutex);

	uiomux = (struct uiomux *) calloc(1, sizeof(*uiomux));
	if (!uiomux)
		return NULL;

	/* Open handles to all hardware blocks */
	for (i = 0; i < UIOMUX_BLOCK_MAX; i++) {
		bit = 1 << i;
		if ((blocks & bit) && (name = uiomux_name(bit)) != NULL) {
			uiomux->uios[i] = uio_open(name);
		}
	}

	return uiomux;
}

struct uiomux *uiomux_open(void)
{
	return uiomux_open_blocks(UIOMUX_ALL);
}

static void uiomux_delete(struct uiomux *uiomux)
{
	struct uio *uio;
	int i;

	pthread_mutex_lock(&mutex);

	for (i = 0; i < UIOMUX_BLOCK_MAX; i++) {
		uio = uiomux->uios[i];
		if (uio != NULL) {
			uio_close(uio);
		}
	}

	pthread_mutex_unlock(&mutex);

	free(uiomux);
}

int uiomux_close(struct uiomux *uiomux)
{
	if (uiomux == NULL)
		return -1;

#ifdef DEBUG
	fprintf(stderr, "%s: IN\n", __func__);
#endif

	uiomux_delete(uiomux);

	return 0;
}

int uiomux_system_reset(struct uiomux *uiomux)
{
	/* Locks by other tasks cannot be invalidated. */
	return -1;
}

int uiomux_system_destroy(struct uiomux *uiomux)
{
	uiomux_delete(uiomux);

	return 0;
}

int uiomux_lock(struct uiomux *uiomux, uiomux_resource_t blockmask)
{
	unsigned long *reg_base;
	int i, k, ret = 0;
	struct uio *uio;

	for (i = 0; i < UIOMUX_BLOCK_MAX; i++) {
		if (blockmask & (1 << i)) {

			uio = uiomux->uios[i];
			if (!uio) {
				fprintf(stderr, "No uio exists.\n");
				goto undo_locks;
			}

			/* Lock uio within this process. This is required because the
			   fcntl()'s advisory lock is only valid between processes, not
			   within a process. */
			ret = pthread_mutex_lock(&uio_mutex[uio_device_index(uio)]);
			if (ret != 0) {
				perror("pthread_mutex_lock failed");
				goto undo_locks;
			}

			ret = flock(uio->dev.fd, LOCK_EX);
			if (ret < 0) {
				perror("flock failed");
				goto undo_locks;
			}

			uiomux->locked_resources |= 1U << i;
			uio_read_nonblocking(uio);
		}
	}

	return 0;

undo_locks:
	{
		int save_errno = errno;
		uiomux_unlock(uiomux, uiomux->locked_resources);
		errno = save_errno;
	}

	return ret;
}

int uiomux_unlock(struct uiomux *uiomux, uiomux_resource_t blockmask)
{
	unsigned long *reg_base;
	int i, k, ret;
	struct uio *uio;

	for (i = UIOMUX_BLOCK_MAX - 1; i >= 0; i--) {
		if (blockmask & (1 << i)) {
			uio = uiomux->uios[i];
			if (uio) {
				ret = flock(uio->dev.fd, LOCK_UN);
				if (ret < 0)
					perror("flock failed");

				ret = pthread_mutex_unlock(&uio_mutex[uio_device_index(uio)]);
				if (ret != 0)
					perror("pthread_mutex_unlock failed");
			}
		}
	}

	return 0;
}

#define MULTI_BIT(x) (((long)x)&(((long)x)-1))

static int
uiomux_get_block_index(struct uiomux *uiomux, uiomux_resource_t blockmask)
{
	int i;

	/* Invalid if multiple bits are set */
	if (MULTI_BIT(blockmask)) {
#ifdef DEBUG
		fprintf(stderr, "%s: Multiple blocks specified\n", __func__);
#endif
		return -1;
	}

	for (i = 0; i < UIOMUX_BLOCK_MAX; i++) {
		if (blockmask & (1 << i)) {
			return i;
		}
	}

	return -1;
}

int uiomux_sleep(struct uiomux *uiomux, uiomux_resource_t blockmask)
{
	struct uio *uio;
	int ret = 0;
	int i;

	/* Invalid if multiple bits are set, or block not found */
	if ((i = uiomux_get_block_index(uiomux, blockmask)) == -1)
		return -1;

	uio = uiomux->uios[i];

	if (uio) {
#ifdef DEBUG
		fprintf(stderr, "%s: Waiting for block %d\n", __func__, i);
#endif
		ret = uio_sleep(uio, NULL);
	}

	return ret;
}

int uiomux_sleep_timeout(struct uiomux *uiomux,
			uiomux_resource_t blockmask,
			struct timeval *timeout)
{
	struct uio *uio;
	long ret = 0;
	int i;

	/* Invalid if multiple bits are set, or block not found */
	if ((i = uiomux_get_block_index(uiomux, blockmask)) == -1)
		return -1;

	uio = uiomux->uios[i];

	if (uio) {
#ifdef DEBUG
		fprintf(stderr, "%s: Waiting for block %d\n", __func__, i);
#endif
		ret = uio_sleep(uio, timeout);

	}

	return ret;
}

int uiomux_wakeup(struct uiomux *uiomux, uiomux_resource_t blockmask)
{
	struct uio *uio;
	int ret = 0;
	int i;

	/* Invalid if multiple bits are set, or block not found */
	if ((i = uiomux_get_block_index(uiomux, blockmask)) == -1)
		return -1;

	uio = uiomux->uios[i];

	if (uio) {
#ifdef DEBUG
		fprintf(stderr, "%s: Waking up block %d\n", __func__, i);
#endif
		write (uio->exit_sleep_pipe[1], "UIOX", 4);
	}
	return 0;
}

void *uiomux_malloc(struct uiomux *uiomux, uiomux_resource_t blockmask,
		    size_t size, int align)
{
	struct uio *uio;
	struct uiomux_addr_block *mem;
	void *ret = NULL;
	int i;

	/* Invalid if multiple bits are set, or block not found */
	if ((i = uiomux_get_block_index(uiomux, blockmask)) == -1)
		return NULL;

	uio = uiomux->uios[i];

	if (uio) {
#ifdef DEBUG
		fprintf(stderr, "%s: Allocating %d bytes for block %d\n",
			__func__, size, i);
#endif
		mem = malloc(sizeof(*mem));
		if (!mem)
			return NULL;

		ret = uio_malloc(uio, size, align, 0);

		if (ret) {
			mem->virt = ret;
			mem->phys = uio->mem.address + (ret - uio->mem.iomem);
			mem->size = size;
			pthread_mutex_lock(&mutex);
			add_mem_block(&g_mem_regions, mem);
			pthread_mutex_unlock(&mutex);
		} else {
			free(mem);
		}

#ifdef DEBUG
		fprintf(stderr, "%s: adding phys addr 0x%lX, virt addr %p\n", __func__, mem->phys, mem->virt);
#endif
	}

	return ret;
}

void *uiomux_malloc_shared(struct uiomux *uiomux, uiomux_resource_t blockmask,
		    size_t size, int align)
{
	struct uio *uio;
	void *ret = NULL;
	int i;

	/* Invalid if multiple bits are set, or block not found */
	if ((i = uiomux_get_block_index(uiomux, blockmask)) == -1)
		return NULL;

	uio = uiomux->uios[i];

	if (uio) {
#ifdef DEBUG
		fprintf(stderr, "%s: Allocating %d bytes shm for block %d\n",
			__func__, size, i);
#endif
		ret = uio_malloc(uio, size, align, 1);
	}

	return ret;
}

void
uiomux_free(struct uiomux *uiomux, uiomux_resource_t blockmask,
	    void *address, size_t size)
{
	struct uio *uio;
	struct uiomux_addr_block *mem;
	int i;

	/* Invalid if multiple bits are set, or block not found */
	if ((i = uiomux_get_block_index(uiomux, blockmask)) == -1)
		return;

	uio = uiomux->uios[i];

	if (uio) {
#ifdef DEBUG
		fprintf(stderr, "%s: Freeing memory for block %d\n",
			__func__, i);
#endif
		uio_free(uio, address, size);

		pthread_mutex_lock(&mutex);
		mem = find_mem_block(&g_mem_regions, address);
		if (mem) {
			rm_mem_block(&g_mem_regions, mem);
			free(mem);
		}
		pthread_mutex_unlock(&mutex);
	}
}

int
uiomux_register (void *virt, unsigned long phys, size_t size)
{
	struct uiomux_addr_block *mem;

#ifdef DEBUG
	fprintf(stderr, "%s: phys addr 0x%lX, virt addr %p\n", __func__, phys, virt);
#endif

	mem = malloc(sizeof(*mem));
	if (!mem)
		return 1;
	mem->virt = virt;
	mem->phys = phys;
	mem->size = size;
	pthread_mutex_lock(&mutex);
	add_mem_block(&g_mem_regions, mem);
	pthread_mutex_unlock(&mutex);
	return 0;
}

int
uiomux_unregister (void *virt)
{
	struct uiomux_addr_block *mem;

	pthread_mutex_lock(&mutex);
	mem = find_mem_block(&g_mem_regions, virt);
	if (mem) {
		rm_mem_block(&g_mem_regions, mem);
		free(mem);
	}
	pthread_mutex_unlock(&mutex);
	return (mem == NULL);
}

unsigned long
uiomux_get_mmio(struct uiomux *uiomux, uiomux_resource_t blockmask,
		unsigned long *address, unsigned long *size, void **iomem)
{
	struct uio *uio;
	int i;

	/* Invalid if multiple bits are set, or block not found */
	if ((i = uiomux_get_block_index(uiomux, blockmask)) == -1)
		return 0;

	uio = uiomux->uios[i];

	/* Invalid if no uio associated with it */
	if (uio == NULL)
		return 0;

	if (address)
		*address = uio->mmio.address;
	if (size)
		*size = uio->mmio.size;
	if (iomem)
		*iomem = uio->mmio.iomem;

	return uio->mmio.address;
}

unsigned long
uiomux_get_mem(struct uiomux *uiomux, uiomux_resource_t blockmask,
	       unsigned long *address, unsigned long *size, void **iomem)
{
	struct uio *uio;
	int i;

	/* Invalid if multiple bits are set, or block not found */
	if ((i = uiomux_get_block_index(uiomux, blockmask)) == -1)
		return 0;

	uio = uiomux->uios[i];

	/* Invalid if no uio associated with it */
	if (uio == NULL)
		return 0;

	if (address)
		*address = uio->mem.address;
	if (size)
		*size = uio->mem.size;
	if (iomem)
		*iomem = uio->mem.iomem;

	return uio->mem.address;
}

static unsigned long
uio_map_virt_to_phys(struct uio_map *map, void *virt_address)
{
	if ((virt_address >= map->iomem)
	    && (((unsigned long)virt_address -
		 (unsigned long)map->iomem) < map->size))
		return map->address + ((unsigned long)virt_address -
				       (unsigned long)map->iomem);

	return (unsigned long) -1;
}

static void *
uio_map_phys_to_virt(struct uio_map *map, unsigned long phys_address)
{
	if ((phys_address >= map->address)
	    && ((phys_address - map->address) < map->size))
		return (void *)((unsigned long)map->iomem +
				(phys_address - map->address));

	return NULL;
}

unsigned long
uiomux_virt_to_phys(struct uiomux *uiomux, uiomux_resource_t blockmask,
		    void *virt_address)
{
	struct uio *uio;
	unsigned long ret;
	int i;

	/* Invalid if multiple bits are set, or block not found */
	if ((i = uiomux_get_block_index(uiomux, blockmask)) == -1)
		return 0;

	uio = uiomux->uios[i];

	/* Invalid if no uio associated with it */
	if (uio == NULL)
		return 0;

	if ((ret =
	     uio_map_virt_to_phys(&uio->mem,
				  virt_address)) != (unsigned long) -1)
		return ret;

	if ((ret =
	     uio_map_virt_to_phys(&uio->mmio,
				  virt_address)) != (unsigned long) -1)
		return ret;

	return 0;
}

unsigned long
uiomux_all_virt_to_phys(void *virt)
{
	struct uiomux_addr_block *mem;
	unsigned long phys = 0;

	pthread_mutex_lock(&mutex);
	mem = find_mem_block(&g_mem_regions, virt);
	if (mem)
		phys = mem->phys + (virt - mem->virt);
	pthread_mutex_unlock(&mutex);

#ifdef DEBUG
	fprintf(stderr, "%s: got phys addr 0x%X, virt addr %p\n", __func__, phys, virt);
#endif

	return phys;
}


void *
uiomux_phys_to_virt(struct uiomux *uiomux, uiomux_resource_t blockmask,
		    unsigned long phys_address)
{
	struct uio *uio;
	void * ret;
	int i;

	/* Invalid if multiple bits are set, or block not found */
	if ((i = uiomux_get_block_index(uiomux, blockmask)) == -1)
		return NULL;

	uio = uiomux->uios[i];

	/* Invalid if no uio associated with it */
	if (uio == NULL)
		return NULL;

	if ((ret =
	     uio_map_phys_to_virt(&uio->mem,
				  phys_address)) != NULL)
		return ret;

	if ((ret =
	     uio_map_phys_to_virt(&uio->mmio,
				  phys_address)) != NULL)
		return ret;

	return NULL;
}

const char *uiomux_name(uiomux_resource_t block)
{
	switch (block) {
	case UIOMUX_SH_BEU:
		return "BEU";
		break;
	case UIOMUX_SH_CEU:
		return "CEU";
		break;
	case UIOMUX_SH_JPU:
		return "JPU";
		break;
	case UIOMUX_SH_VEU:
		return "VEU";
		break;
	case UIOMUX_SH_VPU:
		return "VPU5";
		break;
	case UIOMUX_SH_ISP:
		return "ISP";
		break;
	default:
		return NULL;
	}
}

uiomux_resource_t uiomux_query(void)
{
	uiomux_resource_t blocks = UIOMUX_NONE;
	struct uio *uio;
	const char *name = NULL;
	int i;

	for (i = 0; i < UIOMUX_BLOCK_MAX; i++) {
		if ((name = uiomux_name(1 << i)) != NULL) {
			if ((uio = uio_open(name)) != NULL) {
				blocks |= (1 << i);
				uio_close(uio);
			}
		}
	}

	return blocks;
}

static int uiomux_showversion(struct uiomux *uiomux)
{
	printf("uiomux " VERSION "\n");
	return 0;
}

int uiomux_info(struct uiomux *uiomux)
{
	uiomux_resource_t blocks = UIOMUX_NONE;
	struct uio *uio;
	int i;
	long pagesize;

	uiomux_showversion(uiomux);

	pagesize = sysconf(_SC_PAGESIZE);

	for (i = 0; i < UIOMUX_BLOCK_MAX; i++) {
		uio = uiomux->uios[i];
		if (uio != NULL) {
			printf("%s: %s", uio->dev.path,
			       uio->dev.name);
			printf
			    ("\tmmio\t0x%8lx\t0x%8lx bytes (%ld pages)\n\tmem\t0x%8lx\t0x%8lx bytes (%ld pages)\n",
			     uio->mmio.address,
			     uio->mmio.size,
			     (uio->mmio.size + pagesize - 1)/ pagesize,
			     uio->mem.address, uio->mem.size,
			     (uio->mem.size + pagesize - 1)/ pagesize);
		}
	}

	return 0;
}

int uiomux_meminfo(struct uiomux *uiomux)
{
	uiomux_resource_t blocks = UIOMUX_NONE;
	struct uio *uio;
	int i;
	long pagesize;

	uiomux_showversion(uiomux);

	pagesize = sysconf(_SC_PAGESIZE);

	for (i = 0; i < UIOMUX_BLOCK_MAX; i++) {
		uio = uiomux->uios[i];
		if (uio != NULL) {
			printf("%s: %s", uio->dev.path,
			       uio->dev.name);
			uio_meminfo(uio);
		}
	}

	return 0;
}

int uiomux_list_device(char ***names, int *count)
{
	return uio_list_device(names, count);
}

uiomux_resource_t
uiomux_check_resource(struct uiomux *uiomux, uiomux_resource_t blocks)
{
	int i, bitmask;
	uiomux_resource_t ret = 0;

	for (i = 0; i < UIOMUX_BLOCK_MAX; i++) {
		bitmask = 1 << i;
		if ((bitmask & blocks) && (uiomux->uios[i]))
			ret |= bitmask;
	}

	return ret;
}

char * uiomux_check_name(struct uiomux *uiomux, uiomux_resource_t blocks)
{
	int i;
	struct uio *uio;
	uiomux_resource_t ret = 0;

	for (i = 0; i < UIOMUX_BLOCK_MAX; i++) {
		uio = uiomux->uios[i];
		if (((1 << i) & blocks) && (uio))
			return uio->dev.name;
	}

	return NULL;
}
