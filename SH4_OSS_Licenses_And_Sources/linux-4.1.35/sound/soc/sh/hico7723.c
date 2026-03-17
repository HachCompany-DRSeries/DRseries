/*
 * hico7723.c - provides SOC sound interface (initialization on the module)
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
 * Author: Markus Pietrek
 *
 **/

#include <linux/clkdev.h> 
#include <linux/device.h>
#include <linux/module.h>
#include <linux/em_baseboard.h>
#include <linux/em_common.h>
//#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <linux/gpio.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>

#include <asm/clock.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include <cpu/sh7723.h>

#include "../../../arch/sh/boards/mach-hl/include/sh7723.h"
#include "../codecs/tlv320aic23.h"

#include "siu.h"

/* Default 8000Hz sampling frequency */
static unsigned long codec_freq = 8000 * 512;

static int cpu_is_master;
module_param(cpu_is_master, int, 0);
MODULE_PARM_DESC(cpu_is_master, "The CPU is configured as master and the codec as slave. This changes the frequency of the sound by ~6%");

/* External clock, sourced from the codec at the SIUMCKA pin */
static unsigned long siumcka_recalc(struct clk *clk)
{
	return codec_freq;
}


static struct sh_clk_ops siumcka_clk_ops = {
	.recalc = siumcka_recalc,
};

static struct clk siumcka_clk = {
	.ops		= &siumcka_clk_ops,
	.rate		= 0, /* initialised at run-time */
};

static struct clk_lookup *siumcka_lookup; 

static const int hico7723_snd_gpios[] = {
	GPIO_FN_SIUAILR,
	GPIO_FN_SIUAIBT,
	GPIO_FN_SIUAISLD,
	GPIO_FN_SIUAOLR,
	GPIO_FN_SIUAOBT,
	GPIO_FN_SIUAOSLD,
	GPIO_FN_SIUAMCK,
	/* SPDIF not yet supported in linux */
};



static int hico7723_hw_params_tlv320aic23(struct snd_pcm_substream *substream,
					  struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	int ret;
	unsigned int rate = params_rate(params);

	int fmt = SND_SOC_DAIFMT_I2S;
	if (cpu_is_master)
		fmt |= SND_SOC_DAIFMT_CBS_CFS;
	else
		fmt |= SND_SOC_DAIFMT_CBM_CFM;

	ret = snd_soc_dai_set_sysclk(codec_dai, 0, 12000000, SND_SOC_CLOCK_IN);
	if (ret < 0)
		return ret;

	ret = snd_soc_dai_set_fmt(codec_dai, fmt);
	if (ret < 0)
		return ret;

	ret = snd_soc_dai_set_fmt(rtd->cpu_dai, fmt);
	if (ret < 0)
		return ret;

	codec_freq = rate * 512;
	/*
	 * This propagates the parent frequency change to children and
	 * recalculates the frequency table
	 */

	ret = clk_set_rate(&siumcka_clk, codec_freq);
	if (ret < 0)
		return ret;

	dev_dbg(codec_dai->dev, "%s: configure %luHz\n", __func__, codec_freq);

	ret = snd_soc_dai_set_sysclk(rtd->cpu_dai, SIU_CLKA_EXT,
				     codec_freq / 2, SND_SOC_CLOCK_IN);

	return ret;
}

static int hico7723_hw_free_tlv320aic23(struct snd_pcm_substream *substream)
{
	return 0;
}

static struct snd_soc_ops hico7723_dai_ops_tlv320aic23 = {
	.hw_params = hico7723_hw_params_tlv320aic23,
	.hw_free   = hico7723_hw_free_tlv320aic23,
};

static const struct snd_soc_dapm_widget hico7723_dapm_widgets_tlv320aic23[] = {
	SND_SOC_DAPM_HP("Headphone Jack", NULL),
	SND_SOC_DAPM_LINE("Line In", 	  NULL),
	SND_SOC_DAPM_MIC("Mic Jack", 	  NULL),
};

