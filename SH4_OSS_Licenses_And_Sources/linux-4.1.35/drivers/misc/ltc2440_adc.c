/*
 * ltc2440_adc.c
 *
 * Copyright (c) 2010 by emtrion GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Author:      Markus Pietrek
 * Description: Driver for Linear Technology LTC2440_ADC.
 *              ioctl() and read() behave like introduced in [1]
 * References:
 *   [1] bitbanging SH3 ltc2440.c for linux 2.4.xx
 *
 **/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/spi/spi.h>
#include <asm/uaccess.h>

#include "ltc2440_adc.h"

#define DRIVER_NAME "ltc2440_adc"

#define LTC_EOC		(1 << 31)
#define LTC_DMY		(1 << 30)
#define LTC_SIG		(1 << 29)
#define LTC_STATUS      (LTC_EOC | LTC_DMY | LTC_SIG)
#define LTC_BITS_TO_SHIFT_FOR_MSB_AT_BIT31	3

struct ltc2440_priv {
        dev_t             devt;
        struct cdev       cdev;
        struct class      *class;
	struct spi_device *spi;
        struct device     *dev;
	struct mutex       mutex;
	unsigned int       speed;
};

/**
 * ltc2440_send_speed_and_read_sample - sends an SPI message with speed and gets the sample
 * @return: 0 when OK
 */
static int ltc2440_send_speed_and_read_sample(struct ltc2440_priv *priv, u32* sample)
{
	u32 speed;
	int res;
	struct spi_transfer t = {
		.tx_buf = &speed,
		.rx_buf = sample,
		.len = 4,
	};
	struct spi_message m;

	/* convert to little endian so we can shift out MSB first */
	speed = __cpu_to_le32(priv->speed);

	spi_message_init(&m);
	spi_message_add_tail(&t, &m);
	res = spi_sync(priv->spi, &m);

	if (res)
		/* IO failure */
		return res;

	*sample = __le32_to_cpu(*sample);

	return 0;
}

static int ltc2440_open(struct inode *inode, struct file *filp)
{
	struct ltc2440_priv *priv = container_of(inode->i_cdev, struct ltc2440_priv, cdev);
	filp->private_data = priv;

        return 0;
}

/**
 * ltc2440_read - reads one sample from the LTC2440
 * @return: 4 when successfull, -EBUSY when the conversion is not complete or
 * !=0 otherwise
 */
static ssize_t ltc2440_read (struct file *filp, char *buf, size_t count,
			     loff_t *offset)
{
	struct ltc2440_priv *priv = filp->private_data;
	int res;
	u32 sample;

	if (count < 4)
		return -EINVAL;

	mutex_lock(&priv->mutex);
	res = ltc2440_send_speed_and_read_sample(priv, &sample);
	mutex_unlock(&priv->mutex);

	if (res < 0)
		return res;

	/* the handling of LTC_EOC and the shifting of the sample value has been
	 * taken from ltc2440 so the application can be used without any change */
	if (sample & LTC_EOC)
		/* conversion not complete, try again */
		return -EBUSY;

	sample &= ~LTC_STATUS;
	/* shift MSB of sample to be the MSB of a 32bit value */
	sample <<= LTC_BITS_TO_SHIFT_FOR_MSB_AT_BIT31;

	if (copy_to_user(buf, &sample, 4))
		return -EFAULT;

	return 4;
}

static long ltc2440_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct ltc2440_priv *priv = filp->private_data;
        int err = 0;
	u32 tmp;

	mutex_lock(&priv->mutex);
        switch (cmd) {
	    case LTC_IO_SET_SPEED:
		priv->speed = (int) arg;
		break;

	    case LTC_IO_RESTART:
		err = ltc2440_send_speed_and_read_sample(priv, &tmp);
		break;

            default:
                err = -ENOTTY;
        }
	mutex_unlock(&priv->mutex);

        return err;
}

struct file_operations ltc2440_fops = {
        .owner          = THIS_MODULE,
        .open           = ltc2440_open,
	.read           = ltc2440_read,
        .unlocked_ioctl = ltc2440_ioctl,
};


static int ltc2440_probe(struct spi_device *spi)
{
	struct ltc2440_priv *priv;
        int   err;

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

        err = alloc_chrdev_region(&priv->devt, 0, 1, DRIVER_NAME);
        if (err)
                goto error;

        cdev_init(&priv->cdev, &ltc2440_fops);

        err = cdev_add(&priv->cdev, priv->devt, 1);
        if (err)
                goto error_cdevadd;

        /* we need a class for mdev */
        priv->class = class_create(THIS_MODULE, DRIVER_NAME);
        if (IS_ERR(priv->class))
                goto error_class;

        priv->dev = device_create(priv->class, NULL, priv->devt, NULL, DRIVER_NAME);
        if (IS_ERR(priv->dev))
                goto error_device;

	/* Initialize the driver data */
	priv->spi          = spi;
	spi->mode          = SPI_MODE_0;
	spi->bits_per_word = 32;

	mutex_init(&priv->mutex);

	spi_set_drvdata(spi, priv);

	dev_info(&spi->dev, "initialized (max_speed_hz=%d)\n",spi->max_speed_hz);

        return 0;

error_device:
        class_destroy(priv->class);

error_class:
        cdev_del(&priv->cdev);

error_cdevadd:
        unregister_chrdev_region(priv->devt, 1);

error:
	kfree(priv);

        return err;
}

static int ltc2440_remove(struct spi_device *spi)
{
	struct ltc2440_priv *priv = spi_get_drvdata(spi);
	spi_set_drvdata(spi, NULL);

        device_destroy(priv->class, priv->devt);
	class_destroy(priv->class);
        unregister_chrdev_region(priv->devt, 1);

	kfree(priv);

	return 0;
}

static struct spi_driver ltc2440_driver = {
	.driver = {
		.name =		DRIVER_NAME,
		.owner =	THIS_MODULE,
	},
	.probe =	ltc2440_probe,
	.remove =	ltc2440_remove,

	/* NOTE:  suspend/resume methods are not necessary here.
	 * We don't do anything except pass the requests to/from
	 * the underlying controller.  The refrigerator handles
	 * most issues; the controller driver handles the rest.
	 */
};

module_spi_driver(ltc2440_driver);
MODULE_LICENSE("GPL");
