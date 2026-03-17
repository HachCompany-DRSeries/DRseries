/*
 * arch/sh/boards/mach-emtrion/hico7723/priority.c
 *
 * Copyright (c) 2009 by emtrion GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Author:      Markus Pietrek
 * Description: Adjustment of the SuperH bus priorities
 * References:
 *         [1] file://W:/Datenblaetter/Renesas/SH-Mobile/SH7723/SH7723_Rev1.00_Eng_edi1958.pdf
w *
 **/

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <asm/io.h>

#include "gpr_priority.h"

#define GPR_PRIO_MASK 0x0000003F

typedef struct {
        const char *name;
        int reg;
} gpr_modules_t;

#define MK(_name,_reg) { .name=_name, .reg=_reg }
static const gpr_modules_t gpr_modules[] = {
        MK( "shx",   0xFF800020 ),
        MK( "dbg",   0xFF800028 ),
        MK( "dmac0", 0xFF800030 ),
        MK( "dmac1", 0xFF800038 ),
        MK( "icb",   0xFF800040 ),
        MK( "lmb",   0xFF800048 ),
        MK( "2dgc",  0xFF800050 ),
        MK( "2dg",   0xFF800058 ),
        MK( "atapi", 0xFF800060 ),
};
#undef MK

void __init gpr_priority_set(const char *name, int prio)
{
        int i = 0;

        while (i<ARRAY_SIZE(gpr_modules)) {
                const gpr_modules_t *gpr_module = &gpr_modules[i];

                if (!strcmp(gpr_module->name, name)) {
                        prio &= GPR_PRIO_MASK;
                        printk( KERN_INFO "Setting GPR Priority of %s to 0x%02x\n", name, prio );
                        __raw_writel(prio, gpr_module->reg);
                        return;
                }

                i++;
        }

        printk( KERN_NOTICE "GPR Priority %s not known\n", name );
}

static int __init gpr_priority_setup(char *options)
{
        char *module = options;

        while (module && *module) {
                char *next = strchr(module, ':');
                if (next != NULL)
                        *next++ = '\0';

                if (strlen(module)) {
                        char *val = strchr(module, '=');
                        if (val != NULL) {
                                int prio;
                                *val++ = '\0';
                                prio = simple_strtoul(val, NULL, 0);
                                gpr_priority_set(module, prio);
                        } else
                                printk( KERN_NOTICE "GPR Priority %s wrong format\n", module );
                }
                module = next;
        }

        return 1;
}
__setup("gpr_priority=", gpr_priority_setup);
