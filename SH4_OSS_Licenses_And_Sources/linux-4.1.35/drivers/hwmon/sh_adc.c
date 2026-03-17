/*
 * sh_adc.c
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
 * Description: Driver for the ADC of the SH (SH7723). Only single
 *              mode is supported
 *
 **/

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/interrupt.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/wait.h>
#include <asm/io.h>

#include "sh_adc.h"

#if !defined(IRQF_DISABLED)
#define IRQF_DISABLED 0x00
#endif

/* A/D Converter registers */
#define ADDR(ch)	(0x00+((ch)&0x7)*2)
#define ADCSR		0x08

#define ADCSR_ADF	(1 << 15)
#define ADCSR_ADIE	(1 << 14)
#define ADCSR_ADST	(1 << 13)
#define ADCSR_CKS_545	(2 <<  6)
#define ADCSR_MULTI_SINGLE (0 << 4)
#define ADCSR_CH(ch)    ((ch)&0x7)

#define MAX_ANALOG_CHANNELS	4

struct sh_adc_priv {
	struct device          *hwmon_dev;
	struct platform_device *pdev;
	wait_queue_head_t       waitq;
        void __iomem  *base;
        struct clk    *clk;
        struct mutex  mutex;
	spinlock_t    lock;
	int           irq;
	int           voltage_reference;

	struct {
		struct attribute      *attrs[MAX_ANALOG_CHANNELS+1+1]; /* analog channels+vref+NULL */
		struct attribute_group group;
	} sysfs;

	volatile char sample_available;
};

/********** low-level I/O **********/

static inline void sh_adc_write(const struct sh_adc_priv *priv, u16 val, int reg)
{
        iowrite16(val, priv->base + reg);
}

static inline u16 sh_adc_read(const struct sh_adc_priv *priv, int reg)
{
        return ioread16(priv->base + reg);
}

static irqreturn_t sh_adc_isr(int irq, void *dev_id)
{
        struct sh_adc_priv *priv = dev_id;
	u16 status;

	spin_lock(&priv->lock);
	status = sh_adc_read(priv, ADCSR);

	if (status & ADCSR_ADF) {
		/* clear end-of-conversion flag */
		sh_adc_write(priv, status & ~ADCSR_ADF, ADCSR);

		priv->sample_available = 1;
		wake_up_interruptible(&priv->waitq);
	}
	spin_unlock(&priv->lock);

	return IRQ_HANDLED;
}

static int sh_adc_get_sample(struct sh_adc_priv *priv, int ch, u16 *sample)
{
	int res;
	unsigned long flags;

	enable_irq(priv->irq);
	clk_enable(priv->clk);

 	spin_lock_irqsave(&priv->lock, flags);
	priv->sample_available = 0;
	/* set to maximum conversion time */
	sh_adc_write(priv,
		     ADCSR_ADIE    |
		     ADCSR_ADST    |
		     ADCSR_CKS_545 |
		     ADCSR_MULTI_SINGLE |
		     ADCSR_CH(ch),
		     ADCSR);
 	spin_unlock_irqrestore(&priv->lock, flags);

	res = wait_event_interruptible(priv->waitq, (priv->sample_available == 1));

 	spin_lock_irqsave(&priv->lock, flags);
	*sample = sh_adc_read(priv, ADDR(ch));
	sh_adc_write(priv, 0, ADCSR);
 	spin_unlock_irqrestore(&priv->lock, flags);

	clk_disable(priv->clk);
	disable_irq(priv->irq);

	return 0;
}

/*********** platform sysfs **********/

static ssize_t sh_adc_sysfs_show_vref(struct device *dev, struct device_attribute *da,
				      char *buf)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sh_adc_priv *priv     = platform_get_drvdata(pdev);
	int vref;

	mutex_lock(&priv->mutex);
	vref = priv->voltage_reference;
	mutex_unlock(&priv->mutex);

	return sprintf(buf, "%i mV\n", vref);
}

static ssize_t sh_adc_sysfs_set_vref(struct device *dev, struct device_attribute *devattr,
				     const char *buf, size_t count)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sh_adc_priv *priv     = platform_get_drvdata(pdev);
	int vref = simple_strtol(buf, NULL, 10);

	mutex_lock(&priv->mutex);
	priv->voltage_reference = vref;
	mutex_unlock(&priv->mutex);

	return count;
}

static ssize_t sh_adc_sysfs_show_anx(struct device *dev, struct device_attribute *da,
				     char *buf)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sh_adc_priv *priv     = platform_get_drvdata(pdev);
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	int res;
	u16 sample;
	u16 vin;

	/* we support only single mode. No other channel can be read at the same
	 * time */
	mutex_lock(&priv->mutex);
	res = sh_adc_get_sample(priv, attr->index, &sample);
	/* convert the 10bit digital value to mV */
	vin = (sample*priv->voltage_reference)/65536;
	mutex_unlock(&priv->mutex);

	return res ? res : sprintf(buf, "%u mV\n", vin);
}

#define MK(index) SENSOR_ATTR(an##index, S_IRUGO, sh_adc_sysfs_show_anx, NULL, index)
static struct sensor_device_attribute sh_adc_dev_attr[MAX_ANALOG_CHANNELS] = {
	MK(0),
	MK(1),
	MK(2),
	MK(3),
};
#undef MK
SENSOR_DEVICE_ATTR(vref, S_IRUGO | S_IWUSR, sh_adc_sysfs_show_vref, sh_adc_sysfs_set_vref, 0);

