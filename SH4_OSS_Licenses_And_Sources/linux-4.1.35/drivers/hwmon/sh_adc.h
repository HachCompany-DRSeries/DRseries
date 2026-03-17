/*
 * sh_adc.h
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
 **/

#ifndef __DRIVERS_HWMON_SH_ADC_H
#define __DRIVERS_HWMON_SH_ADC_H

struct sh_adc_platform_data {
	int     voltage_reference; /* in mV */
	u8	ports;
#define SH_ADC_PORT_A	(1<<0)
#define SH_ADC_PORT_B	(1<<1)
#define SH_ADC_PORT_C	(1<<2)
#define SH_ADC_PORT_D	(1<<3)
};

#endif /* __DRIVERS_HWMON_SH_ADC_H */
