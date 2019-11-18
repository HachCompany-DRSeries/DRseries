/* NMI: set/get NMI flag for SH7723
 *
 * author: mschreiber
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>


#define NMI_MAJOR 42
#define NMI_CLEAR 0x01
#define NMIFCR    0xA41400C0

static int nmi_open (struct inode *inode, struct file *file);
static ssize_t nmi_read (struct file *file, 
                         char *buf, 
                         size_t count,
                         loff_t *offset);
static int nmi_ioctl (struct file *file,
                      unsigned int cmd, unsigned long arg);

#ifdef MODULE
extern int init_module(void);
extern int cleanup_module(void);
static int __init nmi_init(void);
#else
extern int __init nmi_init(void);
#endif

static struct file_operations nmi_fops = {
  unlocked_ioctl: nmi_ioctl,
  read:           nmi_read, // read 16 bit (bit15 = NMIL, bit0 = NMIFL).
  open:           nmi_open,
};

static ssize_t nmi_read (struct file *file, 
                         char *buf, 
                         size_t count,
                         loff_t *offset)
{
  memcpy(buf,(char*)NMIFCR,2); // NMIFCR length = 16bit
  return 2;
}

static int nmi_ioctl (struct file *file,
                      unsigned int cmd, unsigned long arg)
{
  switch(cmd)
  {
    case NMI_CLEAR:
      printk("NMI: clear flag\n");
      *((unsigned short*)NMIFCR) &= 0xFFFE; // clear nmi: 0 - NMI not detected, 1 - NMI detected.
      break;
      
    default:
      return -EINVAL;
      break;
  }
  return 0;
}

static int nmi_open (struct inode *inode, struct file *file)
{
  return 0;
}

int __init nmi_init(void)
{
  printk("NMI init\n");
  
  if (register_chrdev(NMI_MAJOR,"nmi",&nmi_fops)) 
  {
    printk("NMI: unable to get major %d for NMI\n",
           NMI_MAJOR);
    return -EIO;
  }
  return 0;
}

int nmi_cleanup(void)
{
  unregister_chrdev(NMI_MAJOR, "nmi");
  return 0;
}

int __init nmi_module_init(void)
{
  return nmi_init();
}

static void __exit nmi_module_exit(void)
{
  nmi_cleanup();
  return;
}

module_init(nmi_module_init);
module_exit(nmi_module_exit);

