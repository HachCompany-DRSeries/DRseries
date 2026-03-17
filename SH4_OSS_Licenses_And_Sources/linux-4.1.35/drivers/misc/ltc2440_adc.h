/*
 * ltc2440_adc.h
 *
 * Copyright (c) 2010 by emtrion GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Author:      Markus Pietrek
 *
 **/

#ifndef LTC2440_ADC_H
#define LTC2440_ADC_H

#define LTC_MAGIC 'l'

#define LTC_IO_RESTART	  _IO(LTC_MAGIC, 1)
#define LTC_IO_SET_SPEED  _IOW(LTC_MAGIC, 2, int)

#endif /* LTC2440_ADC_H */