static const struct snd_soc_dapm_route audio_map_tlv320aic23[] = {
	{ "Headphone Jack", NULL, "LHPOUT"},
	{ "Headphone Jack", NULL, "RHPOUT"},
	{ "LLINEIN", NULL, "Line In"},
	{ "RLINEIN", NULL, "Line In"},
	{ "MICIN", NULL, "Mic Jack"},

};


static int hico7723_init(struct snd_soc_pcm_runtime *rtd)
{
	int ret;

	/* register and configure pins */
	ret = gpio_fn_request_array(hico7723_snd_gpios, ARRAY_SIZE(hico7723_snd_gpios), "sound");
	if (ret<0) {
		printk(KERN_INFO "error on gpio_fn_request_array!\n");
		goto error_gpio;
	}

	ret = clk_register(&siumcka_clk);
	if (ret < 0) {
		printk(KERN_INFO "error on clk_register!\n");
		goto error_clk;
	}

	siumcka_lookup = clkdev_alloc(&siumcka_clk, "siumcka_clk", NULL);
	if (!siumcka_lookup) {
		ret = -ENOMEM;
		goto error_clk;
	}
	clkdev_add(siumcka_lookup);
	
	return 0;
	
error_clk:
	clk_unregister(&siumcka_clk);
	gpio_fn_free_array(hico7723_snd_gpios, ARRAY_SIZE(hico7723_snd_gpios));
error_gpio:
	printk(KERN_INFO "Any error: %d!\n", ret);
	return ret;
}


static struct snd_soc_dai_link hico7723_dai_tlv320aic23[] = {
  {
	.name  		= "tlv320aic23",
	.stream_name 	= "TLV320AIC23",
	.cpu_dai_name   = "siu-pcm-audio",
	.codec_dai_name = "tlv320aic23-hifi",
	.platform_name  = "siu-pcm-audio",
	.codec_name = "tlv320aic23-codec.0-001b", 
	.dai_fmt = SND_SOC_DAIFMT_I2S |
		   SND_SOC_DAIFMT_NB_IF |
		   SND_SOC_DAIFMT_CBS_CFS, 
	.ops = &hico7723_dai_ops_tlv320aic23,
	.init = hico7723_init,

  },
};

/* HiCO.DIMM7723 audio machine driver */
static struct snd_soc_card snd_soc_hico7723_tlv320aic23 = {
	.name 		= "HiCODIMM7723",
	.owner          = THIS_MODULE,
	.dai_link 	= hico7723_dai_tlv320aic23,
	.num_links 	= ARRAY_SIZE(hico7723_dai_tlv320aic23),

 	.dapm_widgets = hico7723_dapm_widgets_tlv320aic23,
	.num_dapm_widgets = ARRAY_SIZE(hico7723_dapm_widgets_tlv320aic23),
	.dapm_routes = audio_map_tlv320aic23,
	.num_dapm_routes = ARRAY_SIZE(audio_map_tlv320aic23),

};

static int snd_hico7723_probe(struct platform_device *pdev)
{
	int ret = 0;

	snd_soc_hico7723_tlv320aic23.dev = &pdev->dev;
	ret = devm_snd_soc_register_card(&pdev->dev, &snd_soc_hico7723_tlv320aic23);
	if (ret)
		dev_err(&pdev->dev, "snd_soc_register_card failed %d\n", ret);

	return ret;
}

static struct platform_driver snd_hico7723_driver = {
	.driver = {
		.name = "hico7723-audio",
	},
	.probe = snd_hico7723_probe,
};

module_platform_driver(snd_hico7723_driver);


MODULE_AUTHOR("Frank Reither, emtrion GmbH");
MODULE_DESCRIPTION("ALSA SoC HiCO.DIMM7723");
MODULE_LICENSE("GPL v2");
