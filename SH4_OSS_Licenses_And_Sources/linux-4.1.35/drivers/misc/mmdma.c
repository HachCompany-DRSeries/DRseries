/*
 * mmdma.c
 *
 * Copyright (c) 2009 by emtrion GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Author:      Markus Pietrek
 * Description: Provides /dev/mmdma. Each opened descriptor allows the
 *              allocation of one memory region with mmap() that is DMA
 *              capable.
 *              It's physical address can be retrieved with the ioctl MMDMA_GETPHYS
 *
 **/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioctl.h>
#include <linux/dma-mapping.h>
#include <asm/uaccess.h>

#include <linux/mmdma.h>

#define DRIVER_NAME "mmdma"

struct file_priv {
        void *va;
        dma_addr_t dma_handle;
        int size;
};

static struct {
        struct device *dev;
        struct cdev  cdev;
        struct class *class;
} priv;


static void mmdma_vmops_close(struct vm_area_struct *vm)
{
	struct file_priv *fpriv = vm->vm_private_data;

        dma_free_coherent(priv.dev, fpriv->size, fpriv->va, fpriv->dma_handle);

        fpriv->va = 0;
}

static const struct vm_operations_struct mmdma_vm_ops = {
        .close = mmdma_vmops_close,
};

static int mmdma_open(struct inode *inode, struct file *filp)
{
        struct file_priv *fpriv = kzalloc(sizeof(*fpriv), GFP_KERNEL);

        if (!fpriv)
                return -ENOMEM;

        filp->private_data = fpriv;
        return 0;
}

static int mmdma_release(struct inode *inode, struct file *filp)
{
        kfree(filp->private_data);
        return 0;
}

static int mmdma_mmap(struct file *filep, struct vm_area_struct *vma)
{
        struct file_priv *fpriv = filep->private_data;
        int err;

        if (fpriv->va)
                /* we allow only one mmap */
                return -EBUSY;

        if (!(vma->vm_flags & VM_LOCKED))
                return -EINVAL;

        if (vma->vm_pgoff)
                return -EINVAL;

        /* allocate dma region */
	fpriv->size = vma->vm_end - vma->vm_start;
        fpriv->va = dma_alloc_coherent(priv.dev, fpriv->size, &fpriv->dma_handle, GFP_KERNEL);

        if (NULL == fpriv->va) {
                dev_err(priv.dev, "mmap: dma_alloc_coherent failed\n");
                return -ENOMEM;
        }

        /* remap memory to user */
        vma->vm_private_data = fpriv;
	vma->vm_ops          = &mmdma_vm_ops;
        // vma->vm_flags |= VM_RESERVED | VM_IO;
        vma->vm_flags |= VM_DONTEXPAND | VM_DONTDUMP | VM_IO;
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
        err = remap_pfn_range(vma, vma->vm_start,
                              fpriv->dma_handle >> PAGE_SHIFT,
                              fpriv->size,
                              vma->vm_page_prot);

        if (err) {
                dev_err(priv.dev, "mmap: remap failed with error %d\n", err);
                dma_free_coherent(priv.dev, fpriv->size, fpriv->va, fpriv->dma_handle);
                return err;
        }

        return 0;

}

static long mmdma_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
        int err;
        struct file_priv *fpriv = filep->private_data;

        switch (cmd) {
            case MMDMA_GETPHYS:
                if (fpriv->va)
                        err = put_user((unsigned long) fpriv->dma_handle, (unsigned long*) arg);
                else
                        /* mapping must exist before physical address can be
                         * returned */
                        err = -EINVAL;
                break;
            default:
                err = -ENOTTY;
        }

        return err;
}

struct file_operations mmdma_fops = {
        .owner          = THIS_MODULE,
        .open           = mmdma_open,
        .release        = mmdma_release,
        .mmap           = mmdma_mmap,
        .unlocked_ioctl = mmdma_ioctl,
};

static int __init mmdma_init(void)
{
        dev_t devt;
        int   err;

        err = alloc_chrdev_region(&devt, 0, 1, DRIVER_NAME);
        if (err)
                goto error;

        cdev_init(&priv.cdev, &mmdma_fops);

        err = cdev_add(&priv.cdev, devt, 1);
        if (err)
                goto error_cdevadd;

        /* we need a class for mdev */
        priv.class = class_create(THIS_MODULE, DRIVER_NAME);
        if (IS_ERR(priv.class))
                goto error_class;

        priv.dev = device_create(priv.class, NULL, devt, NULL, DRIVER_NAME);
        if (IS_ERR(priv.dev))
                goto error_device;

        return 0;

error_device:
        class_destroy(priv.class);

error_class:
        cdev_del(&priv.cdev);

error_cdevadd:
        unregister_chrdev_region(devt, 1);

error:
        return err;
}

static void __exit mmdma_exit(void)
{
        dev_t devt = priv.dev->devt;

        device_destroy(priv.class, devt);
	class_destroy(priv.class);
        cdev_del(&priv.cdev);
        unregister_chrdev_region(devt, 1);
}

module_init(mmdma_init);
module_exit(mmdma_exit);
MODULE_LICENSE("GPL");
