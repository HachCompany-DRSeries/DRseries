/* IRQ: interrupt handler
 *
 * author: mschreiber
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <asm/io.h>

/* IRQF_DISABLED not supported in Linux 4.1 (not needed either) */
#if !defined(IRQF_DISABLED)
#define IRQF_DISABLED 0x00
#endif

// extern
//------------------------------------------------------------------
#ifdef MODULE
static int __init hl_irq_init(void);
#else
extern int __init hl_irq_init(void);
#endif

// defines
//------------------------------------------------------------------
#define HL_IRQ_MAJOR  44

// irq flags
#define HL_IRQ_NMI    0x1
#define HL_IRQ_CCI    0x2

#define IRQEVT_IRQ2	0x640

#define PWCR     0xA4050146
#define PSELD    0xA4050154
#define HIZCRD   0xA405015E

#define ICR1     0xA414001C
#define INTPRI00 0xA4140010
#define INTREQ00 0xA4140024
#define INTMSK00 0xA4140044

// prototypes
//------------------------------------------------------------------
static int hl_irq_open (struct inode *inode, struct file *file);
static int hl_irq_close (struct inode *inode, struct file *file);
static ssize_t hl_irq_read (struct file *file, 
                            char *buf, 
                            size_t count,
                            loff_t *offset);
static long hl_irq_ioctl (struct file *file,
			  unsigned int cmd, unsigned long arg);

// declarations
//------------------------------------------------------------------
DECLARE_WAIT_QUEUE_HEAD(hl_irq_queue);
char hl_irq_flags; // HL_IRQ_NMI, ...

// fops
//------------------------------------------------------------------
static struct file_operations hl_irq_fops = {
  unlocked_ioctl: hl_irq_ioctl,
  read:           hl_irq_read,
  open:           hl_irq_open,
  release:        hl_irq_close,
};

// read
//------------------------------------------------------------------
static ssize_t hl_irq_read (struct file *file, 
                            char *buf, 
                            size_t count,
                            loff_t *offset)
{
  DECLARE_WAITQUEUE(wait, current);
  ssize_t ret;
  
  if (count != sizeof(char))
    return -EINVAL;
  //printk(KERN_INFO "%s sleeping...\n", __FUNCTION__);
  
  add_wait_queue(&hl_irq_queue, &wait);
  do 
  {
    __set_current_state(TASK_INTERRUPTIBLE);
    
    // handle irq
    if (hl_irq_flags)
    {
      ret = put_user(hl_irq_flags, (char __user *)buf) ? 0 : sizeof(char);
      hl_irq_flags = 0;
      break;
    }
    
    // handle err
    if (file->f_flags & O_NONBLOCK) 
    {
      ret = -EAGAIN;
      break;
    }
    if (signal_pending(current)) 
    {
      ret = -ERESTARTSYS;
      break;
    }
    schedule();
  } while (1);
  
  __set_current_state(TASK_RUNNING);  /* must we use set_current_state ? */
  remove_wait_queue(&hl_irq_queue, &wait);
  
  //printk(KERN_INFO "%s awake\n", __FUNCTION__);  
  return ret;
}

// ioctl
//------------------------------------------------------------------
static long hl_irq_ioctl (struct file *file,
			  unsigned int cmd, unsigned long arg)
{
  // not used yet
  switch(cmd)
  {
    default:
      return -EINVAL;
      break;
  }
  return 0;
}

// NMI handler
//------------------------------------------------------------------
void hl_irq_nmi_handler(void)
{
  hl_irq_flags |= HL_IRQ_NMI;
  wake_up_interruptible(&hl_irq_queue);
}

// cell compartment irq handler
//------------------------------------------------------------------
//static void hl_irq_cci_handler(int irq, void* ptr, struct pt_regs* regs)
static irqreturn_t hl_irq_cci_handler(int irq, void* ptr)
{
  unsigned char cTmp;
  
  hl_irq_flags |= HL_IRQ_CCI;
  
  // reset IRQ2 bit in INTREQ00
  cTmp  = __raw_readb(INTREQ00);
  cTmp &= ~0x20;
  __raw_writeb(cTmp, INTREQ00);

  wake_up_interruptible(&hl_irq_queue);

  return IRQ_HANDLED;
}

// open
//------------------------------------------------------------------
static int hl_irq_open (struct inode *inode, struct file *file)
{
  int ret1 = 0;
  int ret2 = 0;
  unsigned short sTmp;
  
  printk("HACH-LANGE: init IRQ handler\n");
  
  /* request IRQ2 for CellCompartment */
  /************************************/

  // set PSELD to IRQ2 fct (bit 10/11: 00)
  sTmp  = __raw_readw(PSELD);
  sTmp &= ~0xC00;
  __raw_writew(sTmp,PSELD);
  
  // set ICR1 to falling flank (bit 10/11: 00)
  sTmp  = __raw_readw(ICR1);
  sTmp &= ~0xC00;
  __raw_writew(sTmp,ICR1);
  
  if ((ret2 = request_irq(evt2irq(IRQEVT_IRQ2), hl_irq_cci_handler, IRQF_DISABLED, "cci", NULL)) < 0)
    printk("ERR: request_irq(CCI,...) failed! (%d)\n",ret2);
  
  /* printk("IRQ2: PWCR=%x, PSELD=%x, HIZCRD=%x\n", */
  /*        (__raw_readw(PWCR)>>4)&0x3, (__raw_readw(PSELD)>>10)&0x3, */
  /*        (__raw_readw(HIZCRD)>>10)&0x1); */
  /* printk("IRQ2: ICR1=%x, INTPRI00=%x, INTREQ00=%x, INTMSK00=%x\n", */
  /*        (__raw_readw(ICR1)>>10)&0x3, (ctrl_inl(INTPRI00)>>20)&0xf,  */
  /*        (__raw_readb(INTREQ00)>>5)&0x1, (__raw_readb(INTMSK00)>>5)&0x1); */

  hl_irq_flags = 0;
  return ret1?ret1:ret2;
}

// close
//------------------------------------------------------------------
static int hl_irq_close (struct inode *inode, struct file *file)
{
  printk("HACH-LANGE: close IRQ handler\n");
  
  /* free IRQ2 for CellCompartment */
  /*********************************/
  free_irq(evt2irq(IRQEVT_IRQ2), NULL);
  
  hl_irq_flags = 0;
  return 0;
}

// driver init
//------------------------------------------------------------------
int __init hl_irq_init(void)
{
  printk("HL_IRQ init\n");
  
  if (register_chrdev(HL_IRQ_MAJOR,"hl_irq",&hl_irq_fops)) 
  {
    printk("HL_IRQ: unable to get major %d for HL_IRQ\n",
           HL_IRQ_MAJOR);
    return -EIO;
  }
  return 0;
}

// driver exit
//------------------------------------------------------------------
int hl_irq_cleanup(void)
{
  unregister_chrdev(HL_IRQ_MAJOR, "hl_irq");
  return 0;
}

//------------------------------------------------------------------
int __init hl_irq_module_init(void)
{
  return hl_irq_init();
}

//------------------------------------------------------------------
static void __exit hl_irq_module_exit(void)
{
  hl_irq_cleanup();
  return;
}

module_init(hl_irq_module_init);
module_exit(hl_irq_module_exit);
