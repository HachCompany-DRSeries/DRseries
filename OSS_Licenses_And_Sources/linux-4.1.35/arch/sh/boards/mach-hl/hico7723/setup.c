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
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/mtd/physmap.h>
#include <linux/mtd/partitions.h> 
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/mtd/sh_flctl.h>
#include <linux/gpio.h>
#include <linux/usb/r8a66597.h>
#include <linux/em_baseboard.h>
#include <linux/em_device_setup.h>
#include <linux/em_module.h>
#include <linux/em_common.h>
#include <linux/em_firmware.h>
#include <video/sh_mobile_lcdc.h>
#include <linux/videodev2.h> 
#include <linux/dm9000.h> 
#include <cpu/sh7723.h>
#include <asm/setup.h>
#include <asm/pgtable.h>
#include <asm/clock.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/machvec.h>
#include <asm/irq.h>
#include <asm/hico7723.h>
#include <linux/memory.h>
#include <sound/simple_card.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>

#include "hico7723.h"
#include "sh7723.h"
#include "../arch/sh/drivers/gpr_priority.h"

#include "../display.h"
#include "../bootlogo.h"

#define LCD_ON 		GPIO_PTH2
#define LCD_DISP	GPIO_PTH4
#define USBH_OC		GPIO_PTF6
#define USBH_PEN	GPIO_PTF5
#define USBF_WAKE 	GPIO_PTF0
#define SOFTRESET	GPIO_PTF7
#define LED_D2_GREEN	GPIO_PTJ7
#define LED_D2_RED	GPIO_PTJ5
#define NAND_WP         GPIO_PTK7
#define EN_SOUND        GPIO_PTD3

#define LDSA1R          0x0430
#define LDMLSR          0x0438
#define LDSR            0x046C
#define LDRCNTR		0x0478

#define LDRCNTR_MRS_B	0x00000002
#define LDSR_MRS_B      0x00000100

/* using NOR and watchdog doesn't work reliable on revision <r5a, therefore it can be disabled */
EM_CMDLINE_FLAG(hico7723_skip_nor);
EM_CMDLINE_FLAG(hico7723_skip_nand);

static void hico7723_lcd_on(void)
{
        gpio_set_value(LCD_ON, 1);
}

static void hico7723_lcd_off(void)
{
        gpio_set_value(LCD_ON, 0);
}

static void hico7723_usb_power(int port, int power)
{
        gpio_set_value(USBH_PEN, power);
}

enum {
        BOOTLOADER_CONFIGURED,
};

static const struct fb_videomode hico7723_lcdc_modes[] = {
	{
		.name = "Default",
		.xres = 800,
		.yres = 480,
		.left_margin = 40,
		.right_margin = 40,
		.hsync_len = 48,
		.upper_margin = 29,
		.lower_margin = 13,
		.vsync_len = 3,
		.sync = 0, /* hsync and vsync are active low */
	},
};


static struct sh_mobile_lcdc_info lcdc_info = {
	.clock_source = LCDC_CLK_EXTERNAL,
	.ch[0] = {
		.chan = LCDC_CHAN_MAINLCD,
		.fourcc = V4L2_PIX_FMT_RGB565, 
		.interface_type = RGB18,
		.clock_divider = 1,
		.lcd_modes = hico7723_lcdc_modes,
		.num_modes = ARRAY_SIZE(hico7723_lcdc_modes),
	}
};

