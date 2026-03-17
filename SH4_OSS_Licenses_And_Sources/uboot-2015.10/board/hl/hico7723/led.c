/*
 * board/emtrion/hico7723/led.c
 *
 * Copyright (c) 2008 by emtrion GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Author:      Markus Pietrek
 * Revision:    $Revision$
 * Description: Provides led functions
 **/

#include <common.h>
#include <status_led.h>
#include <asm/processor.h>
#include <asm/io.h>

void __led_toggle(led_id_t mask)
{
        iormw8(PJDR, ^ mask);
}

void __led_init(led_id_t mask, int state)
{
        /* nothing to do */
}

void __led_set(led_id_t mask, int state)
{
        if (state == STATUS_LED_ON)
                iormw8(PJDR, | mask);
        else
                iormw8(PJDR, & ~mask);
}

