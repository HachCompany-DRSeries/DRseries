/*
 * HiCO_SH7723
 *
 * Copyright (C) 2016 by emtrion GmbH
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/em_common.h>
#include <asm/machvec.h>

int gpio_fn_request_array(const int gpios[], int size, const char *label)
{
        int i;

        for (i=0; i < size; i++) {
                if (gpio_request(gpios[i], label)) {
                        /* print error but continue booting */
                        pr_err("GPIO %u (%s) already reserved.\n", gpios[i], label );
			return -EBUSY;
		}
        }

	return 0;
}
EXPORT_SYMBOL(gpio_fn_request_array);

int gpio_fn_free_array(const int gpios[], int size)
{
        int i;

        for (i=0; i < size; i++)
		gpio_free(gpios[i]);

	return 0;
}
EXPORT_SYMBOL(gpio_fn_free_array);
