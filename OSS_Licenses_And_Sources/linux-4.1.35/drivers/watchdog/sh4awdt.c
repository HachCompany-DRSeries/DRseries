/*
 * sh4awdt.c - watchdog for the SH4A
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
 * This driver has been partly derived from shwdt.c as the SH4a watchdog implementation is
 * slightly different and can therefore be accessed much more simpler than the SH2/SH4
 *
 **/

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/watchdog.h>
#include <linux/spinlock.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

/* SH7723 processor registers */
#define RWTCNT		0x00
#define RWTCSR  	0x04

#define RWTCNT_MAGIC	0x5A00
#define RWTCSR_MAGIC	0xA500
#define RWTCSR_TME  	0x0080
#define RWTCSR_CKS_MASK 0x0007
#define RWTCSR_CKS_1S   0x0005
#define RWTCSR_CKS_8S   0x0006
#define RWTCSR_CKS_32S  0x0007

static struct sh4awdt_priv {
	struct device *dev;
	void __iomem  *base;
	int 	      expect_close;
	unsigned long is_open;
	spinlock_t    lock;
} *priv;

static int timeout  = 32;	/* in seconds */
static int nowayout = 0;

static u8 sh4a_wdt_read_csr(void)
{
	return __raw_readb(priv->base + RWTCSR);
}

static void sh4a_wdt_write_csr(u8 csr)
{
	__raw_writew(RWTCSR_MAGIC | csr, priv->base + RWTCSR);
	printk("--> wdt 0x%x written to CSR at 0x%x\n", RWTCSR_MAGIC | csr, priv->base + RWTCSR);
}

static void sh4a_wdt_write_cnt(u8 cnt)
{
	__raw_writew(RWTCNT_MAGIC | cnt, priv->base + RWTCNT);
	printk("--> wdt 0x%x written to CNT at 0x%x\n", RWTCNT_MAGIC | cnt, priv->base + RWTCNT);
}

static void sh4a_wdt_start(void)
{
	u8 csr;
	unsigned long flags;

	spin_lock_irqsave(&priv->lock, flags);

	sh4a_wdt_write_cnt(0);
	csr  = sh4a_wdt_read_csr();
	csr |= RWTCSR_TME;
	sh4a_wdt_write_csr(csr);

	spin_unlock_irqrestore(&priv->lock, flags);
}

static void sh4a_wdt_stop(void)
{
	u8 csr;
	unsigned long flags;

	spin_lock_irqsave(&priv->lock, flags);

	csr  = sh4a_wdt_read_csr();
	csr &= ~RWTCSR_TME;
	sh4a_wdt_write_csr(csr);

	spin_unlock_irqrestore(&priv->lock, flags);
}

static inline void sh4a_wdt_keepalive(void)
{
	sh4a_wdt_write_cnt(0);
}

static int sh4a_wdt_set_timeout(int t /*seconds*/)
{
	u8 cks;
	u8 csr;
	unsigned long flags;

	/* determine cks setting for timeout */
	switch (t) {
	    case  1: cks = RWTCSR_CKS_1S;  break;
	    case  8: cks = RWTCSR_CKS_8S;  break;
	    case 32: cks = RWTCSR_CKS_32S; break;
	    default:
		return -EINVAL;
	}

	spin_lock_irqsave(&priv->lock, flags);

	csr = sh4a_wdt_read_csr();
	/* CKS may only be modified with watchdog disabled */
	sh4a_wdt_write_csr(0);
	sh4a_wdt_write_cnt(0);

	csr &= ~RWTCSR_CKS_MASK;
	csr |= cks;

	/* restore old mode (running/stopped) with new timeout */
	sh4a_wdt_write_csr(csr);

	spin_unlock_irqrestore(&priv->lock, flags);

	return 0;
}

static int sh4a_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(0, &priv->is_open))
		return -EBUSY;

	if (nowayout)
		__module_get(THIS_MODULE);

	sh4a_wdt_start();

	return nonseekable_open(inode, file);
}

static int sh4a_wdt_close(struct inode *inode, struct file *file)
{
	if (priv->expect_close == 42) {
		sh4a_wdt_stop();
	} else {
		dev_err(priv->dev, "Unexpected close, not stopping watchdog!\n");
		sh4a_wdt_keepalive();
	}

	clear_bit(0, &priv->is_open);
	priv->expect_close = 0;

	return 0;
}

static ssize_t sh4a_wdt_write(struct file *file, const char *buf,
			      size_t count, loff_t *ppos)
{
	size_t i;

	if (!count || nowayout)
		/* no need to look for 42 */
		return count;

	priv->expect_close = 0;

	/* look for 'V' to signal end of write before close so we now the application
	   did not terminate unexpected */
	for (i = 0; i != count; i++) {
		char c;
		if (get_user(c, buf + i))
			return -EFAULT;
		if (c == 'V')
			priv->expect_close = 42;
	}

	sh4a_wdt_keepalive();

	return count;
}

static const struct watchdog_info sh4a_wdt_info = {
	.options		= WDIOF_KEEPALIVEPING | WDIOF_SETTIMEOUT |
				  WDIOF_MAGICCLOSE,
	.firmware_version	= 0,
	.identity		= "SH4A WDT",
};

