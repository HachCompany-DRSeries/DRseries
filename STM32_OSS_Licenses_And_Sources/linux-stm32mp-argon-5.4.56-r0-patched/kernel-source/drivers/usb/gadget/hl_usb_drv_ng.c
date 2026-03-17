/*
 * hl_usb_drv_ng.c
 *
 *  Created on: 15. May 2024
 *      Author: Stephan Radke @ Hach
 */

/**
 * This is a USB gadget and USB composite based device driver for the Viper projects.
 * Based on driver project by Thomas Siegmund, 2004
*/

#include <linux/stddef.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/gadget.h>
#include <linux/usb/composite.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <asm/ioctl.h>

#include "hl_usb_drv_ng.h"
#include "uapi/linux/usb/ch9.h"

MODULE_LICENSE("GPL");

/* Definition of ids */
#define USB_VERSION    0x0110  /* 1.1 (full speed) */
#define VENDOR_ID      0x1670  /* assigned by USB Forum */
#define PRODUCT_ID     0x00C8  /* assigned by company see PID assignments.xls */
#define BCDDEVICE      0x0001  /* assigned by developer */

/* Definition of strings */
#define MANUFACTURE_STR   "Hach-Lange GmbH"
#define PRODUCT_STR       "Hach-Lange Photometer"
#define VERSION_STRW      "1.16" 
#define CONFIG_STR        "HL_USB_NG"

#define VERSION_STRA      "01.16"	

#define VERSION_USB       116

#define NUM_OF_ENDPOINTS  7
#define EP0_FIFO_SIZE           64 // 8
#define RX_BULK_EP_FIFO_SIZE    64
#define TX_BULK_EP_FIFO_SIZE    64
#define RX_INTR_EP_FIFO_SIZE     8
#define TX_INTR_EP_FIFO_SIZE_COMM     8
#define TX_INTR_EP_FIFO_SIZE_MEAS    64

#define NUM_OF_ENDPOINTS_EXCLUDE_ZERO 4 

/* 'M' as magic number */
#define  USB_IO_MAGIC      'M'
#define  USB_IO_RESET      _IO(USB_IO_MAGIC, 0)

#define  USB_IO_CONN_USB                _IO(USB_IO_MAGIC, 1) //, connect)
#define  USB_IO_GET_BULK_DATA_SIZE      _IOR(USB_IO_MAGIC, 2, unsigned int)
#define  USB_IO_BULK_DATA_IN            _IOW(USB_IO_MAGIC, 3, unsigned int)
//#define  USB_IO_USER_BREAK_DOWN       _IOW(USB_IO_MAGIC, 4, unsigned int)
#define  USB_IO_USER_BREAK              _IO(USB_IO_MAGIC, 4)
#define  USB_IO_GET_READ_STREAM_SIZE    _IOR(USB_IO_MAGIC, 5, unsigned int)
#define  USB_IO_GET_USB_STATUS          _IO(USB_IO_MAGIC, 6)
#define  USB_IO_SET_TIMEOUT_WRITE_MEASUREMENT       _IOW(USB_IO_MAGIC, 7, unsigned int)
#define  USB_IO_SET_MEAS_DATA_SIZE      _IOW(USB_IO_MAGIC, 8, unsigned int)
#define  USB_IO_SET_TIMEOUT_WRITE_BULK  _IOW(USB_IO_MAGIC, 9, unsigned int)
#define  USB_IO_GET_COMMAND             _IOR(USB_IO_MAGIC, 10, unsigned int)
#define  USB_IO_GET_VERSION             _IOR(USB_IO_MAGIC, 11, unsigned int)
#define  USB_IO_IOCTL_MAX               12
/************************************************************/

#define YES 1
#define NO  0

/**
 * list of endpoints that are available on the hardware
*/
typedef enum {
  EPT0 = 0,
  EPT1,
  EPT2,
  EPT3,
  EPT4,
  EPT5,
  EPT6,
  EPT7,
  EPT8,
  EPT9,
  EPT10,
  EPT11,
  EPT12,
  EPT13,
  EPT_LAST // EPT_14
} EndPoint;

/**
 * enumeration of device string indexes
*/
typedef enum {
  STR_LANGID = 0,
  STR_MANUFACTURER = 1,
  STR_PRODUCT,
  STR_VERSION,
  STR_LAST_INDEX
} String_index;

/**
 *  Enumeration of control commands
 */
typedef enum {
    USB_NO_COMMAND = 0x00,

    BULK_DATA_OUT_AMOUNT = 0x10, //Host sends a predeterminted amount of data to device
    BULK_DATA_OUT_STREAM = 0x11, //Host sends a stream of data to device and finish with a ZERO LENGTH PACKAGE

    BULK_DATA_IN_AMOUNT  = 0x12, //Device sends a predeterminted amount of data to host
    BULK_DATA_IN_STREAM  = 0x13, //Device sends a stream of data to host and finish with a ZERO LENGTH PACKAGE

    CTRL_DATA_OUT        = 0x14, //Host sends data via control transfer to device
    CTRL_DATA_IN         = 0x15  //Device sends data via control transfer to host
} CtrlCommand;

typedef enum {
  USB_DISCONNECTED = -1,
  USB_CONNECTED = 0   
} USB_STATES;

/**
 *  table of devices that work with this driver
 */
static struct usb_device_id hl_usb_ng_table [] = {
        { USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
        { }                      /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, hl_usb_ng_table);

/**
 * usb transfer structure
*/
struct usb_transfer {
  //unsigned char *buffer;
  size_t size;
  size_t rest;
  size_t copied;
  __u8 transfer_active;
  spinlock_t lock;
  wait_queue_head_t wait;
  struct usb_ep *ep;
  CtrlCommand command;
  struct usb_request *pending_req;
};

/**
 * usb device instance
*/
struct hl_usb_ng_device_inst {
  struct usb_composite_dev *udev;
  struct usb_interface *interface;
  struct usb_transfer bulk_in_transfer;
  struct usb_transfer bulk_out_transfer;
  struct usb_transfer int_cmd_transfer;
  struct usb_transfer int_meas_transfer;
  
