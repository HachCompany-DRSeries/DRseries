/* Hach-Lange 2024 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/sched/signal.h> 
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/wait.h>
#include <linux/platform_device.h>

// defines
#define HL_NMI_MAJOR  42   //   /dev/hl_nmi
#define HL_IRQ_MAJOR  44   //   /dev/hl_irq
#define HL_IRQ_NMI    0x1  //   NMI Flag for standby app
#define HL_IRQ_CCI    0x2  //   IRQ Flag for standby app

DECLARE_WAIT_QUEUE_HEAD(hl_irq_queue);

int irq;                      // IRQ Number 
char hl_irq_flags;            // Storage for IRQ flags
struct gpio_desc *gpio_desc;  // GPIO descriptor 

//------------------------------------------------------------------------ prototypes

static int hl_irq_open(struct inode *inode, struct file *file);
static int hl_nmi_open(struct inode *inode, struct file *file);
static ssize_t hl_irq_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t hl_nmi_read (struct file *file, char *buf, size_t count, loff_t *offset);
static int hl_irq_close (struct inode *inode, struct file *file);
static int hl_nmi_close (struct inode *inode, struct file *file);

//------------------------------------------------------------------------ file operations

static struct file_operations hl_irq_fops = {
  .owner = THIS_MODULE,
  .open  = hl_irq_open,
  .read = hl_irq_read,
  .release = hl_irq_close
};

static struct file_operations hl_nmi_fops = {
  .owner = THIS_MODULE,
  .open  = hl_nmi_open,
  .read = hl_nmi_read,
  .release = hl_nmi_close
};

//------------------------------------------------------------------------ hl_irq functions

static irqreturn_t hl_irq_cci_handler(int irq, void *dev_id)
{     
  hl_irq_flags |= HL_IRQ_CCI;
  hl_irq_flags |= HL_IRQ_NMI;
  printk("hl_irq: irq_handling IRQ_FLAG: %x",hl_irq_flags);
  wake_up_interruptible(&hl_irq_queue);
  return IRQ_HANDLED;
}

static int hl_irq_open(struct inode *inode, struct file *file)
{
  int ret = 0;
  
  printk(KERN_INFO"hl_irq: open\n");
  
  // Convert GPIO descriptor to IRQ number
  irq = gpiod_to_irq(gpio_desc);
  if (irq < 0) {
      printk(KERN_ERR "hl_irq: Failed to convert GPIO to IRQ\n");
      return irq;
  }
  else
  {
      printk("hl_irq: IRQ Num: %i \n", irq);
  }
    
  // Request IRQ for the GPIO pin 
  ret = request_irq(irq, hl_irq_cci_handler, IRQF_TRIGGER_FALLING | IRQF_NO_SUSPEND , "hl_irq", NULL);
  if (ret < 0) {
      printk(KERN_ERR "hl_irq: Failed to request IRQ for GPIO\n");
      return ret;
  }
 
  hl_irq_flags = 0;
  return ret; 
}

static ssize_t hl_irq_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
  ssize_t ret = 0;
  
  DECLARE_WAITQUEUE(wait, current); 
  add_wait_queue(&hl_irq_queue, &wait);
  while (1) {
        set_current_state(TASK_INTERRUPTIBLE);
        if (hl_irq_flags) {
            // Send flags to user space 
            ret = put_user(hl_irq_flags, (char __user *)buf) ? 0 : sizeof(char);
            hl_irq_flags = 0;
            break;
        }
        if (signal_pending(current)) {
            ret = -ERESTARTSYS;
            break;
        }

        if (file->f_flags & O_NONBLOCK) {
            ret = -EAGAIN;
            break;
        }
        schedule();
 };


  set_current_state(TASK_RUNNING);
  remove_wait_queue(&hl_irq_queue, &wait);
  
  return ret;
}

static int hl_irq_close (struct inode *inode, struct file *file)
{
  free_irq(gpiod_to_irq(gpio_desc), NULL);
  hl_irq_flags = 0;
  return 0;
}

//------------------------------------------------------------------------ hl_nmi functions

static int hl_nmi_open(struct inode *inode, struct file *file)
{
  return 0;
}

static ssize_t hl_nmi_read (struct file *file, char *buf, size_t count, loff_t *offset)
{
  ssize_t ret;

  int value = gpiod_get_value(gpio_desc);
  if (value < 0) {
     printk(KERN_ERR "hl_irq: Failed to read GPIO value from hl_nmi\n");
  } 
  else 
  {
     ret = copy_to_user(buf, &value, sizeof(int));
  }

  return sizeof(int);
}

static int hl_nmi_close (struct inode *inode, struct file *file)
{
  return 0;
}

//------------------------------------------------------------------------ driver init/exit

static int hl_irq_probe(struct platform_device *pdev)
{
  int ret;

  // Get GPIO descriptor for the desired GPIO pin
  gpio_desc = devm_gpiod_get_optional(&pdev->dev, NULL, GPIOD_IN);
  if (IS_ERR(gpio_desc)) {
      printk(KERN_ERR "hl_irq: Failed to get GPIO descriptor\n");
      return PTR_ERR(gpio_desc);
  }

  // Set GPIO pin direction to input
  ret = gpiod_direction_input(gpio_desc);
  if (ret) {
      printk(KERN_ERR "hl_irq: Failed to set GPIO direction\n");
      return ret;
  }

  // Register devices 
  if (register_chrdev(HL_IRQ_MAJOR,"hl_irq",&hl_irq_fops)){
      printk(KERN_ERR "hl_irq: unable to get major %d for hl_irq\n",HL_IRQ_MAJOR);
      return -EIO;
  }

  if (register_chrdev(HL_NMI_MAJOR,"hl_nmi",&hl_nmi_fops)){
      printk(KERN_ERR "hl_irq: unable to get major %d for hl_nmi\n",HL_NMI_MAJOR);
      return -EIO;
  }

  return 0; // Success
}

static int hl_irq_remove(struct platform_device *pdev)
{
   unregister_chrdev(HL_IRQ_MAJOR, "hl_irq");
   unregister_chrdev(HL_NMI_MAJOR, "hl_nmi");
   return 0;
}

//------------------------------------------------------------------------  driver description

static const struct of_device_id hl_irq_match[] = {
    { .compatible = "IRQ,Hach-Lange", },
    { /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, hl_irq_match);

static struct platform_driver hl_irq = {
  .probe = hl_irq_probe,
  .remove = hl_irq_remove,
  .driver = {
      .name = "hl_irq",
      .of_match_table = hl_irq_match,
      .owner = THIS_MODULE,
  },
};

module_platform_driver(hl_irq);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lbuehler <luca.buehler@hach.com>");
MODULE_DESCRIPTION("IRQ Module for the standby application of the Viper_STM32 board");