static long sh4a_wdt_ioctl(struct file *file, unsigned int cmd,
			   unsigned long arg)
{
	int new_timeout;
	int options, rc = -EINVAL;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
	    rc = copy_to_user((struct watchdog_info *)arg,
			      &sh4a_wdt_info, sizeof(sh4a_wdt_info)) ? -EFAULT : 0;
	    break;
	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
	    rc = put_user(0, (int *)arg);
	    break;
	case WDIOC_SETOPTIONS:
		if (!get_user(options, (int *)arg)) {
			if (options & WDIOS_DISABLECARD) {
				sh4a_wdt_stop();
				rc = 0;
			}

			if (options & WDIOS_ENABLECARD) {
				sh4a_wdt_start();
				rc = 0;
			}
		} else
			rc = -EFAULT;
		break;
	case WDIOC_KEEPALIVE:
		sh4a_wdt_keepalive();
		rc=0;
		break;
	case WDIOC_SETTIMEOUT:
		if (!get_user(new_timeout, (int *)arg))
			rc = sh4a_wdt_set_timeout(new_timeout);
		else
			rc = -EFAULT;

		if (!rc)
			timeout = new_timeout;
		break;
	case WDIOC_GETTIMEOUT:
		rc = put_user(timeout, (int *)arg);
		break;
	default:
		rc = -ENOTTY;
		break;
	}

	return rc;
}

static int sh4a_wdt_notify_sys(struct notifier_block *this,
			       unsigned long code, void *unused)
{
	if (code == SYS_DOWN || code == SYS_HALT)
		sh4a_wdt_stop();

	return NOTIFY_DONE;
}

static const struct file_operations sh4a_wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= sh4a_wdt_write,
	.unlocked_ioctl	= sh4a_wdt_ioctl,
	.open		= sh4a_wdt_open,
	.release	= sh4a_wdt_close,
};

static struct notifier_block sh4a_wdt_notifier = {
	.notifier_call		= sh4a_wdt_notify_sys,
};

static struct miscdevice sh4a_wdt_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &sh4a_wdt_fops,
};

static int sh4a_wdt_probe(struct platform_device *pdev)
{
	struct resource *res;
	int rc;

	if (priv) {
		dev_err(&pdev->dev, "only one instance allowed\n");
		rc = -EBUSY;
		goto error;
	}
	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		dev_err(&pdev->dev, "cannot get memory\n");
		rc = -ENOMEM;
		goto error;
	}
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "cannot get platform resources\n");
		rc = -EINVAL;
		goto error_map;
	}
	priv->base = ioremap(res->start, (res->end-res->start)+1);
	if (!priv->base) {
		dev_err(&pdev->dev, "cannot get io memory\n");
		rc = -ENOMEM;
		goto error_map;
	}

	rc = register_reboot_notifier(&sh4a_wdt_notifier);
	if (rc) {
		dev_err(&pdev->dev, "can't register reboot notifier (err=%d)\n", rc);
		goto error_register_reboot;
	}

	rc = misc_register(&sh4a_wdt_miscdev);
	if (rc) {
		dev_err(&pdev->dev, "can't register miscdev on minor=%d (err=%d)\n",
			sh4a_wdt_miscdev.minor, rc);
		goto error_misc_register;
	}

	priv->dev = &pdev->dev;
	spin_lock_init(&priv->lock);

	rc = sh4a_wdt_set_timeout(timeout);
	dev_info(&pdev->dev, "initialized. timeout=%d sec (nowayout=%d)\n",
		 timeout, nowayout);

	platform_set_drvdata(pdev, priv);

	return 0;

error_misc_register:
	unregister_reboot_notifier(&sh4a_wdt_notifier);
error_register_reboot:
	iounmap(priv->base);
error_map:
	kfree(priv);
error:

	return rc;
}

static int sh4a_wdt_remove(struct platform_device *pdev)
{
	misc_deregister(&sh4a_wdt_miscdev);
	unregister_reboot_notifier(&sh4a_wdt_notifier);
	iounmap(priv->base);
	kfree(priv);
	platform_set_drvdata(pdev, NULL);
	priv = NULL;

	return 0;
}

static struct platform_driver sh4a_wdt_driver = {
	.probe		= sh4a_wdt_probe,
	.remove		= sh4a_wdt_remove,
	.driver		= {
		.name	= "sh4a_wdt",
		.owner	= THIS_MODULE,
	},
};

static int __init sh4a_wdt_init(void)
{
	return platform_driver_register(&sh4a_wdt_driver);
}

static void __exit sh4a_wdt_exit(void)
{
	platform_driver_unregister(&sh4a_wdt_driver);
}

MODULE_AUTHOR("Markus Pietrek");
MODULE_DESCRIPTION("SupeH SH4A watchdog driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);

module_param(timeout, int, 0);
MODULE_PARM_DESC(timeout, "Watchdog timeout in seconds.");

module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout,
		 "Watchdog cannot be stopped once started (default="
		 __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

module_init(sh4a_wdt_init);
module_exit(sh4a_wdt_exit);
