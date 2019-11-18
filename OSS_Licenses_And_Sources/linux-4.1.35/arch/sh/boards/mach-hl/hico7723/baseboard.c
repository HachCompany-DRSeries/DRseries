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
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/em_baseboard.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/i2c/pcf857x.h>
#include <linux/i2c-gpio-sh.h>
#include <linux/gpio_poweroff.h>
#include <linux/em_device_setup.h>
#include <linux/em_common.h>
#include <cpu/sh7723.h>
#include <asm/irq.h>
#include <asm/io.h>

#include "hico7723.h"
#include "sh7723.h"

static int last_gpio;

/* we have split it up in different files to not loose the overview */

#include "baseboard_io_sh_adc.c"
#include "baseboard_io_tpu.c"
#include "baseboard_io_camera.c"
#include "baseboard_io_display.c"
#include "baseboard_io_spi.c"
// #include "baseboard_io_sd_mmc.c"
// #include "baseboard_io_okican.c"

#define INFO(board) pr_info("Configured for baseboard: %s\n", board)

/*********** Baseboard ***********/
static int __init hico7723_register_hachlange(void)
{      
  int i;
  static struct gpio_poweroff_platform_data gpio_poweroff_pdata = {
    .active_level = 0,
  };
  static struct platform_device gpio_poweroff_device = {
    .name = "gpio_poweroff",
    .id  = 0,
    .dev = {
      .platform_data = &gpio_poweroff_pdata,
    },
  };
  static struct pcf857x_platform_data pcf8574_info_dimmbase; /* will be initialised at runtime */
  
  static struct i2c_board_info baseboard_i2c_devices[] = {
    {
      I2C_BOARD_INFO("pcf8574",     0x39),
	    .platform_data = &pcf8574_info_dimmbase,
    },
    {
      I2C_BOARD_INFO("tlv320aic23", 0x1B),
      
    },
  };
  
  
  static struct spi_board_info hico_spi_devices[] = {
    {
      .modalias = "ltc2440_adc",
      .max_speed_hz = 8333333,
      .bus_num = 0,
    },
  };
  
  static const struct device_setup base_devices[] __initdata = {
    EM_MKS(hico7723_setup_pwm_tpu),
    EM_MKS(hico7723_setup_camera),
    EM_MKS(hico7723_setup_adc),
    EM_MKS(hico7723_setup_spi),
    EM_MKS(hico7723_setup_display_i2c),
  };
  
  static struct platform_device* baseboard_devices[] = {
    &gpio_poweroff_device,
  };
  int res;
  
  pcf8574_info_dimmbase.gpio_base = last_gpio;
  last_gpio += 8;  
  gpio_poweroff_pdata.gpio = pcf8574_info_dimmbase.gpio_base+5;
  
/* enable clock for camera */
  res = gpio_request_one(GPIO_PTZ7, GPIOF_OUT_INIT_HIGH, "en-24mhz");
  gpio_free(GPIO_PTZ7);
 
/* enable clock for sound chip */
  res = gpio_request_one(GPIO_PTZ5, GPIOF_OUT_INIT_HIGH, "en-12mhz");
  gpio_free(GPIO_PTZ5);

  
  __raw_writew(0x0000, MSELCRA);
/* up to U-Boot 2009.08em4 PTK5 was configured always as input.
   But on Rev. 3a or later, we need it as special function */
  __raw_writew(__raw_readw(PORT_PKCR) & ~(3<<(5*2)), PORT_PKCR); 
 
  
  pr_info("%s: i2c_register_board_info...\n",__FUNCTION__);
  res = i2c_register_board_info(0, baseboard_i2c_devices, ARRAY_SIZE(baseboard_i2c_devices));
  
  pr_info("%s: register base_devices...\n",__FUNCTION__);
  if (!res) {
    for (i=0; i < ARRAY_SIZE(base_devices); i++) {
      pr_info("%s: register base_device #%d (%s)\n",__FUNCTION__,i,base_devices[i].name);
      res = base_devices[i].fn();
      if (res<0) {
	pr_err("setup failed at function %s\n", base_devices[i].name);
	break;
      }
    }
  }
  
  res = spi_register_board_info(hico_spi_devices, ARRAY_SIZE(hico_spi_devices));
  if (res<0) {
    pr_err("setup failed for spi ltc2440_adc\n");
  }

  
  if (!res){
    pr_info("%s: platform_add_devices...\n",__FUNCTION__);
    res = platform_add_devices(baseboard_devices, ARRAY_SIZE(baseboard_devices));
  }
  else
    pr_err("ERR: %s: DON'T CALL platform_add_devices!\n",__FUNCTION__);

  // hico7723_usbh_oc_connected = 1;
  return res;
}

int __init baseboard_register(void)
{
	int res;

	res = hico7723_register_hachlange(); 
	return res;
}

void __init baseboard_init_irq(void)
{
}
