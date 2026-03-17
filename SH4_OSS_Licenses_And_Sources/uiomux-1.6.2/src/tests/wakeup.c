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

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <pthread.h>

#include <uiomux/uiomux.h>

#include "uiomux_tests.h"

#define VBSRR (0xb4)
#define VSAYR (0x18)
#define VESWR (0x10)
#define VESSR (0x14)
#define VEDWR (0x30)
#define VDAYR (0x34)
#define VEIER (0xA0)
#define VESTR (0x00)
#define VEVTR (0xA4)

#define INTERRUPT_COUNT 10

static UIOMux *uiomux;

void
write_reg(void *reg_base, unsigned long offset, unsigned long value) {
	*(unsigned long *)((char *)reg_base + offset) = value;
}

void *
interrupt_handler_thread(void *arg) {

	void **args;
	int *handled;
	int *exit_flag;
	void **reg_base;
	int int_count = 0;

	args = (void **)arg;
	handled = (int *)args[0];
	exit_flag = (int *)args[1];
	reg_base = (void **)args[2];

	while (!*exit_flag) {
		if (uiomux_sleep(uiomux, UIOMUX_SH_VEU) < 0) {
			INFO ("Interrupt handler woken up");
			break;
		}
		printf("Handling an interrupt #%d\n", int_count++);
		write_reg(*reg_base, VEVTR, 0x100);
		*handled = 1;
	}

	INFO("Safely exiting interrupt handler");
	return NULL;
}

/*generate_veu_interrupt*/
/*Not the prettiest method, but will generate an interrupt from
  the VEU.*/
void
generate_veu_interrupt(void *reg_base, unsigned long mem_region) {
	write_reg(reg_base, VBSRR, 0x100);
	write_reg(reg_base, VSAYR, mem_region);
	write_reg(reg_base, VESWR, 4);
	write_reg(reg_base, VESSR, 4 << 16 | 4);
	write_reg(reg_base, VDAYR, mem_region + 0x100);
	write_reg(reg_base, VEDWR, 4);
	write_reg(reg_base, VEIER, 1);
	write_reg(reg_base, VESTR, 1);
}

int
main() {

	int ret;

	void *veu_register_base;
	unsigned long veu_phys_memory_base;
	int i;
	pthread_t tid;

	volatile int handled = 0, exit_flag = 0;
	void **thread_args = calloc(3, sizeof (void *));
	thread_args[0] = (void *)&handled;
	thread_args[1] = (void *)&exit_flag;
	thread_args[2] = (void *)&veu_register_base;

	INFO ("Opening UIOMux for VEU");
	uiomux = uiomux_open();

	if (uiomux == NULL)
		FAIL ("Opening UIOMux");

	ret = uiomux_get_mmio (uiomux, UIOMUX_SH_VEU,
                NULL, NULL, &veu_register_base);
	if (!ret)
		FAIL ("Get MMIO region");

	ret = uiomux_get_mem (uiomux, UIOMUX_SH_VEU,
                &veu_phys_memory_base, NULL, NULL);
	if (!ret)
		FAIL ("Get MEM region");

	INFO ("Creating interrupt handler thread");
	pthread_create(&tid, NULL, interrupt_handler_thread, thread_args);

	for (i=0; i < INTERRUPT_COUNT; i++) {
		printf("Generating interrupt %d\n", i);
		generate_veu_interrupt(veu_register_base,
			veu_phys_memory_base);
		while (!handled) {
			usleep(10);
		}
		handled = 0;
	}
	INFO("Going to sleep for a bit");
	sleep (1);
	INFO("Waking up sleeping interrupt handler");
	exit_flag = 1;
	uiomux_wakeup(uiomux, UIOMUX_SH_VEU);
	pthread_join(tid, NULL);

	free(thread_args);

	INFO ("Closing UIOMux");
	ret = uiomux_close(uiomux);
	if (ret != 0)
		FAIL ("Closing UIOMux");

	return 0;
}
