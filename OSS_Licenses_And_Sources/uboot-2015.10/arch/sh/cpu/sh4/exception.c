/*
 * exception.c
 *
 * Copyright (c) 2010 by emtrion GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Author:      Markus Pietrek
 *
 **/

#include <common.h>

static void exception_reboot(const char *msg)
{
	printf("Exception: %s\n", msg);
	serial_tx_flush();
	reset_cpu(0);
}

void exception_nmi(void)
{
	exception_reboot("NMI");
}

void exception_illegal(void)
{
	exception_reboot("Illegal");
}

void exception_tlb(void)
{
	exception_reboot("TLB");
}