static struct resource lcdc_resources[] = {
	[0] = {
		.name	= "LCDC",
		.start	= LCDC_BASE, /* P4-only space */
		.end	= LCDC_END,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= evt2irq(0x580),
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device lcdc_device = {
	.name		= "sh_mobile_lcdc_fb",
	.num_resources	= ARRAY_SIZE(lcdc_resources),
	.resource	= lcdc_resources,
	.dev		= {
		.platform_data	= &lcdc_info,
	},
};

/*
 * internal datapath is fixed 16 bit
 */
static struct dm9000_plat_data dm9000_platdata = {
	.flags	= DM9000_PLATF_16BITONLY,
};
 
static struct resource dm9000_resources[] = {
	[0] = {
		.name   = "address area",
		.start	= 0x16000000,
		.end	= 0x16000001,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
	        .name   = "data area",
		.start	= 0x16000002,
		.end	= 0x16000003,
		.flags	= IORESOURCE_MEM,
	},
	[2] = {
		.start	= evt2irq(IRQEVT_IRQ5),
		.end	= evt2irq(IRQEVT_IRQ5),
		.flags	= IORESOURCE_IRQ | IRQF_TRIGGER_RISING,
	},
};


/*********** I2C ***********/

static struct i2c_board_info hico7723_i2c_devices[] = {
        {	
	        I2C_BOARD_INFO("ds1337",      0x68),
	  
	},
        {
                I2C_BOARD_INFO("ar1020",     0x4D),
                .irq = evt2irq(IRQEVT_IRQ7)
	},
};

/*********** Ethernet ***********/
static struct platform_device dm9000_device = {
	.name		= "dm9000",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(dm9000_resources),
	.resource	= dm9000_resources,
	.dev		= {
		.platform_data = &dm9000_platdata,
	} 
};

/*********** NOR ***********/
static struct physmap_flash_data hico7723_nor_flash_data = {
	.width		= 2,
};

static struct resource hico7723_nor_flash_resources[] = {
	[0] = {
		.name		= "NOR Flash",
		.start		= 0x00000000,
		.end		= 0x00800000,
		.flags		= IORESOURCE_MEM,
	}
};

static struct platform_device hico7723_nor_flash_device = {
	.name		= "physmap-flash",
	.resource	= hico7723_nor_flash_resources,
	.num_resources	= ARRAY_SIZE(hico7723_nor_flash_resources),
	.dev		= {
		.platform_data = &hico7723_nor_flash_data,
	},
};

/*********** NAND ***********/
/* FLCTL */
static struct resource nand_flash_resources[] = {
	[0] = {
		.start	= 0xa4530000,
		.end	= 0xa45300ff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= evt2irq(0x0d80), /* flstei: status error irq */
		.end	= evt2irq(0x0d80), /* flstei: status error irq */
		.flags	= IORESOURCE_IRQ,
	}, 
};

static struct sh_flctl_platform_data nand_flash_data = {
	.flcmncr_val	= FCKSEL_E | TYPESEL_SET | NANWF_E,
	.use_holden     = 0,
	.has_hwecc	= 0, /* disable HWECC as it is 4-Byte ECC and different
                              * to U-Boot/Linux SWECC */
};

static struct platform_device nand_flash_device = {
	.name		= "sh_flctl",
	.resource	= nand_flash_resources,
	.num_resources	= ARRAY_SIZE(nand_flash_resources),
	.dev		= {
		.platform_data = &nand_flash_data,
	},
};

static int __init hico7723_setup_lcd(void)
{
	static const int gpios[] __initdata = {
		GPIO_FN_LCDVCPWC,
		GPIO_FN_LCDVEPWC,
		GPIO_FN_LCDVSYN,
		GPIO_FN_LCDHSYN,
		GPIO_FN_LCDDCK,
		GPIO_FN_LCDRD,
		GPIO_FN_LCDD23,
		GPIO_FN_LCDD22,
		GPIO_FN_LCDD21,
		GPIO_FN_LCDD20,
		GPIO_FN_LCDD19,
		GPIO_FN_LCDD18,
		GPIO_FN_LCDD17,
		GPIO_FN_LCDD16,
		GPIO_FN_LCDD15,
		GPIO_FN_LCDD14,
		GPIO_FN_LCDD13,
		GPIO_FN_LCDD12,
		GPIO_FN_LCDD11,
		GPIO_FN_LCDD10,
		GPIO_FN_LCDD9,
		GPIO_FN_LCDD8,
		GPIO_FN_LCDD7,
		GPIO_FN_LCDD6,
		GPIO_FN_LCDD5,
		GPIO_FN_LCDD4,
		GPIO_FN_LCDD3,
		GPIO_FN_LCDD2,
		GPIO_FN_LCDD1,
		GPIO_FN_LCDD0,
	};
	int flag;
	int res;

        /* register and configure pins */
        // return 0; // MS

        res = gpio_fn_request_array(gpios, ARRAY_SIZE(gpios), "lcdc");
	if (res<0)
		return res;

	/* configure LCDLCLK */
	__raw_writew((__raw_readw(PSELA) & ~0x3000) | 0x1000, PSELA);
        /* enable lcd_don for lcd clock */
	flag = GPIOF_OUT_INIT_HIGH;
	res = gpio_request_one(LCD_ON, flag, "lcdc");
	if (res<0)
		return res;

	/* keep lcddisp as it is configured by the bootloader  */
	res = gpio_request(LCD_DISP, "lcddisp");
	return res;
}

#ifndef CONFIG_SH_HICO7723_ENABLE_USB_HIGH_SPEED
int hico7723_usb_disable_highspeed=1;
#else
int hico7723_usb_disable_highspeed=0;
#endif

int hico7723_usbh_oc_connected = 1;

static int __init hico7723_setup_usb(void)
{
        struct device *dev = bus_find_device_by_name(&platform_bus_type, NULL, "r8a66597_hcd.0");
	int res;

        if (dev != NULL) {
                struct r8a66597_platdata *pdata = dev_get_platdata(dev);

                res = gpio_request_one(USBH_PEN, GPIOF_OUT_INIT_HIGH, "usb");
		if (res<0)
			return res;

		pdata->port_power  = hico7723_usb_power;
                pdata->disable_highspeed = hico7723_usb_disable_highspeed;
		if (hico7723_usbh_oc_connected) {
			pdata->oc.poll_for_oc  = 1;
			pdata->oc.active_level = 0;
			pdata->oc.gpio         = USBH_OC;
		}
        }

	return 0;
}

static int __init hico7723_setup_nor(void)
{
	if (hico7723_skip_nor)
		return 0;

	return platform_device_register(&hico7723_nor_flash_device);
}

static int __init hico7723_setup_nand(void)
{
        /* struct flctl is that part of sh_flctl needed for FLCMNCR.
           sh_flctl is quite big, so don't put it on kernel stack */
	struct {
                void __iomem *reg;
        } flctl = {
                .reg = (void __iomem*) nand_flash_resources[0].start,
        };

	if (hico7723_skip_nand)
		return 0;
/* FLCTL */
	gpio_request(GPIO_FN_FCE, NULL);
	gpio_request(GPIO_FN_NAF7, NULL);
	gpio_request(GPIO_FN_NAF6, NULL);
	gpio_request(GPIO_FN_NAF5, NULL);
	gpio_request(GPIO_FN_NAF4, NULL);
	gpio_request(GPIO_FN_NAF3, NULL);
	gpio_request(GPIO_FN_NAF2, NULL);
	gpio_request(GPIO_FN_NAF1, NULL);
	gpio_request(GPIO_FN_NAF0, NULL);
	gpio_request(GPIO_FN_FCDE, NULL);
	gpio_request(GPIO_FN_FOE, NULL);
	gpio_request(GPIO_FN_FSC, NULL);
	gpio_request(GPIO_FN_FWE, NULL);
	gpio_request(GPIO_FN_FRB, NULL);

        /* reuse nand timing configured already by bootloader to support
         * different chip types which the linux kernel doesn't need to be aware of */
        nand_flash_data.flcmncr_val =
                TYPESEL_SET |
                NANWF_E |
                (readl(FLCMNCR((&flctl))) & ( QTSEL_E | FCKSEL_E ));

	gpio_request(GPIO_PTK7, "nand_wp"); /* Nand write protect */
	gpio_direction_output(GPIO_PTK7, 1);
	gpio_export(GPIO_PTK7, 0);

	return platform_device_register(&nand_flash_device);
}

static int __init hico7723_setup_scif(void)
{
        static const int gpios[] = {
		GPIO_FN_SCIF1_PTS_TXD,
		GPIO_FN_SCIF1_PTS_RXD,
		GPIO_FN_SCIF2_PTT_TXD,
		GPIO_FN_SCIF2_PTT_RXD,
		GPIO_FN_SCIF3_PTS_TXD,
		GPIO_FN_SCIF3_PTS_RXD,
		GPIO_FN_SCIF3_PTS_CTS,
		GPIO_FN_SCIF3_PTS_RTS,
		GPIO_FN_SCIF4_PTE_TXD,
		GPIO_FN_SCIF4_PTE_RXD,
		GPIO_FN_SCIF5_PTE_TXD,
		GPIO_FN_SCIF5_PTE_RXD,
        };
        struct device *dev = bus_find_device_by_name(&platform_bus_type, NULL, "sh-sci.0");

	if (dev != NULL)
		/* HiCO.DIMM7723 has no sh-sci.0/ttySC0 connected, so remove it.
		 * The device node might still be visible but can't be opened*/
		platform_device_unregister(to_platform_device(dev));

	return gpio_fn_request_array(gpios, ARRAY_SIZE(gpios), "scif");
}


/************************************************************************************************
 * 
 *  setting up misc devices like LEDs and so on
 * 
 ************************************************************************************************/
static int __init hico7723_setup_misc(void)
{
	static struct gpio gpios[] = {
		{ .gpio = SOFTRESET,    .flags = GPIOF_OUT_INIT_LOW,  "softreset" },
//		{ .gpio = EN_SOUND,     .flags = GPIOF_OUT_INIT_HIGH,  "enable_sound" },
#ifdef CONFIG_BUSY_LEDS
		{ .gpio = LED_D2_GREEN, .flags = GPIOF_OUT_INIT_HIGH, "led d2 green" },
		{ .gpio = LED_D2_RED,   .flags = GPIOF_OUT_INIT_HIGH, "led d2 red" },
#endif
	};
        static const int gpios_bsc[] = {
		GPIO_FN_WE3_ICIOWR,
		GPIO_FN_WE2_ICIORD,
		GPIO_FN_WAIT,
		GPIO_FN_CS5A_CE2A,
		GPIO_FN_CS5B_CE1A,
		GPIO_FN_CS6A_CE2B,
		GPIO_FN_CS6B_CE1B,
        };
	int res;

#ifdef CONFIG_BUSY_LEDS
        pr_info("Enabling busy LEDs\n");
#endif
        
        /* AU power on */
        gpio_request(GPIO_PTX5, NULL);
        gpio_direction_output(GPIO_PTX5, 1);
        gpio_export(GPIO_PTX5, 1);

	res = gpio_request_array(gpios, ARRAY_SIZE(gpios));
	if (res<0)
		return res;

	res = gpio_fn_request_array(gpios_bsc, ARRAY_SIZE(gpios_bsc), "bsc");
	if (res<0)
		return res;

	res = gpio_request(GPIO_FN_IRQ5, "dm9000");
	if (res<0)
		return res;

	res = gpio_request(GPIO_FN_IRQ6, "ds1337");
	if (res<0)
		return res;
#if 0
	gpio_request(GPIO_PTG0, NULL); /* Backlight Control */
	gpio_direction_output(GPIO_PTG0, 1);
	gpio_export(GPIO_PTG0, 0); 
#endif
	gpio_request(GPIO_PTK7, NULL); /* Nand write protect */
	gpio_direction_output(GPIO_PTK7, 1);
	gpio_export(GPIO_PTK7, 1); 

	return gpio_request(GPIO_FN_IRQ7, "tsc2007");
}



static int __init hico7723_setup_display(void)
{
	int res = 0;
	int anz_param = 0;
	struct sh_mobile_lcdc_info *lcd_mode;
	struct platform_device *dev;
	struct fb_videomode *fb;
	char *tok, *end;
	unsigned long params[12];
        struct sh_mobile_lcdc_chan_cfg *ch;
	
	dev = (struct platform_device *)&lcdc_device;
	lcd_mode = (struct sh_mobile_lcdc_info *)dev->dev.platform_data;
	fb = (struct fb_videomode *)&(lcd_mode->ch[0].lcd_modes[0]);
	ch = (struct sh_mobile_lcdc_chan_cfg *)&lcd_mode->ch[0];
	
	res = display_configured();
	if ( res == -2 ) {
	  printk(KERN_WARNING "Display Configuration not found or empty, using default values\n");
	}
	else {
	  tok = kstrdup(display_configuration, GFP_KERNEL);
	  end = tok;
	  while ( tok != NULL ) {
	    strsep(&end, ",");
	    if ( (anz_param < DISPLAY_PARA_ANZ) ) {
		kstrtol(tok, 10, &params[anz_param]);
	    }
	    else {
	      anz_param = 0; /* parameter list to big, signalling with setting the count val to a unexpected value */
	      break;
	    }
	    anz_param++;
	    tok = end;
	  }
/* when the number of the parameters is ok, the default settings for the display are overwritten now */
	  if ( (anz_param == DISPLAY_PARA_ANZ) && (params[0] != 0) && (params[1] != 0) ) {
	    fb->name         = kstrdup("Conf", GFP_KERNEL);
	    fb->xres         = params[0];
	    fb->yres         = params[1];
	    fb->left_margin  = params[3];
	    fb->right_margin = params[4];
	    fb->hsync_len    = params[5];
	    fb->upper_margin = params[6];
	    fb->lower_margin = params[7];
	    fb->vsync_len    = params[8];
	    fb->sync         = params[9];
	    fb->pixclock     = params[10];
	  }
	  kfree(tok);
	}
	
	printk(KERN_INFO "LCD parameters: xres = %d, yres = %d\n", fb->xres, fb->yres);
	printk(KERN_INFO "                lm = %d, rm = %d, hs = %d\n", fb->left_margin, fb->right_margin, fb->hsync_len);
	printk(KERN_INFO "                um = %d, lm = %d, vhs = %d, sync = %d\n", fb->upper_margin, fb->lower_margin, fb->vsync_len, fb->sync);
	printk(KERN_INFO "PTH4 (LCD_DISP) already configured by the bootloader\n");

	res = platform_device_register(&lcdc_device);
	if (res < 0)
	    goto error;
	return 0;

error:
        return res;
}

static int __init hico7723_setup_i2c(void)
{
	int res;

	res = i2c_register_board_info(0, hico7723_i2c_devices,
				      ARRAY_SIZE(hico7723_i2c_devices));
	return res;
}

int __init hico7723_devices_setup(struct device_setup devices[], int len)
{
	int res;
	int i;

	for (i=0; i < len; i++) {
		res = devices[i].fn();
		if (res<0) {
			pr_err("setup failed at function %s\n", devices[i].name);
			return res;
		}
	}

	return 0;
}


static struct asoc_simple_card_info siu_tlv320aic23_info = {
	.name		= "tlv320aic23",
	.card		= "hico7723-audio",
	.codec		= "tlv320aic23-codec.0-001b",
	.platform	= "siu-pcm-audio",
	.daifmt		= SND_SOC_DAIFMT_I2S |
		   SND_SOC_DAIFMT_NB_IF |
		   SND_SOC_DAIFMT_CBS_CFS,
};

static struct platform_device siu_tlv320aic23_device = {
	.name	= "hico7723-audio",
	.dev	= {
		.platform_data	= &siu_tlv320aic23_info,
	},
};

/*********** all devices ***********/

static struct platform_device* hico7723_devices[] = {
	&dm9000_device,
	&siu_tlv320aic23_device,
};

static int __init hico7723_init(void)
{
        int res;
	int i;

	static const struct device_setup module_devices[] __initdata = {
		EM_MKS(hico7723_setup_nor),
		EM_MKS(hico7723_setup_misc),
		EM_MKS(hico7723_setup_lcd),
		EM_MKS(hico7723_setup_display),
		EM_MKS(hico7723_setup_usb),
		EM_MKS(hico7723_setup_i2c),
		EM_MKS(hico7723_setup_scif),
		EM_MKS(hico7723_setup_nand),
	};
	
	pr_info("%s: platform_add_devices...\n",__FUNCTION__);
	res = platform_add_devices(hico7723_devices, ARRAY_SIZE(hico7723_devices));
        if (res < 0) {
	  pr_err("%s: platform devices couldn't be added\n", __FUNCTION__ );
	  goto out;
        }
	
	pr_info("%s: baseboard_register...\n",__FUNCTION__);
	res = baseboard_register();
	if (res < 0)
	  goto out;
	
	for (i=0; i < ARRAY_SIZE(module_devices); i++) {
	  res = module_devices[i].fn();
	  if (res<0) {
	    pr_err("setup failed at function %s\n", module_devices[i].name);
	    if (res < 0)
	      goto out;
	  }
	}
	
out:
        return res;
}
arch_initcall(hico7723_init);

static void __init hico7723_setup(char **cmdline_p)
{
	struct sh_mobile_lcdc_info *lcd_mode;
	struct platform_device *dev;
	struct fb_videomode *fb;
        struct sh_mobile_lcdc_chan_cfg *ch;

	dev = (struct platform_device *)&lcdc_device;
	lcd_mode = (struct sh_mobile_lcdc_info *)dev->dev.platform_data;
	fb = (struct fb_videomode *)&(lcd_mode->ch[0].lcd_modes[0]);
	ch = (struct sh_mobile_lcdc_chan_cfg *)&lcd_mode->ch[0];

	/*
          Set default priorities (based on HiCO.DIMM7723 Rev. 2a with NL6448
          display)

          o Camera(ov772x,not analog) will lock-up/timeout in shcodecs-cap when
            lmb and icb is 0x18.
          o Camera(ov772x) will return blank data when lmb and icb <=0x28
          o When Meram is not active, screen will flicker when lm and icb
            is default 0x18. When Meram is active, lmb and icb can be
            reduced to 0x18
          o 2dg/lcd will lock-up in some examples of df_dok when 2dg<0x2a
        */
        gpr_priority_set("lmb", 0x2A);
        gpr_priority_set("icb", 0x2A);
        gpr_priority_set("2dg", 0x2A);


#ifdef CONFIG_EM_FIRMWARE_BY_BOOTLOADER
	em_firmware_init(FIRMWARE_START, FIRMWARE_SIZE);
#endif

        /* sh_mobile_lcdcfb.c doubles yres_virtual by 3 to support software panning/triple buffering */
        if (bootlogo_init(ch, __raw_readl(LCDC_BASE+LDSA1R), __raw_readl(LCDC_BASE+LDMLSR)*fb->yres*3) == BL_COPIED) {

		/* flicker-free reconfiguration of the framebuffer pointer by using side b registers */
                sh_mobile_lcdc_early_copy_registers_to_sideb(LCDC_BASE);

                /* switch to side B */
		__raw_writel(__raw_readl(LCDC_BASE+LDRCNTR) | LDRCNTR_MRS_B, LCDC_BASE+LDRCNTR);

                /* wait until side B is used by hardware */
                while (1) {
                        if (__raw_readl(LCDC_BASE+LDSR) & LDSR_MRS_B)
                                break;
                }

                /* safely update side A */
		__raw_writel(ch->bootlogo.smem_start, LCDC_BASE+LDSA1R);

                /* switch back to side A */
		__raw_writel(__raw_readl(LCDC_BASE+LDRCNTR) & ~LDRCNTR_MRS_B, LCDC_BASE+LDRCNTR);
        }
}

static void hico7723_reset(void)
{
        /* do a reset by the reset controller */
        gpio_set_value(SOFTRESET, 1);
}


#ifdef CONFIG_BUSY_LEDS
static void hico7723_led_busy(int on)
{
        u8 status = __raw_readb(PORT_PJDR);
        status = on ?
                (status |  PIN_DATA(LED_D2_GREEN)) :
                (status & ~PIN_DATA(LED_D2_GREEN));
        __raw_writeb(status, PORT_PJDR);
}

static void hico7723_led_in_irq(int on)
{
        /* using gpio_set_value is not allowed in do_IRQ
           which is calling us. So do it directly */
        u8 status = __raw_readb(PORT_PJDR);
        status = on ?
                (status |  PIN_DATA(LED_D2_RED)) :
                (status & ~PIN_DATA(LED_D2_RED));
        __raw_writeb(status, PORT_PJDR);
}
#endif

static void __init hico7723_init_irq(void)
{
	baseboard_init_irq();
}





static struct sh_machine_vector mv_hico7723 __initmv = {
	.mv_name	= "HiCODIMM7723",
	.mv_setup	= hico7723_setup,
	.mv_reset	= hico7723_reset,
	.mv_init_irq	= hico7723_init_irq,
#ifdef CONFIG_BUSY_LEDS
	.mv_led_busy	= hico7723_led_busy,
	.mv_led_in_irq  = hico7723_led_in_irq,
#endif
};









#ifdef CONFIG_TOUCHSCREEN_TSC2007
int tsc2007_pen_is_down(void)
{
	return __raw_readb(INTREQ00) & 1;
}
EXPORT_SYMBOL(tsc2007_pen_is_down);
#endif

static int __init hico7723_usb_speed_setup(char *options)
{
        if (options != NULL)
		hico7723_usb_disable_highspeed = strcmp("highspeed", options);

        return 0;
}
early_param("hico7723_usb_speed", hico7723_usb_speed_setup);

static int __init hico7723_nand_bbt_setup(char *options)
{
        if (options == NULL)
                return 0;

        nand_flash_data.has_bbt = simple_strtoul(options, NULL, 0);

        return 1;
}
__setup("nand_bbt=", hico7723_nand_bbt_setup);

static int __init hico7723_nand_hwecc_setup(char *options)
{
        if (options == NULL)
                return 0;

        nand_flash_data.has_hwecc = simple_strtoul(options, NULL, 0);

        return 1;
}
__setup("nand_hwecc=", hico7723_nand_hwecc_setup);