  spinlock_t device_lock;
  int interface_id;
};

/**
 * basic driver functions used for file operations
*/
static int hl_usb_ng_open(struct inode *, struct file *);
static long hl_usb_ng_ioctl(struct file *, unsigned int, unsigned long);
static int hl_usb_ng_release(struct inode *, struct file *);
static int hl_usb_ng_write_meas(struct file *, const char __user *, size_t, loff_t *);
static int hl_usb_ng_read_ctrl(struct file *, char __user *, size_t, loff_t *);
static int hl_usb_ng_write_ctrl(struct file *, const char __user *, size_t, loff_t *);
static unsigned int hl_usb_ng_poll_ctrl(struct file *filp, poll_table *wait);
static int hl_usb_ng_read_bulk(struct file *, char __user *, size_t, loff_t *);
static int hl_usb_ng_write_bulk(struct file *, const char __user *, size_t, loff_t *);
static unsigned int hl_usb_ng_poll_bulk(struct file *filp, poll_table *wait);

/**
 * minor device ids
*/
#define HL_USB_NG_CNTRL_DEVICE         0
#define HL_USB_NG_MEASUREMENT_DEVICE   1
#define HL_USB_NG_BULK_DEVICE          2

/**
 * declaration of file operations for major and minor devices
*/
struct file_operations hl_usb_ng_fops = {
  .open = hl_usb_ng_open
};

const struct file_operations hl_usb_ng_fops_meas = {
  //.owner = THIS_MODULE,
  .read = hl_usb_ng_read_bulk,
  .write = hl_usb_ng_write_meas,
  .unlocked_ioctl = hl_usb_ng_ioctl,
  .poll = hl_usb_ng_poll_bulk,
  .release = hl_usb_ng_release
};

const struct file_operations hl_usb_ng_fops_bulk = {
  //.owner = THIS_MODULE,
  .read = hl_usb_ng_read_bulk,
  .write = hl_usb_ng_write_bulk,
  .unlocked_ioctl = hl_usb_ng_ioctl,
  .poll = hl_usb_ng_poll_bulk,
  .release = hl_usb_ng_release
};

const struct file_operations hl_usb_ng_fops_ctrl = {
  //.owner = THIS_MODULE,
  .read = hl_usb_ng_read_ctrl,
  .write = hl_usb_ng_write_ctrl,
  .unlocked_ioctl = hl_usb_ng_ioctl,
  .poll = hl_usb_ng_poll_ctrl,
  .release = hl_usb_ng_release
};

const struct file_operations* hl_usb_ng_fop_array[] = {
  &hl_usb_ng_fops_ctrl,
  &hl_usb_ng_fops_meas,
  &hl_usb_ng_fops_bulk
};

/**
 * USB device descriptor structure
*/
struct usb_device_descriptor hl_usb_ng_dev_desc = {
  .bLength        = USB_DT_DEVICE_SIZE,
  .bDescriptorType = USB_DT_DEVICE,
  .bcdUSB         = USB_VERSION,
  .bDeviceClass   = USB_CLASS_PER_INTERFACE,
  .bDeviceSubClass = 0,
  .bDeviceProtocol = 0,
  .bMaxPacketSize0 = EP0_FIFO_SIZE,
  .idVendor       = VENDOR_ID,
  .idProduct      = PRODUCT_ID,
  .bcdDevice      = BCDDEVICE,
  .iManufacturer  = 1,
  .iProduct       = 2,
  .iSerialNumber  = 0, //STR_VERSION,
  .bNumConfigurations = 1
};

struct usb_composite_overwrite overwrite =
{
  .idProduct = PRODUCT_ID,
  .idVendor = VENDOR_ID,
  .bcdDevice = BCDDEVICE,
  .manufacturer = MANUFACTURE_STR,
  .product = PRODUCT_STR,
  .serial_number = 0
};

/**
 * USB configuration descriptor structure
*/
struct usb_config_descriptor hl_usb_ng_conf_desc = {
	.bLength        = USB_DT_CONFIG_SIZE,
	.bDescriptorType = USB_DT_CONFIG,
  .wTotalLength   = sizeof(struct usb_config_descriptor),
  .bNumInterfaces = 1,
	.bConfigurationValue = 1,
	.iConfiguration = 0,
	.bmAttributes   = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
	.bMaxPower      = 0
};

/**
 * USB interface descriptor structure
*/
struct usb_interface_descriptor hl_usb_ng_intf_desc = {
	.bLength        = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
  .bInterfaceNumber = 0,
	.bAlternateSetting = 0,
	.bNumEndpoints  = NUM_OF_ENDPOINTS_EXCLUDE_ZERO,
	.bInterfaceClass = USB_CLASS_MASS_STORAGE,
	.bInterfaceSubClass = USB_SUBCLASS_VENDOR_SPEC,
	.bInterfaceProtocol = 0x01,
	.iInterface     = 0
};

/**
 * USB endpoint descriptor structures
*/
struct usb_endpoint_descriptor hl_usb_ng_in_ep1_desc = {
        .bLength = USB_DT_ENDPOINT_SIZE,
        .bDescriptorType = USB_DT_ENDPOINT,
        .bEndpointAddress = EPT1 | (1<<7),
        .bmAttributes = USB_ENDPOINT_XFER_BULK,
        .wMaxPacketSize = TX_BULK_EP_FIFO_SIZE,
        .bInterval = 0
};

struct usb_endpoint_descriptor hl_usb_ng_out_ep2_desc = {
        .bLength = USB_DT_ENDPOINT_SIZE,
        .bDescriptorType = USB_DT_ENDPOINT,
        .bEndpointAddress = EPT2,
        .bmAttributes = USB_ENDPOINT_XFER_BULK,
        .wMaxPacketSize = RX_BULK_EP_FIFO_SIZE,
        .bInterval = 0
};

struct usb_endpoint_descriptor hl_usb_ng_in_ep3_desc = {
        .bLength = USB_DT_ENDPOINT_SIZE,
        .bDescriptorType = USB_DT_ENDPOINT,
        .bEndpointAddress = EPT3 | (1<<7),
        .bmAttributes = USB_ENDPOINT_XFER_INT,
        .wMaxPacketSize = TX_INTR_EP_FIFO_SIZE_COMM,
        .bInterval = 255
};

struct usb_endpoint_descriptor hl_usb_ng_in_ep5_desc = {
        .bLength = USB_DT_ENDPOINT_SIZE,
        .bDescriptorType = USB_DT_ENDPOINT,
        .bEndpointAddress = EPT5 | (1<<7),
        .bmAttributes = USB_ENDPOINT_XFER_INT,
        .wMaxPacketSize = TX_INTR_EP_FIFO_SIZE_MEAS,
        .bInterval = 1
};

/**
 * USB device strings
*/
struct usb_string hl_usb_ng_strings[] =
{
  {1, MANUFACTURE_STR},
  {2, PRODUCT_STR},
  {0, NULL},
};

struct usb_gadget_strings hl_usb_ng_gadg_stringtab = {
        .language = 0x0409,
        .strings = hl_usb_ng_strings,
};

struct usb_gadget_strings* hl_usb_ng_gadg_strings[] = {
  &hl_usb_ng_gadg_stringtab,
  NULL
};

struct hl_usb_ng_device_inst device_inst;

/**
 * complete usb descriptors for usb device interface description
*/
const struct usb_descriptor_header* hl_usb_ng_desc_headers[] = {
  (const struct usb_descriptor_header*)&hl_usb_ng_intf_desc,
  (const struct usb_descriptor_header*)&hl_usb_ng_in_ep1_desc,
  (const struct usb_descriptor_header*)&hl_usb_ng_out_ep2_desc,
  (const struct usb_descriptor_header*)&hl_usb_ng_in_ep3_desc,
  (const struct usb_descriptor_header*)&hl_usb_ng_in_ep5_desc,
  NULL
};

/**
 * USB function type function definitions
*/
static void hl_usb_ng_function_disable(struct usb_function* f);
static int hl_usb_ng_function_set_alt(struct usb_function* f, unsigned interface, unsigned alt);
static int hl_usb_ng_function_bind(struct usb_configuration* c, struct usb_function* f);

/**
 * usb function struct used for initialisation
*/
static struct usb_function hl_usb_ng_function = 
{
  .name = "HL_USB_FUNC",
  .fs_descriptors = (struct usb_descriptor_header**)hl_usb_ng_desc_headers,
  .hs_descriptors = NULL, 
  .ss_descriptors = NULL,
  .ssp_descriptors = NULL,
  .disable = hl_usb_ng_function_disable,
  .set_alt = hl_usb_ng_function_set_alt,
  .get_alt = NULL,
  .strings = hl_usb_ng_gadg_strings,
  .bind = hl_usb_ng_function_bind  
};

static int hl_usb_ng_configuration_setup(struct usb_configuration *, const struct usb_ctrlrequest *);
static void hl_usb_ng_configuration_unbind(struct usb_configuration *);

/**
 * usb configuration use dduring initialisation (probing)
*/
static struct usb_configuration hl_usb_ng_config_driver = 
{
  .label = CONFIG_STR,
  .bmAttributes = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
  .strings = hl_usb_ng_gadg_strings,
  .descriptors = NULL,
  .bConfigurationValue = 1,
  .iConfiguration = 0,
  .MaxPower = 0,
  .cdev = 0,
  .fullspeed = true,
  .highspeed = false,
  .superspeed = false,
  .superspeed_plus = false,
  .setup = hl_usb_ng_configuration_setup,
  .unbind = hl_usb_ng_configuration_unbind
};

/**
 * functions used for composite driver initialisation
*/
static int hl_usb_ng_comp_bind(struct usb_composite_dev *cdev);
static int hl_usb_ng_comp_unbind(struct usb_composite_dev *cdev);
static void hl_usb_ng_comp_disconnect(struct usb_composite_dev *cdev);
static void hl_usb_ng_comp_suspend(struct usb_composite_dev *cdev);
static void hl_usb_ng_comp_resume(struct usb_composite_dev *cdev);

/**
 * definition of composite driver structure
*/
static struct usb_composite_driver hl_usb_ng_comp_driver = {
        .name           = "Hach Lange USB Device Driver",
        .dev            = &hl_usb_ng_dev_desc, // usb_device_descriptor*
        .strings        = hl_usb_ng_gadg_strings, // usb_gadget_strings**
        .max_speed      = USB_SPEED_FULL, // enum usb_device_speed
        .needs_serial   = 0, // usigned :1;
        .bind           = hl_usb_ng_comp_bind, // int (*bind)(struct usb_composite_dev *cdev);
        .unbind         = hl_usb_ng_comp_unbind, // int (*unbind)(struct usb_composite_dev *);
        .disconnect     = hl_usb_ng_comp_disconnect, // void (*disconnect)(struct usb_composite_dev *);
        .suspend        = hl_usb_ng_comp_suspend, // void (*suspend)(struct usb_composite_dev *);
        .resume         = hl_usb_ng_comp_resume // void (*resume)(struct usb_composite_dev *);
};

/**
 * completion callback for transfer
*/
static void transfer_completed(struct usb_ep *ep, struct usb_request *req);

static int init_transfer(struct usb_transfer* tf, char* buf, size_t size);


static int hl_usb_ng_configuration_setup(struct usb_configuration *conf, const struct usb_ctrlrequest *req)
{
  pr_debug("----- %s called\n", __func__);
  return 0;
}

static void hl_usb_ng_configuration_unbind(struct usb_configuration *conf)
{
  pr_debug("----- %s called\n", __func__);
}

/**
 * bind function called during dirver probe to initialise usb device functions
*/
static int hl_usb_ng_function_bind(struct usb_configuration* c, struct usb_function* f)
{
  struct usb_ep* ep;

  int retval = 0;

  pr_debug("----- %s called\n", __func__);

  retval = usb_interface_id(c,f);
  if(retval < 0)
  {
    pr_err("----- %s: Error obtaining interface id %d\n",__func__, retval);
    return retval;
  }
  device_inst.interface_id = retval;

  //endpoint for bulk_in
  ep = usb_ep_autoconfig(c->cdev->gadget, &hl_usb_ng_in_ep1_desc);
  if(!ep)
  {
    return -ENODEV;
  }
  device_inst.bulk_in_transfer.ep = ep;

  pr_debug("----- %s: device_inst.bulk_in_transfer.ep->ops=%p\n", __func__, device_inst.bulk_in_transfer.ep->ops);
  pr_debug("----- %s: device_inst.bulk_in_transfer.ep->address=%d\n", __func__, device_inst.bulk_in_transfer.ep->address);
  pr_debug("----- %s: device_inst.bulk_in_transfer.ep->desc=%p\n", __func__, device_inst.bulk_in_transfer.ep->desc);

  //1st dummy endpoint
  usb_ep_autoconfig(c->cdev->gadget, &hl_usb_ng_out_ep2_desc);
  //endpoint for bulk_out
  ep = usb_ep_autoconfig(c->cdev->gadget, &hl_usb_ng_out_ep2_desc);
  if(!ep)
  {
    return -ENODEV;
  }
  device_inst.bulk_out_transfer.ep = ep;

  pr_debug("----- %s: device_inst.bulk_out_transfer.ep->ops=%p\n", __func__, device_inst.bulk_out_transfer.ep->ops);
  pr_debug("----- %s: device_inst.bulk_out_transfer.ep->address=%d\n", __func__, device_inst.bulk_out_transfer.ep->address);
  pr_debug("----- %s: device_inst.bulk_out_transfer.ep->desc=%p\n", __func__, device_inst.bulk_out_transfer.ep->desc);

  //2nd dummy endpoint
  usb_ep_autoconfig(c->cdev->gadget, &hl_usb_ng_in_ep3_desc);
  //endpoint for int_cmd
  ep = usb_ep_autoconfig(c->cdev->gadget, &hl_usb_ng_in_ep3_desc);
  if(!ep)
  {
    return -ENODEV;
  }
  device_inst.int_cmd_transfer.ep = ep;

  pr_debug("----- %s: device_inst.int_cmd_transfer.ep->ops=%p\n", __func__, device_inst.int_cmd_transfer.ep->ops);
  pr_debug("----- %s: device_inst.int_cmd_transfer.ep->address=%d\n", __func__, device_inst.int_cmd_transfer.ep->address);
  pr_debug("----- %s: device_inst.int_cmd_transfer.ep->desc=%p\n", __func__, device_inst.int_cmd_transfer.ep->desc);

  //3rd dummy endpoint
  usb_ep_autoconfig(c->cdev->gadget, &hl_usb_ng_in_ep5_desc);
  //endpoint for int_meas
  ep = usb_ep_autoconfig(c->cdev->gadget, &hl_usb_ng_in_ep5_desc);
  if(!ep)
  {
    return -ENODEV;
  }
  device_inst.int_meas_transfer.ep = ep;

  pr_debug("----- %s: device_inst.int_meas_transfer.ep->ops=%p\n", __func__, device_inst.int_meas_transfer.ep->ops);
  pr_debug("----- %s: device_inst.int_meas_transfer.ep->address=%d\n", __func__, device_inst.int_meas_transfer.ep->address);
  pr_debug("----- %s: device_inst.int_meas_transfer.ep->desc=%p\n", __func__, device_inst.int_meas_transfer.ep->desc);
  
  pr_debug("----- %s alloc_request %p \n",__func__, device_inst.int_meas_transfer.ep->ops->alloc_request);
  pr_debug("----- %s dequeue %p \n",__func__, device_inst.int_meas_transfer.ep->ops->dequeue);
  pr_debug("----- %s disable %p \n",__func__, device_inst.int_meas_transfer.ep->ops->disable);
  pr_debug("----- %s dispose %p \n",__func__, device_inst.int_meas_transfer.ep->ops->dispose);
  pr_debug("----- %s enable %p \n",__func__, device_inst.int_meas_transfer.ep->ops->enable);
  pr_debug("----- %s fifo_flush %p \n",__func__, device_inst.int_meas_transfer.ep->ops->fifo_flush);
  pr_debug("----- %s fifo_status %p \n",__func__, device_inst.int_meas_transfer.ep->ops->fifo_status);
  pr_debug("----- %s free_request %p \n",__func__, device_inst.int_meas_transfer.ep->ops->free_request);
  pr_debug("----- %s queue %p \n",__func__, device_inst.int_meas_transfer.ep->ops->queue);
  pr_debug("----- %s set_halt %p \n",__func__, device_inst.int_meas_transfer.ep->ops->set_halt);
  pr_debug("----- %s set_wedge %p \n",__func__, device_inst.int_meas_transfer.ep->ops->set_wedge);

  pr_debug("----- %s ep->desc = %p\n",__func__, device_inst.int_meas_transfer.ep->desc);

  device_inst.udev = c->cdev;

  return 0;

}

static int disable_endpoints(struct hl_usb_ng_device_inst *dev)
{
  int retval = -EINVAL;
  unsigned int flags;
  if(dev)
  {
    //free USB requests and disable endpoints
    spin_lock_irqsave(&device_inst.bulk_in_transfer.lock, flags);
    if(dev->bulk_in_transfer.pending_req)
    {
      usb_ep_dequeue(dev->bulk_in_transfer.ep, dev->bulk_in_transfer.pending_req);
      if(dev->bulk_in_transfer.pending_req->buf)
      {
        kfree(dev->bulk_in_transfer.pending_req->buf);
        dev->bulk_in_transfer.pending_req->buf = NULL;
      }
    }
    if(dev->bulk_in_transfer.ep->enabled)
    {
      usb_ep_disable(dev->bulk_in_transfer.ep);
      dev->bulk_in_transfer.ep->enabled = 0;
    }
    spin_unlock_irqrestore(&device_inst.bulk_in_transfer.lock, flags);

    spin_lock_irqsave(&device_inst.bulk_out_transfer.lock, flags);
    if(dev->bulk_out_transfer.pending_req)
    {
      usb_ep_dequeue(dev->bulk_out_transfer.ep, dev->bulk_out_transfer.pending_req);
      if(dev->bulk_out_transfer.pending_req->buf)
      {
        kfree(dev->bulk_out_transfer.pending_req->buf);
        dev->bulk_out_transfer.pending_req->buf = NULL;
      }
    }
    if(dev->bulk_out_transfer.ep->enabled)
    {
      usb_ep_disable(dev->bulk_out_transfer.ep);
      dev->bulk_out_transfer.ep->enabled = 0;
    }
    spin_unlock_irqrestore(&device_inst.bulk_out_transfer.lock, flags);

    spin_lock_irqsave(&device_inst.int_cmd_transfer.lock, flags);
    if(dev->int_cmd_transfer.pending_req)
    {
      usb_ep_dequeue(dev->int_cmd_transfer.ep, dev->int_cmd_transfer.pending_req);
      if(dev->int_cmd_transfer.pending_req->buf)
      {
        kfree(dev->int_cmd_transfer.pending_req->buf);
        dev->int_cmd_transfer.pending_req->buf = NULL;
      }
    }
    if(dev->int_cmd_transfer.ep->enabled)
    {
      usb_ep_disable(dev->int_cmd_transfer.ep);
      dev->int_cmd_transfer.ep->enabled = 0;
    }
    spin_unlock_irqrestore(&device_inst.int_cmd_transfer.lock, flags);

    spin_lock_irqsave(&device_inst.int_meas_transfer.lock, flags);
    if(dev->int_meas_transfer.pending_req)
    {
      usb_ep_dequeue(dev->int_meas_transfer.ep, dev->int_meas_transfer.pending_req);
      if(dev->int_meas_transfer.pending_req->buf)
      {
        kfree(dev->int_meas_transfer.pending_req->buf);
        dev->int_meas_transfer.pending_req->buf = NULL;
      }
    }
    if(dev->int_meas_transfer.ep->enabled)
    {
      usb_ep_disable(dev->int_meas_transfer.ep);
      dev->int_meas_transfer.ep->enabled = 0;
    }
    spin_unlock_irqrestore(&device_inst.int_meas_transfer.lock, flags);
   
    retval = 0;
  }
  return retval;
}

static int set_interface(struct hl_usb_ng_device_inst *dev, struct usb_function *func)
{
  int retval = -EINVAL;
  char* buf = NULL;
  unsigned int flags;

  pr_debug("%s:%d dev = %x func = %x\n", __func__, __LINE__, (unsigned int)dev, (unsigned int)func);
  
  retval = disable_endpoints(dev);

  if(func)
  {
    spin_lock_irqsave(&dev->bulk_in_transfer.lock, flags);
    retval = config_ep_by_speed(dev->udev->gadget, func, dev->bulk_in_transfer.ep);
    if(!retval)
    {
      retval = usb_ep_enable(dev->bulk_in_transfer.ep);
      if(retval == 0 && dev->bulk_in_transfer.pending_req == NULL)
      {
        retval = init_transfer(&dev->bulk_in_transfer, NULL, 0);
      }
    }
    spin_unlock_irqrestore(&dev->bulk_in_transfer.lock, flags);

    spin_lock_irqsave(&dev->bulk_out_transfer.lock, flags);
    if(!retval)
    {
      retval = config_ep_by_speed(dev->udev->gadget, func, dev->bulk_out_transfer.ep);
    }
    if(!retval)
    {
      retval = usb_ep_enable(dev->bulk_out_transfer.ep);
      if(retval == 0)
      {
        buf = kmalloc(1024, GFP_KERNEL);
        if(buf)
        {
          if(dev->bulk_out_transfer.pending_req == NULL)
          {
            //contact may have been lost re-initialize the endpoint
            retval = init_transfer(&dev->bulk_out_transfer, buf, 1024);
          }
          else
          {
            dev->bulk_out_transfer.pending_req->buf = buf;
            dev->bulk_out_transfer.pending_req->length = 1024;
          }
        }
      }
      if(retval == 0)
      {
        dev->bulk_out_transfer.transfer_active = YES;
        retval = usb_ep_queue(dev->bulk_out_transfer.ep, dev->bulk_out_transfer.pending_req, GFP_ATOMIC);
      }
    }
    spin_unlock_irqrestore(&dev->bulk_out_transfer.lock, flags);

    spin_lock_irqsave(&dev->int_cmd_transfer.lock, flags);
    if(!retval)
    {
      retval = config_ep_by_speed(dev->udev->gadget, func, dev->int_cmd_transfer.ep);
    }
    if(!retval)
    {
      retval = usb_ep_enable(dev->int_cmd_transfer.ep);
      if(retval == 0 && dev->int_cmd_transfer.pending_req == NULL)
      {
        retval = init_transfer(&dev->int_cmd_transfer, NULL, 0);
      }
    }
    spin_unlock_irqrestore(&dev->int_cmd_transfer.lock, flags);

    spin_lock_irqsave(&dev->int_meas_transfer.lock, flags);
    if(!retval)
    {
      retval = config_ep_by_speed(dev->udev->gadget, func, dev->int_meas_transfer.ep);
    }
    if(!retval)
    {
      retval = usb_ep_enable(dev->int_meas_transfer.ep);
      if(retval == 0 && dev->int_meas_transfer.pending_req == NULL)
      {
        retval = init_transfer(&dev->int_meas_transfer, NULL, 0);
      }
    }
    spin_unlock_irqrestore(&dev->int_meas_transfer.lock, flags);
  }
  pr_debug("%s:%d retval = %d\n", __func__, __LINE__, retval);
  return retval;
}

/**
 * disables all endpoints
*/
static void hl_usb_ng_function_disable(struct usb_function* f)
{
  pr_debug("----- %s called\n", __func__);
  disable_endpoints(&device_inst);
}

/**
 * function called during usb device setup to reinitialise endpoints
*/
static int hl_usb_ng_function_set_alt(struct usb_function* f, unsigned interface, unsigned alt)
{
  int retval = 0;

  pr_debug("----- %s called with alt = %d, interface = %d\n", __func__, alt, interface);

  retval = set_interface(&device_inst, f);

  return retval;
}

/**
 * binds usb function to the device configuration
*/
static int hl_usb_ng_config_bind(struct usb_configuration *c)
{
  int retval = 0;
  struct usb_function_instance *inst = NULL;
  struct usb_function* func = NULL;

  pr_debug("----- Called %s\n", __func__);
  
  inst = usb_get_function_instance("HL_USB_FUNC");
  if((int)inst > 0)
  {
    pr_debug("----- %s found HL_USB_FUNC instance  %p\n",__func__, inst);
    func = usb_get_function(inst);
    if(func > 0)
    {
      pr_debug("----- %s found function %p\n",__func__, func);
      usb_remove_function(c, func);
    }
  }

  retval = usb_add_function(c, &hl_usb_ng_function);
  if(retval < 0)
  {
    pr_debug("----- %s: error adding usb function %d\n", __func__, retval);
    usb_put_function(&hl_usb_ng_function);
  }

  return retval;
}

/**
 * called during driver probe to start the initialisation
*/
static int hl_usb_ng_comp_bind(struct usb_composite_dev *cdev)
{
  int retval = 0;
  int ret = 0;
  struct usb_ep* list;
  struct list_head* next;

  pr_debug("----- Called %s\n", __func__);

  hl_usb_ng_config_driver.strings = hl_usb_ng_gadg_strings;

  pr_debug("hl_usb_ng_dev_desc.bcdUSB = %d\n", hl_usb_ng_dev_desc.bcdUSB);

  hl_usb_ng_dev_desc.bcdUSB = USB_VERSION;

  pr_debug("---- %s cdev->gadget->ops = %p,  cdev->gadget->ops->udc_set_speed =%p\n", __func__, cdev->gadget->ops, cdev->gadget->ops->udc_set_speed);
  if(cdev->gadget->ops != NULL && cdev->gadget->ops->udc_set_speed != NULL)
  {
    pr_debug("---- %s tried to set the gadget speed to FULL \n", __func__);
    cdev->gadget->ops->udc_set_speed(cdev->gadget, USB_SPEED_FULL);
  }
  else{
    pr_debug("---- %s tried to set the gadget speed to FULL the hard way \n", __func__);
    cdev->gadget->max_speed = USB_SPEED_FULL;
    cdev->gadget->speed = USB_SPEED_FULL;
  }

  /* Register our configuration. */
	if ((ret = usb_add_config(cdev, &hl_usb_ng_config_driver, hl_usb_ng_config_bind)) < 0)
  {
		retval = -1;
    pr_debug("----- failed usb_add_config() in %s\n", __func__);
    goto error;
  }  

  usb_composite_overwrite_options(cdev, &overwrite);
  INFO(cdev, "Hach-Lange USB device driver bound\n");

  pr_debug("----- %s EP0 address = %d, ops = %p\n",__func__, cdev->gadget->ep0->address, cdev->gadget->ep0->ops);

  next = &cdev->gadget->ep_list;
  do
  {
    list = list_entry(next, struct usb_ep, ep_list);
    pr_debug("Found endpoint address = %d, name = %s, ops = %p\n",list->address, list->name, list->ops);
  } while ((next = next->next) != &cdev->gadget->ep_list);

	return 0;

  error:

  return retval;
}

static int hl_usb_ng_comp_unbind(struct usb_composite_dev *cdev)
{
  int retval = 0;

  pr_debug("----- Called %s\n", __func__);
  
  device_inst.udev = 0;

  return retval;
}

static void hl_usb_ng_comp_disconnect(struct usb_composite_dev *cdev)
{
  pr_debug("----- Called %s\n", __func__);

  usb_ep_clear_halt(cdev->gadget->ep0);

  disable_endpoints(&device_inst);

}

static void hl_usb_ng_comp_suspend(struct usb_composite_dev *cdev)
{
  pr_debug("----- Called %s\n", __func__);

  disable_endpoints(&device_inst);

  /* Workaround: Trigger soft disconnect in case of suspend */
  pr_debug("HL USB Device Driver Workaroud - SoftDisco \n");
  usb_gadget_disconnect(cdev->gadget);
  usb_gadget_connect(cdev->gadget);
}

static void hl_usb_ng_comp_resume(struct usb_composite_dev *cdev)
{
  pr_debug("----- Called %s\n", __func__);
  set_interface(&device_inst, &hl_usb_ng_function);
}

/**
 * open file operation
*/
static int hl_usb_ng_open(struct inode *inode, struct file *file)
{
  int  minor = MINOR(inode->i_rdev);
 
  pr_debug("----- %s minor: %d\n",__func__, minor);
  pr_debug("----- %s device_inst.udev = %p\n",__func__, device_inst.udev);

  //if (!try_module_get(THIS_MODULE))
	//	return -EBUSY;

  spin_lock(&device_inst.device_lock);
  
  if(!device_inst.udev || !device_inst.udev->gadget->connected)
  {
    spin_unlock(&device_inst.device_lock);
    pr_err("State => USB bus isn't connected\n");
    return -EIO;
  }
  spin_unlock(&device_inst.device_lock);
  
  if( HL_USB_NG_BULK_DEVICE < minor )  
  {
    pr_err("%d: invalid minor, can't open device\n", minor);
    return -ENODEV;
  }
  else 
  { 
    pr_debug("minor number: %d\n", minor); 
  }
  
  file->f_op = hl_usb_ng_fop_array[minor];

  pr_debug("file->f_op->owner = %p\n", file->f_op->owner);
  pr_debug("file->f_op->ioctl = %p\n", file->f_op->unlocked_ioctl);
  pr_debug("file->f_op->poll = %p\n", file->f_op->poll);
  pr_debug("file->f_op->read = %p\n", file->f_op->read);
  pr_debug("file->f_op->write = %p\n", file->f_op->write);
  pr_debug("file->f_op->flush = %p\n", file->f_op->flush);

  switch( minor ) 
  {
    case HL_USB_NG_CNTRL_DEVICE:
      pr_debug("HL_USB_NG_CNTRL_DEVICE\n");
    break;
    case HL_USB_NG_MEASUREMENT_DEVICE:
      pr_debug("HL_USB_NG_MEASUREMENT_DEVICE\n");
    break;
    case HL_USB_NG_BULK_DEVICE:
      pr_debug("HL_USB_NG_BULK_DEVICE\n");
    break;
    default:
    break;
  }

  return 0;

}

/**
 * read file operation for bulk endpoit
*/
static int hl_usb_ng_read_bulk(struct file *fd, char __user *buffer, size_t count, loff_t *)
{
  int retval = 0;
  int read = 0;
  unsigned int flags;

  pr_debug("----- %s:%d count = %d\n", __func__, __LINE__, count);  

  spin_lock_irqsave(&device_inst.bulk_out_transfer.lock, flags);

  if(device_inst.bulk_out_transfer.pending_req)
  {
    pr_debug("----- %s:%d device_inst.bulk_out_transfer.pending_req = %x\n", __func__, __LINE__, (unsigned int)device_inst.bulk_out_transfer.pending_req);  
    if(device_inst.bulk_out_transfer.pending_req->actual > 0)
    {
      retval = copy_to_user(buffer, device_inst.bulk_out_transfer.pending_req->buf, device_inst.bulk_out_transfer.pending_req->actual);
      pr_debug("----- %s:%d retval = %d\n", __func__, __LINE__, retval);  
      if(retval != 0)
      {
        retval = -EFAULT;
        spin_unlock_irqrestore(&device_inst.bulk_out_transfer.lock, flags);
        return retval;
      }
      memset(device_inst.bulk_out_transfer.pending_req->buf, 0, device_inst.bulk_out_transfer.pending_req->length);
      read = device_inst.bulk_out_transfer.pending_req->actual;
      device_inst.bulk_out_transfer.pending_req->actual = 0;
      device_inst.bulk_out_transfer.copied = read;
      pr_debug("----- %s:%d device_inst.bulk_out_transfer.transfer_active = %d\n", __func__, __LINE__, device_inst.bulk_out_transfer.transfer_active);
      if(device_inst.bulk_out_transfer.transfer_active == NO)
      {
        retval = 0; //send EOF
        //re-arm the endpoint to receive another packet
        pr_debug("%s:%d re-arm\n",__func__,__LINE__);
        device_inst.bulk_out_transfer.transfer_active = YES;
        usb_ep_queue(device_inst.bulk_out_transfer.ep, device_inst.bulk_out_transfer.pending_req, GFP_KERNEL);
      }
      
    }
  }
  else
  {
    pr_debug("----- %s: No request pending\n", __func__);
  }

  spin_unlock_irqrestore(&device_inst.bulk_out_transfer.lock, flags);

  return retval;
}

/**
 * read file operation for ctrl endpoit
*/
static int hl_usb_ng_read_ctrl(struct file *fd, char __user *buffer, size_t count, loff_t *)
{
  return 0;
}

/**
 * sends data via usb request
*/
static int send_data(const char* data, size_t count, struct usb_transfer* tf, int timeout)
{
  int retval = 0;
  unsigned int flags;
  unsigned int bytes_transferred = 0;

  pr_debug("----- %s count = %d\n", __func__, count);
  pr_debug("----- %s tf = %x, tf->pending_req = %x, tf->pending_req->buf = %x\n", __func__, (unsigned int)tf, tf != NULL ? (unsigned int)tf->pending_req : (unsigned int)NULL, tf != NULL && (unsigned int)tf->pending_req != (unsigned int)NULL ? (unsigned int)tf->pending_req->buf : (unsigned int)NULL);

  spin_lock_irqsave(&tf->lock, flags);
  
  if(!tf || !tf->pending_req)
  {
    spin_unlock_irqrestore(&tf->lock, flags);
    return -EINVAL;
  }

  if(tf->transfer_active == YES)
  {
    spin_unlock_irqrestore(&tf->lock, flags);
    return -EBUSY;
  }

  tf->pending_req->buf = kmalloc(count, GFP_KERNEL);
  if(NULL == tf->pending_req->buf)
  {
    spin_unlock_irqrestore(&tf->lock, flags);
    return -ENOMEM;
  }

  if(copy_from_user(tf->pending_req->buf, data, count))
  {
    kfree(tf->pending_req->buf);
    tf->pending_req->buf = NULL;
    tf->pending_req->length = 0;
    spin_unlock_irqrestore(&tf->lock, flags);
    return -EFAULT;
  }

  tf->pending_req->length = count;
  tf->pending_req->context = tf;
  tf->pending_req->complete = transfer_completed;

  pr_debug("----- %s before queue \n", __func__);
  pr_debug("----- %s request querried, ret = %d\n", __func__, retval);
  pr_debug("----- %s req = %p\n", __func__, tf->pending_req);
  pr_debug("----- %s req->buf = %p\n", __func__, tf->pending_req->buf);
  pr_debug("----- %s req->length = %d\n", __func__, tf->pending_req->length);
  pr_debug("----- %s req->status = %d\n", __func__, tf->pending_req->status);

  retval = usb_ep_queue(tf->ep, tf->pending_req, GFP_KERNEL);
  if(retval != 0)
  {
    pr_debug("----- %s usb_ep_queue failed, ret = %d\n", __func__, retval);
    kfree(tf->pending_req->buf);
    tf->pending_req->buf = NULL;
    tf->pending_req->length = 0;
    spin_unlock_irqrestore(&tf->lock, flags);
    return -EFAULT;
  }
  
  tf->transfer_active = YES;
  set_current_state(TASK_INTERRUPTIBLE);
  spin_unlock_irqrestore(&tf->lock, flags);

  retval = wait_event_interruptible_timeout(tf->wait, tf && tf->transfer_active == NO, msecs_to_jiffies(timeout));
  
  spin_lock_irqsave(&tf->lock, flags);
  set_current_state(TASK_RUNNING);
  pr_debug("----- %s:%d usb_ep_queue executed, ret = %d\n", __func__, __LINE__, retval);
  pr_debug("----- %s req->status = %d\n", __func__, tf->pending_req ? tf->pending_req->status : -1);
  
  if(tf->pending_req && tf->pending_req->buf)
  {
    kfree(tf->pending_req->buf);
    tf->pending_req->buf = NULL;
    tf->pending_req->length = 0;
    bytes_transferred = tf->pending_req->actual;
  }

  tf->transfer_active = NO;
  
  if (retval > 0) 
  {
      //success, transfer executed
      spin_unlock_irqrestore(&tf->lock, flags);
      //return the length of data that has been sent
      return bytes_transferred;
  }

  //no success, clean up the queue
  usb_ep_dequeue(tf->ep, tf->pending_req);

  spin_unlock_irqrestore(&tf->lock, flags);

  return -EIO;
}

static int init_transfer(struct usb_transfer* tf, char* buf, size_t size)
{
  int retval = -EINVAL;
  if(tf)
  {
    tf->size = 0;
    tf->transfer_active = NO;
    tf->command = 0;
    tf->pending_req = usb_ep_alloc_request(tf->ep, GFP_KERNEL);
    if(tf->pending_req != NULL)
    {
      tf->pending_req->complete = transfer_completed;
      tf->pending_req->context = tf;
      if(buf)
      {
        tf->pending_req->buf = buf;
        tf->pending_req->length = size;  
      }
      retval = 0;
    }
  }
  return retval;
}

/**
 * completion callback for bulk out transfer
*/
static void transfer_completed(struct usb_ep *ep, struct usb_request *req)
{
  struct usb_transfer* tf = (struct usb_transfer*)req->context;

  pr_debug("----- %s\n",__func__);

  if(tf != NULL)
  {
    tf->transfer_active = NO;
    if (req->status == -ECONNRESET)		/* Request was cancelled */
		  usb_ep_fifo_flush(ep);
    wake_up(&tf->wait);  
  }
}

/**
 * write file operation for bulk in endpoint
*/
static int hl_usb_ng_write_bulk(struct file *file, const char __user * buffer, size_t count , loff_t *ppos)
{
  int retval;
  static int first = YES;

  pr_debug("----- %s\n",__func__);

  if(0 < count)
  {
    if(first)
    {
      device_inst.bulk_in_transfer.size = count;
      first = NO;
    }
    if(!(device_inst.bulk_in_transfer.command != BULK_DATA_IN_AMOUNT &&
       device_inst.bulk_in_transfer.command != BULK_DATA_IN_STREAM))
    {
      return -EPERM;
    }
  }

  retval = send_data(buffer, count, &device_inst.bulk_in_transfer, 1000);

  return retval;
}

/**
 * write file operation for ctrl endpoint
*/
static int hl_usb_ng_write_ctrl(struct file *file, const char __user * buffer, size_t count , loff_t *ppos)
{
  pr_debug("----- %s\n",__func__);
  return 0;
}

/**
 * write file operation for meas int endpoint
*/
static int hl_usb_ng_write_meas(struct file *file, const char __user * buffer, size_t count , loff_t *ppos)
{
  int retval;
  
  pr_debug("----- %s\n",__func__);

  retval = send_data(buffer, count, &device_inst.int_meas_transfer, 1000);

  return retval;
}

/**
 * ioctl file operation
*/
static long hl_usb_ng_ioctl(struct file *fd, unsigned int cmd, unsigned long arg)
{
  int ret = 0;

  pr_debug("%s... cmd = %d\n",__func__, cmd);

  /*************************************************
   ** IOCTL codes are tested as well as the pointers 
   **************************************************/
  if(_IOC_TYPE(cmd) != USB_IO_MAGIC) 
  { 
    pr_debug("Wrong IOCTL Code\n"); 
    return -ENOTTY;
  } 
  else { 
    pr_debug("_IOC_TYPE(cmd) == %d\n", _IOC_TYPE(cmd)); 
  }
  pr_debug("_IOC_NR(cmd) == %d\n", _IOC_NR(cmd));
  pr_debug("_IOC_DIR(cmd) == %d\n", _IOC_DIR(cmd));
  if(_IOC_NR(cmd) >= USB_IO_IOCTL_MAX) 
  { 
    pr_debug("Wrong IOCTL number\n"); 
    return -ENOTTY;
  }
  if(_IOC_DIR(cmd) & _IOC_READ)
    ret = !access_ok((void*)arg, _IOC_SIZE(cmd));
  else if(_IOC_DIR(cmd) & _IOC_WRITE)	
    ret = !access_ok((void*)arg, _IOC_SIZE(cmd));
  
  if(ret)
  {
    pr_debug("Error Wrong address & pointer in IOCTL codes!!\n"); 
    return -EFAULT;
  }   

  switch(cmd)   
  {
    case USB_IO_GET_READ_STREAM_SIZE:
      pr_debug("USB_IO_GET_READ_STREAM_SIZE\n");
      ret = __put_user(device_inst.bulk_out_transfer.copied, (unsigned int*)arg);
      pr_debug("ReadDataStreamSize: %d\n", device_inst.bulk_out_transfer.copied);
      device_inst.bulk_out_transfer.copied = 0;
    break;

    case USB_IO_SET_TIMEOUT_WRITE_MEASUREMENT:
      pr_debug("USB_IO_SET_TIMEOUT_WRITE_MEASUREMENT\n");
      break;

    case USB_IO_SET_TIMEOUT_WRITE_BULK:
      pr_debug("USB_IO_SET_TIMEOUT_WRITE_BULK\n");
      device_inst.bulk_in_transfer.command = BULK_DATA_IN_STREAM;

      break;

    case USB_IO_GET_COMMAND:
      pr_debug("USB_IO_GET_COMMAND\n");    
      //if we get more commands to handle, maybe we need a switch or something else
      ret = __put_user(device_inst.bulk_in_transfer.command, (unsigned int*)arg);
      break;
    case USB_IO_SET_MEAS_DATA_SIZE:
      pr_debug("USB_IO_SET_MEAS_DATA_SIZE\n");
      //size of data

      ret = __get_user(device_inst.int_meas_transfer.size, (unsigned int*)arg);
      device_inst.int_meas_transfer.rest = device_inst.int_meas_transfer.size;
      device_inst.int_meas_transfer.command = BULK_DATA_IN_AMOUNT;

      pr_debug("USB_IO_SET_MEAS_DATA_SIZE gusbtransf.IntMeasTf.dataSize: %d\n\n\n", 
             device_inst.int_meas_transfer.size);
      break;

    case USB_IO_BULK_DATA_IN:
      pr_debug("USB_IO_BULK_DATA_IN\n");
       
      ret = __get_user(device_inst.int_cmd_transfer.size, (unsigned int*)arg);
      device_inst.int_cmd_transfer.rest = device_inst.int_cmd_transfer.size;
      device_inst.int_cmd_transfer.command = BULK_DATA_IN_AMOUNT;

      pr_debug("\nUSB_IO_BULK_DATA_IN gusbtransf.IntCmdTf.dataSize: %d\n\n", device_inst.int_cmd_transfer.size);

      //maybe we have to wait of for the return of the interrupt transfer: Answer NO
      send_data((const char*)&device_inst.int_cmd_transfer.size, 
                sizeof(device_inst.int_cmd_transfer.size), 
                &device_inst.int_cmd_transfer,
                1000);
      break;

    case USB_IO_GET_USB_STATUS:
      int BusState;
      pr_debug("USB_IO_GET_USB_STATUS\n");
      if(hl_usb_ng_config_driver.cdev != NULL)
      {
        pr_debug("----- %s hl_usb_ng_config_driver.cdev->gadget->state = %d\n",__func__, hl_usb_ng_config_driver.cdev->gadget->state);
        BusState = (hl_usb_ng_config_driver.cdev->gadget->state >= USB_STATE_CONFIGURED) ? USB_CONNECTED : USB_DISCONNECTED;
      }
      else
      {
        BusState = USB_DISCONNECTED;
      }
      pr_debug("BusState = %d\n", BusState);
       if( copy_to_user((void*)arg, (void*)&BusState, sizeof(BusState)) )  {
         return -EFAULT;
      }
       break;
	 
    case USB_IO_GET_VERSION:
      pr_debug("USB_IO_GET_VERSION\n");
      pr_debug("Get USB Version: %d\n", VERSION_USB);
      ret = __put_user(VERSION_USB, (unsigned int*)arg);
      break;
    default:
      pr_err("%s ioctl cmd unknown\n",__func__);
      return -EINVAL;
  }

  return ret;
}

/**
 * release / close file operation
*/
static int hl_usb_ng_release(struct inode *i, struct file *f)
{
  int retval = 0;
  int minor = MINOR(i->i_rdev);

  pr_debug("----- %s minor = %d\n",__func__, minor);
  pr_debug("----- %s device_inst.bulk_out_transfer.pending_req = %p\n", __func__, device_inst.bulk_out_transfer.pending_req);
  pr_debug("----- %s device_inst.bulk_in_transfer.pending_req = %p\n", __func__, device_inst.bulk_in_transfer.pending_req);
  pr_debug("----- %s device_inst.int_meas_transfer.pending_req = %p\n", __func__, device_inst.int_meas_transfer.pending_req);
  pr_debug("----- %s device_inst.int_cmd_transfer.pending_req = %p\n", __func__, device_inst.int_cmd_transfer.pending_req);
  
  switch(minor)
  {
    case HL_USB_NG_MEASUREMENT_DEVICE:

    break;

    case HL_USB_NG_BULK_DEVICE: 

    break;
  }

  return retval;        
}

/**
 * poll file operation for ctrl endpoint
*/
static unsigned int hl_usb_ng_poll_ctrl(struct file *filp, poll_table *wait)
{
  pr_debug("----- %s\n",__func__);
  
  return 0;
}

/**
 * poll file operation for bulk endpoint
*/
static unsigned int hl_usb_ng_poll_bulk(struct file *filp, poll_table *wait)
{
  unsigned int mask = 0, flags = 0;

  if(hl_usb_ng_config_driver.cdev != NULL)
  {
    pr_debug("----- %s hl_usb_ng_config_driver.cdev->gadget->state = %d\n",__func__, hl_usb_ng_config_driver.cdev->gadget->state);
    if(hl_usb_ng_config_driver.cdev->gadget->state != USB_STATE_CONFIGURED) 
    {
      mask |= POLLERR;
      pr_debug("POLLERR \n");
    }
    else
    {

      poll_wait(filp, &device_inst.bulk_out_transfer.wait, wait);

      spin_lock_irqsave(&device_inst.bulk_out_transfer.lock, flags);
      
      if(device_inst.bulk_out_transfer.pending_req != NULL)
      {
        pr_debug("----- %s: Found pending request\n", __func__);
        pr_debug("----- %s: status = %d\n", __func__, device_inst.bulk_out_transfer.pending_req->status);
        pr_debug("----- %s: length = %d\n", __func__, device_inst.bulk_out_transfer.pending_req->length);
        if(device_inst.bulk_out_transfer.pending_req->actual > 0)
        {
          pr_debug("----- %s Received %d bytes\n", __func__, device_inst.bulk_out_transfer.pending_req->actual);
          mask = POLLIN | POLLRDNORM;
        }

        if(device_inst.bulk_out_transfer.copied > 0 || device_inst.bulk_out_transfer.pending_req->status == -ESHUTDOWN)
        {
          pr_debug("----- %s transfer_active = %d\n",__func__, device_inst.bulk_out_transfer.transfer_active);
          mask |= POLLHUP;
        }
      }
      else
      {
        pr_debug("----- %s no pending request\n", __func__);
      }

      spin_unlock_irqrestore(&device_inst.bulk_out_transfer.lock, flags);

    }
  }
  else
  {
    mask |= POLLERR;
    pr_debug("POLLERR, Disconnected \n");
  }

  return mask;
}

/**
 * driver initialisation
*/
static int __init hl_usb_ng_gadget_init(void) 
{
  int ret = 0;
  pr_debug("---- %s: hl_usb_ng_comp_driver.max_speed = %d\n", __func__, hl_usb_ng_comp_driver.max_speed);
  ret = usb_composite_probe(&hl_usb_ng_comp_driver);
  if (ret != 0) {
      pr_err(KERN_ERR "Failed to register composite driver: %d\n", ret);
      return ret;
  }
  if( register_chrdev(HL_USB_NG_MAJOR,"HL_USB_NG", &hl_usb_ng_fops) )
  {
    pr_err("%s unable to get major no: %d for HL_USB_NG\n", __func__, HL_USB_NG_MAJOR);
    return -EIO;
  }
  spin_lock_init(&device_inst.device_lock);

  init_waitqueue_head(&device_inst.bulk_in_transfer.wait);
  spin_lock_init(&device_inst.bulk_in_transfer.lock);

  init_waitqueue_head(&device_inst.bulk_out_transfer.wait);
  spin_lock_init(&device_inst.bulk_out_transfer.lock);

  init_waitqueue_head(&device_inst.int_cmd_transfer.wait);
  spin_lock_init(&device_inst.int_cmd_transfer.lock);

  init_waitqueue_head(&device_inst.int_meas_transfer.wait);
  spin_lock_init(&device_inst.int_meas_transfer.lock);
  return 0;
}

/**
 * driver exit
*/
static void __exit hl_usb_ng_gadget_exit(void) 
{
  pr_debug("---- %s \n",__func__);
  usb_composite_unregister(&hl_usb_ng_comp_driver);
  unregister_chrdev(HL_USB_NG_MAJOR, "HL_USB_NG");
}

module_init(hl_usb_ng_gadget_init);
module_exit(hl_usb_ng_gadget_exit);

