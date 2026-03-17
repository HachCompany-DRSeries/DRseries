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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA	02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>

#include <uiomux/uiomux.h>

#include "uiomux_tests.h"

#define NR_THREADS 10
#define NR_LOOPS 1000

static int protected_data = 0;

void *
thread_main (void * arg)
{
	UIOMux * uiomux;
	int threadid = * (int *)arg;
	char buf[256];
	int i, j, ret, rc = 0;

	usleep (rand() % 10);

	snprintf (buf, 256, "Opening UIOMux for thread %d", threadid);
	INFO ("%s", buf);
	uiomux = uiomux_open();
	if (uiomux == NULL)
		return (void*) -1;

	for (i = 0; i < NR_LOOPS; i++) {
		uiomux_lock (uiomux, UIOMUX_SH_BEU);

		/* Check the protected data */
		if (protected_data != 0) rc = -1;

		/* Nominal processing */
		for (j = 0; j < 100; j++) protected_data++;
		if (protected_data != 100) rc = -1;
		for (j = 0; j < 100; j++) protected_data--;

		uiomux_unlock (uiomux, UIOMUX_SH_BEU);

		usleep (rand() % 10);
	}

	snprintf (buf, 256, "Closing UIOMux for thread %d", threadid);
	INFO ("%s", buf);
	ret = uiomux_close(uiomux);
	if (ret != 0)
		return (void*) -1;

	return (void*)rc;
}

int
main (int argc, char *argv[])
{
	pid_t pid;
	pthread_t threads[NR_THREADS];
	int i, ret, nums[NR_THREADS];
	char buf[256];

	for (i = 0; i < NR_THREADS; i++) {
		nums[i] = i;
		snprintf (buf, 256, "Creating thread %d", i);
		INFO ("%s", buf);
		pthread_create (&threads[i], NULL, thread_main, &nums[i]);
	}

	for (i = 0; i < NR_THREADS; i++) {
		snprintf (buf, 256, "Joining thread %d", i);
		INFO ("%s", buf);
		pthread_join (threads[i], (void*)&ret);
		if (ret != 0)
			FAIL ("Locking failed");
	}

	exit (0);
}