/*********** platform probe **********/

static int __init sh_adc_probe(struct platform_device *pdev)
{
        struct sh_adc_priv *priv;
        struct sh_adc_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct resource *res;
	struct attribute **attr;
        int error = 0;
	int i;
	unsigned long flags;

        if (!pdata) {
                dev_err(&pdev->dev, "Need platform data\n");
                error = -EINVAL;
                goto error;
        }

        priv = kzalloc(sizeof(*priv), GFP_KERNEL);
        if (!priv) {
                dev_err(&pdev->dev, "cannot get memory\n");
                error = -ENOMEM;
                goto error;
        }
        priv->pdev = pdev;
        platform_set_drvdata(pdev, priv);

        mutex_init(&priv->mutex);
	spin_lock_init(&priv->lock);
	init_waitqueue_head(&priv->waitq);

	priv->voltage_reference = pdata->voltage_reference;

	/* register resources */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
        if (!res) {
                dev_err(&pdev->dev, "cannot get platform mem resources\n");
                error = -EINVAL;
                goto error_mem;
        }

        priv->base = ioremap(res->start, (res->end-res->start)+1);
        if (!priv->base) {
                dev_err(&pdev->dev, "cannot get io memory\n");
                error = -ENOMEM;
                goto error_mem;
        }

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
        if (!res) {
                dev_err(&pdev->dev, "cannot get platform irq resources\n");
                error = -EINVAL;
                goto error_irq;
        }
	priv->irq = res->start;
        error = request_irq(priv->irq, sh_adc_isr, IRQF_DISABLED, dev_name(&priv->pdev->dev), priv);
        if (error) {
                dev_err(&pdev->dev, "cannot get irq\n");
                goto error_irq;
        }

        priv->clk = clk_get(&pdev->dev, "adc0");
        if (IS_ERR(priv->clk)) {
                dev_err(&pdev->dev, "cannot get clock");
                goto error_clock;
        }

	/* register sysfs hooks */
	attr = &priv->sysfs.attrs[0];
	*attr = &sensor_dev_attr_vref.dev_attr.attr;
	attr++;
	/* add SH_ADC_PORT_A/B/C/D when present on board */
	for (i=0; i < ARRAY_SIZE(sh_adc_dev_attr); i++) {
		if (pdata->ports & (1 << i)) {
			*attr = &sh_adc_dev_attr[i].dev_attr.attr;
			attr++;
		}
	}
	*attr = NULL;
	priv->sysfs.group.attrs = priv->sysfs.attrs;

	error = sysfs_create_group(&pdev->dev.kobj, &priv->sysfs.group);
	if (error) {
                dev_err(&pdev->dev, "sysfs failed");
		goto error_sysfs;
	}

	/* register hwmon hooks */
	priv->hwmon_dev = hwmon_device_register(&pdev->dev);
	if (IS_ERR(priv->hwmon_dev)) {
                dev_err(&pdev->dev, "hwmon_device_register failed\n");
		error = PTR_ERR(priv->hwmon_dev);
		goto error_hwmon;
	}

	/* reset A/D */
	clk_enable(priv->clk);
	disable_irq(priv->irq);
	spin_lock_irqsave(&priv->lock, flags);
	sh_adc_write(priv, 0, ADCSR);
	if (sh_adc_read(priv, ADCSR))
		/* clear end of A/D conversion */
		sh_adc_write(priv, ADCSR_ADF, ADCSR);
	spin_unlock_irqrestore(&priv->lock, flags);
	clk_disable(priv->clk);

        dev_info(&pdev->dev, "initialized\n");
        return 0;

error_hwmon:
	sysfs_remove_group(&pdev->dev.kobj, &priv->sysfs.group);

error_sysfs:
        clk_put(priv->clk);

error_clock:
        free_irq(priv->irq, priv);

error_irq:
        iounmap(priv->base);

error_mem:
        kfree(priv);

error:
        return error;
}

static int __exit sh_adc_remove(struct platform_device *pdev)
{
	struct sh_adc_priv *priv = platform_get_drvdata(pdev);

	hwmon_device_unregister(priv->hwmon_dev);
	sysfs_remove_group(&pdev->dev.kobj, &priv->sysfs.group);
        clk_put(priv->clk);
        free_irq(priv->irq, priv);
        iounmap(priv->base);
        kfree(priv);

        platform_set_drvdata(pdev, NULL);

        return 0;
}


static struct platform_driver sh_adc_driver = {
	.probe		= sh_adc_probe,
	.remove		= sh_adc_remove,
	.driver		= {
		.name	= "sh_adc",
		.owner	= THIS_MODULE,
	},
};

static int __init sh_adc_init(void)
{
	return platform_driver_register(&sh_adc_driver);
}

static void __exit sh_adc_exit(void)
{
	platform_driver_unregister(&sh_adc_driver);
}

module_init(sh_adc_init);
module_exit(sh_adc_exit);

MODULE_AUTHOR("Markus Pietrek");
MODULE_DESCRIPTION("SH7723 A/D Converter driver");
MODULE_LICENSE("GPL");

