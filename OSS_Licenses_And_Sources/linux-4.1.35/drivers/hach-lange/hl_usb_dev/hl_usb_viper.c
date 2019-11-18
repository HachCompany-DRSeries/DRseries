/***************************************************************************
                          usb_isp1181b.c  -  description
                             -------------------
    begin                : Fre Sep 24 2004
    copyright            : (C) 2004 by Thomas Siegmund
    email                : tsiegmund@hach-lange.de
***************************************************************************/

#ifndef __KERNEL__
#define __KERNEL__
#endif

/* IRQF_DISABLED not supported in Linux 4.1 (not needed either) */
#if !defined(IRQF_DISABLED)
#define IRQF_DISABLED 0x00
#endif

//#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/module.h>
//#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/version.h>
//#include <linux/smp_lock.h>
#ifdef CONFIG_DEVFS_FS
#include <linux/devfs_fs_kernel.h>
#endif
#include <linux/proc_fs.h>
#include <linux/poll.h>

/* If you want debugging uncomment: */
/* #define DEBUG */
//#define DPRINT(format, args...) printk(DEVICE_NAME "%d: " format, current_drive , ## args)

#include <linux/init.h>
#include <asm/uaccess.h>
// #include <asm/sc1000.h>  //  /opt/Embedix/usr/local/sh3-linux/include
#include <asm/io.h>
#include <asm/ioctl.h>
//#include <linux/ioport.h>
#include <asm/irq.h>
#include <linux/usb.h>    // /opt/Embedix/home/project/gamma_tsg/build/rpmdir/BUILD/linux/include/linux
//#include <linux/lvm.h>
#include <asm/io.h>

#include <linux/time.h>

/**** own headers ************/
#include "hl_usb_reg.h"
#include "hl_usb_drv.h"

//#include "UsbDescriptor.h"
#include "hl_usb_ioctl.h"
#include "hl_usb_viper.h"
#include "hl_usb_state.h"
#include "hl_usb_core.h"
#include "hl_usb_version.h"
#include "hl_usb_drv.c"

/*********************************
 ** defines
 *********************************/
#define USB_ISP1181B_IRQ evt2irq(IRQEVT_IRQ3)
#define INTREQ00         0xA4140024
#define ICR1             0xA414001C
#define PSELD            0xA4050154
#define IRQEVT_IRQ3	 0x660

/*******************************
** externs
*******************************/
extern const int usb_isp1181b_tx_endpoint_addr[];
extern const int usb_isp1181b_rx_endpoint_addr[];

/***********************************************/
static int usb_initialized = 0; 
static int USB_ISP1181B_interrupt_handler_installed = 0; 
static int gBulkOutTransferCnt   = 0;
static DWORD  ReadDataStreamSize = 0;
struct UsbInTransferTimer_st   meastimer;
struct UsbInTransferTimer_st   bulkintimer;

ushort *port_ctrl = 0;
ushort *port_data = 0;
struct resource *region_ctrl = 0;
struct resource *region_data = 0;

/***********************************************/
#ifdef _SELF_MODULE_UNLOAD_
static int EIerror;
#endif
/***********************************************/
#ifdef DEBUG
int package=0;
//int usb_debug = 2;

char* getEPstate(int state)
{
  switch(state)  {
    case EP_NOT_ASSIGNED:   return "EP_NOT_ASSIGNED";
    case EP_ENABLED:        return "EP_ENABLED";
    case EP_DISABLED:       return "EP_DISABLED";
    case EP_IN_PROGRESS:    return "EP_IN_PROGRESS";
    case EP_STALL:          return "EP_STALL";
    default:                return "??? unknown ???";
  }
}

/* void prn(BYTE state)  */
/* {         */
/*   switch(state) {  */
/*     case ST_RESET:    */
/*       PDBG("\nState is: ST_RESET\n");  */
/*       break; */
/*     case ST_SUSPEND:  */
/*       PDBG("\nState is: ST_SUSPEND\n");  */
/*       break;  */
/*     case ST_RESUME:   */
/*       PDBG("\nState is: ST_RESUME\n");  */
/*       break;  */
/*     case ST_OPERAT:   */
/*       PDBG("\nState is: ST_OPERAT\n");  */
/*       break;  */
/*   }  */
/* } */
#endif

/*--------------------------------------------------------------------------*/
void StartCheckTimer(struct timeval *checkTV)
{
  do_gettimeofday(checkTV);  
}

/*--------------------------------------------------------------------------*/
int CheckTimerUS(struct timeval *checkTV)
{
  struct timeval  tv;
  long            readTimeUS; // micro sec
  
  do_gettimeofday(&tv);
  
  // usec overrun (expected times < 1s)
  if (tv.tv_usec < checkTV->tv_usec)
    readTimeUS = 1000000 - (checkTV->tv_usec - tv.tv_usec);
  else
    readTimeUS = tv.tv_usec - checkTV->tv_usec;
  
  return readTimeUS;
}

/***************************************************************
**** Cntrl transfer device
****************************************************************/
void IntCrtlOutTransfer(void)
{
  gusbtransf.CtrlTfout.command      = USB_NO_COMMAND;
  gusbtransf.CtrlTfout.dataSize     = 0;
  gusbtransf.CtrlTfout.rest2process = 0;
  gusbtransf.CtrlTfout.buffcnt      = 0;
  gusbtransf.CtrlTfout.ready        = NO;
  gusbtransf.CtrlTfout.exitOnZLP    = NO;

//  gusbtransf.CtrlTfout.exitSendZLP = NO;
//  gusbtransf.CtrlTfout.userbreak
  gusbtransf.CtrlTfout.TransfActive = NO;
}

static unsigned int usb_isp1181b_poll_cntrl(struct file *filp, poll_table *wait)
{
  //no data to read
  unsigned int mask = 0;
  //BYTE fnh = GetUsbDevReg(FNH);
  PDBG("%s...\n",__FUNCTION__);
  
  poll_wait(filp, &gusbtransf.CtrlTfout.waitq, wait);

  spin_unlock_wait(&gusbtransf.CtrlTfout.lock);
  spin_lock(&gusbtransf.CtrlTfout.lock);

  if(gusbtransf.CtrlTfout.buffcnt)
    mask = POLLIN | POLLRDNORM;

  /*  //write
      if(bulkstate.recbufcnt)
      mask = POLLOUT | POLLWRNORM;
  */
  if(gusbtransf.CtrlTfout.buffcnt == CTRL_MAX_BUFFER_SIZE)
    mask |= POLLPRI;

  //all data were read
  if(gusbtransf.CtrlTfout.ready == YES) { mask |= POLLHUP; }

  spin_lock(&DevState.lock);
  if(DevState.UsbState.BusState == USB_DISCONNECTED) 
  {
    mask |= POLLERR;
    PDBG("POLLERR\n");
    deactivate_usb();
    reset_usb();
  }
  spin_unlock(&DevState.lock);	
  spin_unlock(&gusbtransf.CtrlTfout.lock);
  
  return mask;
}
/************************************************************************************
 ** if end of trasnfer with zero length package then store size of data and
 ** user must read via IOCTL this value
 *************************************************************************************/
static int usb_isp1181b_read_cntrl(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
  static DWORD len =0;
  
  wait_queue_t wait;
  ssize_t read, ret;
  PDBG("%s...\n",__FUNCTION__);
  
  //for blocking read!!
  init_waitqueue_entry(&wait, current);
  add_wait_queue(&gusbtransf.CtrlTfout.waitq, &wait);
  current->state = TASK_INTERRUPTIBLE;

  // from "pipe.c" (MS: this was removed from pipe.c)
  // Seeks are not allowed on pipes.
  // ret = -ESPIPE;
  read = 0;
  // if(f_pos != &filp->f_pos)  { goto out_nolock; PDBG("ERROR:  f_pos != &filp->f_pos\n"); }
	  
  // Always return 0 on null read.
  ret = 0;
  if(count == 0)   goto out_nolock;

  do 
  {
    //get the spin lock
    spin_unlock_wait(&gusbtransf.CtrlTfout.lock);
    
    spin_lock_irq(&gusbtransf.CtrlTfout.lock);
    //check the bulk command maybe superfluous
    if(gusbtransf.CtrlTfout.command != CTRL_DATA_OUT) 
    {
      //wrong command
      ret = -EFAULT;
      goto spin_unlock_and_out;
    }

    gusbtransf.CtrlTfout.TransfActive = YES;

    if(gusbtransf.CtrlTfout.buffcnt) 
    {
      if(copy_to_user(buf+len, gusbtransf.CtrlTfout.buffer, gusbtransf.CtrlTfout.buffcnt))  
      {
        ret = -EFAULT;
        goto spin_unlock_and_out;
      }
      
      //clear buffer
      memset(gusbtransf.CtrlTfout.buffer, 0, CTRL_MAX_BUFFER_SIZE);
      
      read = gusbtransf.CtrlTfout.buffcnt;
      gusbtransf.CtrlTfout.buffcnt = 0;
      
      len += read;
      read = len;
      *f_pos += gusbtransf.CtrlTfout.buffcnt;
      
      goto check_ready_and_out;
    }
    else 
    {
      /*nothing to do*/  
    }
    
    //non blocking
    if(filp->f_flags & O_NONBLOCK)  
    {
      ret = -EAGAIN;
      goto check_ready_and_out;
    }

    //blocking
    if(signal_pending(current))   
    {
      ret = -ERESTARTSYS;
      goto check_ready_and_out;
    }

    schedule();
  } while(1);

 check_ready_and_out:
  if(gusbtransf.CtrlTfout.ready == YES) {//OK
    //we finished with a zero length package
    ReadDataStreamSize = len;
    //we MUST return with 0
    len = 0;
    //poll should give back POLLHUP
    read = len;
    
    zero_length_data_responseEP0();
  }
  
 spin_unlock_and_out:
  //give back spin lock
  //spin_unlock_irqrestore(&bulkstate.lock, flags);
  spin_unlock_irq(&gusbtransf.CtrlTfout.lock);

 out_nolock:
  current->state = TASK_RUNNING;
  remove_wait_queue(&gusbtransf.CtrlTfout.waitq, &wait);
  
  if(read) 
    ret = read;

  //PDBG("return ret: %d  and go out read: %d \n", ret, read);
  return ret;
}
/***************************************************************
**** Interrupt transfer device
****************************************************************/
void InitCmdInTransfer(void)
{
  gusbtransf.IntCmdTf.command      = USB_NO_COMMAND;
  gusbtransf.IntCmdTf.dataSize     = 0;
  gusbtransf.IntCmdTf.rest2process = 0;
  gusbtransf.IntCmdTf.buffcnt      = 0;
  gusbtransf.IntCmdTf.ready        = NO;
  gusbtransf.IntCmdTf.exitOnZLP    = NO;
  gusbtransf.IntCmdTf.TransfActive = NO;
}
/*****************************************************************/
/******************   bulk data read   ***************************/
/*****************************************************************/
void InitBulkOutTransfer(void)   //read
{
  PDBG("%s...\n",__FUNCTION__);
  //when there is a vendor request with an actually size to read or send
  //this will be set to zero      default command
  gusbtransf.BulkTfout.command      = BULK_DATA_OUT_STREAM; //USB_NO_COMMAND;
  gusbtransf.BulkTfout.dataSize     = 0;
  gusbtransf.BulkTfout.rest2process = 0;
  gusbtransf.BulkTfout.buffcnt      = 0;
  gusbtransf.BulkTfout.ready        = NO;
  gusbtransf.BulkTfout.exitOnZLP    = NO;
  //  gusbtransf.BulkTfout.exitSendZLP = NO;
  //  gusbtransf.BulkTfout.userbreak
  gusbtransf.BulkTfout.TransfActive = NO;
  
  InitCmdInTransfer();
}
/***************************************************************************/
static unsigned int usb_isp1181b_poll_bulk(struct file *filp, poll_table *wait)
{   //no data to read
  unsigned int mask = 0;

  PDBG("%s...\n",__FUNCTION__);
  poll_wait(filp, &gusbtransf.BulkTfout.waitq, wait);
  
  spin_unlock_wait(&gusbtransf.BulkTfout.lock);
  // spin_lock(&gusbtransf.BulkTfout.lock);
  spin_lock_irq(&gusbtransf.BulkTfout.lock);
  
  if(gusbtransf.BulkTfout.buffcnt)  
  {
    mask = POLLIN | POLLRDNORM;  
  }
  
  /*  //write
      if(bulkstate.recbufcnt)
      mask = POLLOUT | POLLWRNORM;
  */
  if(gusbtransf.BulkTfout.buffcnt == BULK_MAX_BUFFER_SIZE)
    mask |= POLLPRI;
  
  //all data were read
  if(gusbtransf.BulkTfout.ready == YES) 
  { 
    mask |= POLLHUP; 
  }

  spin_lock(&DevState.lock);
  if(DevState.UsbState.BusState == USB_DISCONNECTED) 
  {
    mask |= POLLERR;
    PDBG("POLLERR\n");
    deactivate_usb();
    reset_usb();
  }
  spin_unlock(&DevState.lock);
  
  spin_unlock_irq(&gusbtransf.BulkTfout.lock);
    
  return mask;
}

/************************************************************************************
 ** if end of trasnfer with zero length package then store size of data and
 ** user must read via IOCTL this value
 *************************************************************************************/
static int usb_isp1181b_read_bulk(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
  //unsigned long flags = 0;
  static DWORD len =0;

  wait_queue_t wait;
  ssize_t read, ret;

  PDBG("%s...\n",__FUNCTION__);

#ifdef DEBUG
  package = 0;
#endif


  //for blocking read!!
  init_waitqueue_entry(&wait, current);
  add_wait_queue(&gusbtransf.BulkTfout.waitq, &wait);
  PDBG("%s: add_wait_queue\n",__FUNCTION__);
  current->state = TASK_INTERRUPTIBLE;

  // from "pipe.c" (MS: this was removed from pipe.c)
  // Seeks are not allowed on pipes.
  // ret = -ESPIPE;  
  read = 0;
  // if(f_pos != &filp->f_pos)
  // {
  // PDBG("ERROR:  f_pos != &filp->f_pos\n");
  // goto out_nolock;
  // }

  // Always return 0 on null read.
  ret = 0;
  if(count == 0)
    goto out_nolock;

  do
  { //get the spin lock
    spin_unlock_wait(&gusbtransf.BulkTfout.lock);      
    spin_lock_irq(&gusbtransf.BulkTfout.lock);
    //spin_lock_irqsave(&bulkstate.lock, flags);

    gusbtransf.BulkTfout.TransfActive = YES;

    //check the bulk command maybe superfluous
    if(gusbtransf.BulkTfout.command != BULK_DATA_OUT_STREAM && 
       gusbtransf.BulkTfout.command != BULK_DATA_OUT_AMOUNT)
    { //wrong command
      // PDBG("ERR: %s: wrong cmd\n",__FUNCTION__);
      ret = -EFAULT;
      goto spin_unlock_and_out;
    }

    if(gusbtransf.BulkTfout.buffcnt)
    {
      if(copy_to_user(buf+len, gusbtransf.BulkTfout.buffer, gusbtransf.BulkTfout.buffcnt))
      {
        /* PDBG("ERR: %s: copy_to_user(trg=%p,+len=%d,src=%p,cnt=%d)\n",__FUNCTION__,buf,len, */
        /*       gusbtransf.BulkTfout.buffer,gusbtransf.BulkTfout.buffcnt); */
        ret = -EFAULT;
        goto spin_unlock_and_out;
      }
      
      //clear buffer
      memset(gusbtransf.BulkTfout.buffer, 0, BULK_MAX_BUFFER_SIZE);
      len += gusbtransf.BulkTfout.buffcnt;
      gusbtransf.BulkTfout.buffcnt = 0;
      
      read = len;
      *f_pos += gusbtransf.BulkTfout.buffcnt; // reduces page errors in copy_to_user
      
      goto check_ready_and_out;
    }
    else
    {// bulkstate.recbufcnt == 0 => go further
    }

    //non blocking
    if( filp->f_flags & O_NONBLOCK )  
    {
      ret = -EAGAIN;
      goto check_ready_and_out;
    }

    //blocking
    if( signal_pending(current) ) 
    {
      ret = -ERESTARTSYS;
      goto check_ready_and_out;
    }
    
    schedule();
  } while(1);

  
 check_ready_and_out:
  if(gusbtransf.BulkTfout.ready == YES)
  {//OK
    //PDBG("..............ready in read !!!!! len: %d\n", len);
    //we finished with a zero length package
    ReadDataStreamSize = len;
    //we MUST return with 0
    len = 0;
    //poll should give back POLLHUP
    read = len;
    
    // SetStallEpx(EPT2); soll wieder rein
    if(gusbtransf.BulkTfout.command == BULK_DATA_OUT_AMOUNT)
    {
      InitBulkOutTransfer();
    }
  }
  
 spin_unlock_and_out:
  //give back spin lock
  //spin_unlock_irqrestore(&bulkstate.lock, flags);
  spin_unlock_irq(&gusbtransf.BulkTfout.lock);

 out_nolock:
  current->state = TASK_RUNNING;
  remove_wait_queue(&gusbtransf.BulkTfout.waitq, &wait);
  PDBG("%s: remove_wait_queue\n",__FUNCTION__);

  if(read) ret = read;
  // PDBG("read return ret: %d  and go out: %d \n", ret, read);
  return ret;
}
/**************************************************************/
void InitMeasInTransfer(void)
{
  gusbtransf.IntMeasTf.command      = USB_NO_COMMAND;
  gusbtransf.IntMeasTf.dataSize     = 0;
  gusbtransf.IntMeasTf.rest2process = 0;
  gusbtransf.IntMeasTf.buffcnt      = 0;
  gusbtransf.IntMeasTf.ready        = NO;
  gusbtransf.IntMeasTf.exitOnZLP    = NO;
  // gusbtransf.IntMeasTf.exitSendZLP = NO;
  // gusbtransf.IntMeasTf.userbreak
  gusbtransf.IntMeasTf.TransfActive = NO;
}
/*****************************************************
** MeasDataTimeOut
******************************************************/
void MeasDataTimeOut(unsigned long  arg)
{
  spin_unlock_wait(&gusbtransf.IntMeasTf.lock);
  spin_lock(&gusbtransf.IntMeasTf.lock);

  gusbtransf.IntMeasTf.timer->bTimeOut = YES;

  deactivate_usb();
  reset_usb();
  
  PDBG("Error: USB disconnected or application was not started!!!\n");
  PDBG("MeasDataTimeOut  time elapsed: %Lu ms\n",(unsigned long long) gMeasurementTimeOut*10 -
       (jiffies - gusbtransf.IntMeasTf.timer->TimerList.expires));
  spin_unlock(&gusbtransf.IntMeasTf.lock);
  wake_up_interruptible(&gusbtransf.IntMeasTf.waitq);
}
/*****************************************************
 ** set timeout for write
 ** jiffies resolution = 10 ms
 ******************************************************/
void SetMeasSendDataTimeout(void)
{
  if(gusbtransf.IntMeasTf.timer->bTimerpresent == YES)  
  {
    init_timer(&gusbtransf.IntMeasTf.timer->TimerList);
    gusbtransf.IntMeasTf.timer->TimerList.function = MeasDataTimeOut;
    //         bulkstate.TimerMeasr.data = ep_no;
    gusbtransf.IntMeasTf.timer->TimerList.expires = jiffies + gMeasurementTimeOut;
    
    gusbtransf.IntMeasTf.timer->bTimeOut = NO;
    add_timer(&gusbtransf.IntMeasTf.timer->TimerList);
  }
}
/*******************************************************************************************************/
static int usb_isp1181b_write_measurement(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
  int  bytes2transfer;
  int  ret;
  wait_queue_t wait;

  PDBG("%s...\n",__FUNCTION__);
  //  PDBG("Expected package: %d  in usb_isp1181b_write_measurement\n", ++package);

  spin_unlock_wait(&gusbtransf.IntMeasTf.lock);
  spin_lock_irq(&gusbtransf.IntMeasTf.lock);

  if( 0 <= gusbtransf.IntMeasTf.dataSize ) 
  {
    //1st entry in write
    ret = bytes2transfer = 0;
		
    if( TX_INTR_EP_FIFO_SIZE_MEAS < gusbtransf.IntMeasTf.dataSize ) 
    {
      //optimization
      ret = -EFAULT;
      goto cleanup_out;
    }
    else 
    { 
      bytes2transfer = gusbtransf.IntMeasTf.dataSize; 
    }
		
    gusbtransf.IntMeasTf.exitOnZLP = (gusbtransf.IntMeasTf.dataSize % TX_INTR_EP_FIFO_SIZE_MEAS) ? NO : YES;

    //alloc memory
    if( !(gusbtransf.IntMeasTf.buffer = kmalloc(bytes2transfer, GFP_KERNEL)) ) 
    {
      ret = -ENOMEM;
      goto cleanup_out;
    }
  }
  else 
  {
    ret = -EFAULT;
    goto cleanup_out;
  }

  //get user data
  if( copy_from_user(gusbtransf.IntMeasTf.buffer, buf, bytes2transfer) ) 
  {
    ret = -EFAULT;
    goto cleanup_out;
  }   

  //set time out
  SetMeasSendDataTimeout();

  //give back spin lock
  //USED TO BE HERE: spin_unlock_irq(&gusbtransf.IntMeasTf.lock);	 
  ret = SendInterruptData(gusbtransf.IntMeasTf.buffer, bytes2transfer, EPT5);

  //wait for wake up signal
  init_waitqueue_entry(&wait, current);
  current->state = TASK_INTERRUPTIBLE;
  add_wait_queue(&gusbtransf.IntMeasTf.waitq, &wait);
  
  //give back spin lock
  spin_unlock_irq(&gusbtransf.IntMeasTf.lock);	 
  schedule();

  remove_wait_queue(&gusbtransf.IntMeasTf.waitq, &wait);
  current->state = TASK_RUNNING;

  spin_unlock_wait(&gusbtransf.IntMeasTf.lock);
  spin_lock_irq(&gusbtransf.IntMeasTf.lock);
  if( YES == gusbtransf.IntMeasTf.timer->bTimerpresent && YES == gusbtransf.IntMeasTf.timer->bTimeOut ) 
  {
    //time out
    ret = -ETIME;
    goto cleanup_out;
  }
     	
  if( ret != gusbtransf.IntMeasTf.dataSize ) 
  {
    ret = -EFAULT;
    goto cleanup_out;
  }
	 
 cleanup_out:
  if( NULL != gusbtransf.IntMeasTf.buffer )  
  {
    kfree(gusbtransf.IntMeasTf.buffer);
    gusbtransf.IntMeasTf.buffer = NULL;
  }
  
  InitMeasInTransfer();
  spin_unlock_irq(&gusbtransf.IntMeasTf.lock);
  
  return ret;
}
/***********************************************************************/
void InitBulkInTransfer(void)
{
  gusbtransf.BulkTfin.command      = BULK_DATA_IN_STREAM;
  gusbtransf.BulkTfin.dataSize     = 0;
  gusbtransf.BulkTfin.rest2process = 0;
  gusbtransf.BulkTfin.buffer       = NULL;
  gusbtransf.BulkTfin.buffcnt      = 0;
  gusbtransf.BulkTfin.ready        = NO;
  gusbtransf.BulkTfin.exitOnZLP    = NO;
//  gusbtransf.BulkTfout.exitSendZLP = NO;
//  gusbtransf.BulkTfout.userbreak
  gusbtransf.BulkTfin.TransfActive = NO;
}
/*****************************************************
** BulkDataTimeOut
******************************************************/
void BulkDataTimeOut(unsigned long arg)
{
  // spin_unlock_wait(&gusbtransf.BulkTfin.lock);
  spin_lock(&gusbtransf.BulkTfin.lock);
  
  gusbtransf.BulkTfin.timer->bTimeOut = YES;
  
  deactivate_usb();
  reset_usb();
  
  PDBG("Error: USB disconnected or application was not started!!!\n");
  PDBG("BulkDataTimeOut  time elapsed: %Lu ms\n",(unsigned long long) gBulkInTimeOut*10 -
       (jiffies - gusbtransf.BulkTfin.timer->TimerList.expires));
  
  spin_unlock(&gusbtransf.BulkTfin.lock);
  wake_up_interruptible(&gusbtransf.BulkTfin.waitq);
}
/*****************************************************
** set timeout for write
** jiffies resolution = 10 ms
******************************************************/
void SetBulkSendDataTimeout(void)
{
  if( YES == gusbtransf.BulkTfin.timer->bTimerpresent )    
  {
    init_timer(&gusbtransf.BulkTfin.timer->TimerList);
    gusbtransf.BulkTfin.timer->TimerList.function = BulkDataTimeOut;
    gusbtransf.BulkTfin.timer->TimerList.expires = jiffies + gBulkInTimeOut;
    
    gusbtransf.BulkTfin.timer->bTimeOut = NO;
    add_timer(&gusbtransf.BulkTfin.timer->TimerList);
  }
}
/*****************************************************************************************************/
static int usb_isp1181b_write_bulk(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
  int    rslt;
  DWORD  bytes2copy        = 0;
  DWORD  bytestransferred  = 0;
  static int   if_pos      = 0;
  static int   first       = YES;  
  wait_queue_t wait;
  
  PDBG("%s[%d]...\n",__FUNCTION__,__LINE__);
  
  // spin_unlock_wait(&gusbtransf.BulkTfin.lock);
  spin_lock_irq(&gusbtransf.BulkTfin.lock);
  
  if( NULL == gusbtransf.BulkTfin.buffer && 0 < count )  
  {
    //1st entry in write    		
    if( first ) 
    {		   
      gusbtransf.BulkTfin.TransfActive = YES;
      gusbtransf.BulkTfin.dataSize     = count;        
      first = NO;
    }
    
    if( !(gusbtransf.BulkTfin.command == BULK_DATA_IN_AMOUNT || 
          gusbtransf.BulkTfin.command == BULK_DATA_IN_STREAM ) ) 
    { 
      //wrong command
      spin_unlock_irq(&gusbtransf.BulkTfin.lock);
      return -EPERM;
    }
    
    //how much we can send out??
    if( count > BULK_MAX_BUFFER_SIZE )    
    {
      bytes2copy = BULK_MAX_BUFFER_SIZE;
    }
    else   
    {
      bytes2copy = count;
    }
    
    gusbtransf.BulkTfin.rest2process = bytes2copy;
    
    //alloc memory
    if( !(gusbtransf.BulkTfin.buffer = kmalloc(bytes2copy, GFP_KERNEL)) )   
    {
      spin_unlock_irq(&gusbtransf.BulkTfin.lock);
      return -ENOMEM;
    }
  }  
  else if ( gusbtransf.BulkTfin.buffer == NULL && count <= 0 )  
  {
    spin_unlock_irq(&gusbtransf.BulkTfin.lock);
    InitBulkInTransfer();
    return -EINVAL;
  }
  
  if_pos += bytes2copy;
  
  if( if_pos == gusbtransf.BulkTfin.dataSize ) 
  {
    if_pos = 0;
    first  = NO;			
    //Do we have to send a ZLP??
    if( BULK_DATA_IN_STREAM  == gusbtransf.BulkTfin.command ) 
    {
      gusbtransf.BulkTfin.exitOnZLP = (gusbtransf.BulkTfin.dataSize % TX_BULK_EP_FIFO_SIZE) ? NO : YES;
    }        
  }
  
  //clear buffer
  memset(gusbtransf.BulkTfin.buffer, 0, bytes2copy);
  gusbtransf.BulkTfin.buffcnt = 0;
  
  //get user data
  if( copy_from_user(gusbtransf.BulkTfin.buffer, buf, bytes2copy) )	
  {
    kfree(gusbtransf.BulkTfin.buffer);   
    gusbtransf.BulkTfin.buffer = NULL;    	
    bytestransferred = -ETIME; 
    goto out;
  }
  
  //give back spin lock
  spin_unlock_irq(&gusbtransf.BulkTfin.lock);     	
  
  //wait for wake up signal
  init_waitqueue_entry(&wait, current);
  current->state = TASK_INTERRUPTIBLE;
  add_wait_queue(&gusbtransf.BulkTfin.waitq, &wait);
  
  SendBulkData(); 
  
  rslt = schedule_timeout(1000);

  //spin_unlock_wait(&gusbtransf.BulkTfin.lock);
  spin_lock_irq(&gusbtransf.BulkTfin.lock);
  
  remove_wait_queue(&gusbtransf.BulkTfin.waitq, &wait);		
  current->state = TASK_RUNNING;
  
  if (!rslt){
    gusbtransf.BulkTfin.command      = USB_NO_COMMAND;
    gusbtransf.BulkTfin.dataSize     = 0;
    gusbtransf.BulkTfin.rest2process = 0;
    gusbtransf.BulkTfin.buffcnt      = 0;
    gusbtransf.BulkTfin.ready        = NO;
    gusbtransf.BulkTfin.exitOnZLP    = NO;
    gusbtransf.BulkTfin.ready        = NO;
    config_usb();
    bytestransferred = -EAGAIN;
  }
  else if (gusbtransf.BulkTfin.timer->bTimeOut == YES)
    bytestransferred = -ETIME;
  else
    bytestransferred = bytes2copy;		
  
  PDBG("WRITE_BULK_FREE\n");
  kfree(gusbtransf.BulkTfin.buffer);
  gusbtransf.BulkTfin.buffer = NULL;
  
 out:
  gBulkInTimeOut = 0;
  
  if ( if_pos == gusbtransf.BulkTfin.dataSize || 0 > bytestransferred ) 
  {
    InitBulkInTransfer();   
  }     
  
  spin_unlock_irq(&gusbtransf.BulkTfin.lock);
  
  // PDBG("returned  bytestransferred: %d \n", bytestransferred);
  return bytestransferred;
}

typedef unsigned int io_data;

/******************************************************************************************************/
static long usbdev_ioctl(struct file *file, 
                         unsigned int cmd, unsigned long arg)
{
  int ret = 0;
  // DWORD io_data;

  PDBG("%s...\n",__FUNCTION__);

  /*************************************************
   ** IOCTL codes are tested as well as the pointers 
   **************************************************/
  if(_IOC_TYPE(cmd) != USB_IO_MAGIC) { PDBG("Wrong IOCTL Code\n"); return -ENOTTY;} 
  if(_IOC_NR(cmd) >= USB_IO_IOCTL_MAX) { PDBG("Wrong IOCTL number\n"); return -ENOTTY;} 
  if(_IOC_DIR(cmd) & _IOC_READ)
    ret = !access_ok(VERIFY_WRITE, (void*)arg, _IOC_SIZE(cmd));
  else if(_IOC_DIR(cmd) & _IOC_WRITE)	
    ret = !access_ok(VERIFY_READ, (void*)arg, _IOC_SIZE(cmd));
  
  if(ret){PDBG("Error Wrong address & pointer in IOCTL codes!!\n"); return -EFAULT;}   

  switch(cmd)   
  {
    /*
      case USB_IO_USER_BREAK:
      //  ret = __get_user(bulkstate.userbreak, (int*)arg);
      //  wake up all (read / write)
      bulkstate.ready = YES;
      wake_up_interruptible(&bulkstate.sendq);
      wake_up_interruptible(&bulkstate.recq);
      #ifdef  EVENT_CONTROL
      wake_up_interruptible(&bulkstate.cmdq);
      #endif
      //  PDBG("USB_IO_USER_BREAK\n");
      break;
    */
    case USB_IO_GET_READ_STREAM_SIZE:
      ret = __put_user(ReadDataStreamSize, (DWORD*)arg);
      PDBG("ReadDataStreamSize: %d\n", ReadDataStreamSize);
      ReadDataStreamSize = 0;
		 
      if( YES == gusbtransf.CtrlTfout.ready )
      {//we have to do it here, so that the POLLHIUP
        IntCrtlOutTransfer();
      }

      if( YES ==gusbtransf.BulkTfout.ready )
      {//we have to do it here, so that the POLLHIUP
        InitBulkOutTransfer();
      }
      break;

    case USB_IO_SET_TIMEOUT_WRITE_MEASUREMENT:
      ret = __get_user(gMeasurementTimeOut, (int*)arg);

      if(gMeasurementTimeOut > 0)
      {
        gusbtransf.IntMeasTf.timer->bTimerpresent = YES;
        //because of HZ -> 100/s
        gMeasurementTimeOut = gMeasurementTimeOut/10;
      }

      PDBG("USB_IO_SET_TIMEOUT_WRITE: %d ms bTimerAlive: %s\n",
           gMeasurementTimeOut*10, (gusbtransf.IntMeasTf.timer->bTimerpresent) ? "YES" : "NO");
      break;

    case USB_IO_SET_TIMEOUT_WRITE_BULK:
      ret = __get_user(gBulkInTimeOut, (int*)arg);

      if(gBulkInTimeOut > 0)
      {
        gusbtransf.BulkTfin.timer->bTimerpresent = YES;
        //because of HZ -> 100/s
        gBulkInTimeOut = gBulkInTimeOut/10;
      }
      gusbtransf.BulkTfin.command = BULK_DATA_IN_STREAM;

      PDBG("USB_IO_SET_TIMEOUT_BULK: %d ms bTimerAlive: %s\n",
           gBulkInTimeOut*10, (gusbtransf.BulkTfin.timer->bTimerpresent) ? "YES" : "NO");
      break;

    case USB_IO_GET_COMMAND:
      //if we get more commands to handle, maybe we need a switch or something else
      ret = __put_user(gusbtransf.BulkTfin.command, (DWORD*)arg);
      break;
      /*
        case USB_IO_GET_BULK_DATA_SIZE:
        //  PDBG("USB_IO_GET_BULK_DATA_SIZE 1st \n");
        #ifdef  EVENT_CONTROL
        init_waitqueue_entry(&wait, current);
		  
        current->state = TASK_INTERRUPTIBLE;
        add_wait_queue(&bulkstate.cmdq, &wait);
		  
        schedule();
        remove_wait_queue(&bulkstate.cmdq, &wait);
        #endif
        //  PDBG("USB_IO_GET_BULK_DATA_SIZE: %d\n",bulkstate.dataSize);
        io_data = bulkstate.dataSize;
        ret = __put_user(io_data, (DWORD*)arg);
        break;
      */
    case USB_IO_SET_MEAS_DATA_SIZE:
      //size of data
      spin_unlock_wait(&gusbtransf.IntMeasTf.lock);
      spin_lock(&gusbtransf.IntMeasTf.lock);

      ret = __get_user(gusbtransf.IntMeasTf.dataSize, (DWORD*)arg);
      gusbtransf.IntMeasTf.rest2process = gusbtransf.IntMeasTf.dataSize;
      gusbtransf.IntMeasTf.command = BULK_DATA_IN_AMOUNT;

      gusbtransf.IntMeasTf.TransfActive = YES;

      spin_unlock(&gusbtransf.IntMeasTf.lock);

      PDBG(" USB_IO_SET_MEAS_DATA_SIZE gusbtransf.IntMeasTf.dataSize: %d\n\n\n", gusbtransf.IntMeasTf.dataSize);
      break;

    case USB_IO_BULK_DATA_IN:
      spin_unlock_wait(&gusbtransf.IntCmdTf.lock);
      spin_lock_irq(&gusbtransf.IntCmdTf.lock);
       
      ret = __get_user(gusbtransf.IntCmdTf.dataSize, (DWORD*)arg);
      gusbtransf.IntCmdTf.rest2process = gusbtransf.IntCmdTf.dataSize;
      gusbtransf.IntCmdTf.command = BULK_DATA_IN_AMOUNT;

      gusbtransf.IntCmdTf.TransfActive = YES;         

      spin_lock(&DevState.lock);
      DevState.UsbState.EpState[EPT3] = EP_IN_PROGRESS;
      spin_unlock(&DevState.lock);     		 
      PDBG("\nUSB_IO_BULK_DATA_IN gusbtransf.IntCmdTf.dataSize: %d\n\n", gusbtransf.IntCmdTf.dataSize);

#ifdef DEBUG		 
      if(&gusbtransf.IntCmdTf.dataSize == NULL)
      {PDBG("Error NULL ptr -> SendInterruptData() in IOCTL function\n"); return -EFAULT; }   
#endif
      spin_unlock_irq(&gusbtransf.IntCmdTf.lock);   
      //maybe we have to wait of for the return of the interrupt transfer: Answer NO
      SendInterruptData((BYTE*)&gusbtransf.IntCmdTf.dataSize, sizeof(gusbtransf.IntCmdTf.dataSize), EPT3);
      break;

    case USB_IO_GET_USB_STATUS:
      spin_lock(&DevState.lock);
      if( copy_to_user((BYTE*)arg, &DevState.UsbState, sizeof(DevState.UsbState)) )  {
        spin_unlock(&DevState.lock);
        return -EFAULT;
      }
      spin_unlock(&DevState.lock);
      break;
	 
    case USB_IO_GET_VERSION:
      PDBG("Get USB Version: %d\n", VERSION_USB);
      ret = __put_user(VERSION_USB, (DWORD*)arg);
      break;
    default:
      return -EINVAL;
      //break;
  }

  return ret;
}

//------------------------------------------------------------------
void HandleIrqEP0OUT(void)
{
  ushort statEvnt;
  ushort statErr;
  ushort sTmp;
  ushort err;
  int    len, i;

  PDBG("HandleIrqEP0OUT...\n");

  clear_control_buffer(&ctrl_receive_buf);
    
  statEvnt = UsbRd2B(ReadCtrlOutStat);
  statErr  = UsbRd2B(ReadCtrlOutErr);
  
  spin_lock(&DevState.lock);
  DevState.UsbState.EpState[EPT0] = EP_IN_PROGRESS;
  spin_unlock(&DevState.lock);
  
  
  /* ERROR */
  /*********/
  if (statErr & ERROR)
  {
    err = (statErr & ERROR) >> 1;
    
    switch (err)
    {
      case EPERR_NAK:
        ctrl_send_buf.byte_cnts = 0;  //exit multi-packet mode
        // MS: FLUSH_TX0;
        FlushRx(EPT0);
        break;
        
      default:
        PDBG("HandleIrqEP0OUT: error 0x%x not handled\n", err);
        break;
    }
  }
  
  /* SETUP received */
  /******************/
  if(statEvnt & SETUPT)
  {
    PDBG("%s: Ack SETUP\n",__FUNCTION__);
    UsbWr0B(AckSetup);
    
    // check for setup overwrite
    sTmp = UsbRd2B(ReadCtrlOutStat);
    if (sTmp & OVERWRITE)
    {
      sTmp = UsbRd2B(ReadCtrlOutErr);
      PDBG("%s: SETUP overwritten (err=0x%x)\n",__FUNCTION__,sTmp);
      FlushRx(EPT0);
    }
    else
    {
/*       UsbWr0B(UnstallCtrlOutEP); */
/*       UsbWr0B(UnstallCtrlInEP); */

      //clear Multi pack data size counter
      //device_buffers.DataSizMulPack = 0;
      gusbtransf.CtrlTfout.dataSize     = 0;
      gusbtransf.CtrlTfout.rest2process = 0;
      
      len = readfifo(reqfifo, EPT0, fifo_sizes[EPT0]);
      if (!len || (statErr & DATA01)) 
      {
        PDBG("INVALID OPERATION in SETUP -> flush FIFO!!\n");
        FlushRx(EPT0);
      }
      else if (len > fifo_sizes[EPT0])
      {
        PDBG("STRANGE SETUP: len(%d) > fifo_sizes[EPT0](%d)\n",
             len,fifo_sizes[EPT0]);
        len = fifo_sizes[EPT0];
      }
      
      //get data & adjust the size of data to be received
      ctrl_receive_buf.data      = (BYTE*) &reqfifo[0];
      ctrl_receive_buf.byte_cnts = len;
      ep_stat[EPT0]->toggle_bit  = 0x01;
      
      // MS: FLUSH_TX0;
      FlushRx(EPT0);
      
      spin_lock(&DevState.lock);
      DevState.UsbState.EpState[EPT0] = EP_ENABLED;
      spin_unlock(&DevState.lock);
      
      dev_req_handler();
    }
  } 
    
  /* OUT token received */
  /**********************/
  else  
  {
    //if(device_buffers.DataSizMulPack)
    if(gusbtransf.CtrlTfout.rest2process)  
    {

      statErr  = UsbRd2B(ReadCtrlOutErr);
      statEvnt = UsbRd2B(ReadCtrlOutStat);

      /* toggle bit ok */
      /*****************/
      if (((statErr & DATA01)>0) == ep_stat[EPT0]->toggle_bit)
      {
        // MS: if((rxstate & RX_LAST) && ((rxstate & RXTOGGLE) == Toggle)) 
          
        len = readfifo(reqfifo, EPT0, fifo_sizes[EPT0]);
        if (!len || (statErr & DATA01)) 
        {
          PDBG("INVALID OPERATION in OUT token -> flush FIFO!!\n");
          FlushRx(EPT0);
        }
        else if (len > fifo_sizes[EPT0])
        {
          PDBG("STRANGE OUT: len > fifo_sizes[EPT0]\n");
          len = fifo_sizes[EPT0];
        }
          
        //get data & adjust the size of data to be received
        ctrl_receive_buf.data      = (BYTE*) &reqfifo[0];
        ctrl_receive_buf.byte_cnts = len;
          
        gusbtransf.CtrlTfout.rest2process -= len;
          
        //size of 16 -> 2 times of transfer size of EP0, so there is time enough to copy all
        if(CTRL_MAX_BUFFER_SIZE < (gusbtransf.CtrlTfout.buffcnt + CTRL_2TIMES_TRANSFER_SIZE)) 
        {
          //wake up to copy to user space
          //so can save time
          wake_up_interruptible(&gusbtransf.CtrlTfout.waitq);
        }
          
        //set lock
        spin_unlock_wait(&gusbtransf.CtrlTfout.lock);
        spin_lock(&gusbtransf.CtrlTfout.lock);
          
        for(i=0; i<len; i++)
          gusbtransf.CtrlTfout.buffer[gusbtransf.CtrlTfout.buffcnt+i] = ctrl_receive_buf.data[i];
          
        gusbtransf.CtrlTfout.buffcnt += ctrl_receive_buf.byte_cnts;
          
        if(gusbtransf.CtrlTfout.rest2process == 0) 
        {
          gusbtransf.CtrlTfout.ready = YES;
          
          //we have to toggle the bit to "DATA 1"
          ep_stat[EPT0]->toggle_bit = 0x01;
            
          //because for spinlock in read() the interrupt to send zero length package
          //doesn't occur, so we have to send after the copy
          //gusbtransf.CtrlTfout.exitOnZLP = YES;
        }
        else 
        { 
          ep_stat[EPT0]->toggle_bit = ep_stat[EPT0]->toggle_bit ? 0 : 1; 
        }

        //unlock
        spin_unlock(&gusbtransf.CtrlTfout.lock);
          
        if(gusbtransf.CtrlTfout.ready == YES)  
        {
          //wake up for last raed
          wake_up_interruptible(&gusbtransf.CtrlTfout.waitq);
        }
      }
    }
      
    // MS: FLUSH_TX0;
    clear_control_buffer(&ctrl_send_buf); //clear multi package mode
      
    //re-enable receive of data
    FlushRx(EPT0);
  }
  DevState.UsbState.EpState[EPT0] = EP_ENABLED;
}

//------------------------------------------------------------------
void HandleIrqEP2(void)
{
  ushort  statEvnt;
  ushort  statErr;

  PDBG("%s...\n",__FUNCTION__);
  
  statErr  = UsbRd2B(ReadEP2Err);
  statEvnt = UsbRd2B(ReadEP2Stat);
  
  /* not in remote state (device not open) */
  /*****************************************/
  if(gusbtransf.BulkTfout.DevOpen == NO && 
     gusbtransf.IntMeasTf.DevOpen == NO) 
  {
    printk("USB device not open! GO IN REMOTE!\n");
    FlushRx(EPT2);
    return;
  }
    
  /* buf full */
  /************/
  if( /* (statEvnt & EPFULL0) || */ BufferFull()) 
  {
    PDBG("EP2: buffer full\n");
    //buffer is full -> now send a NAK to prevent the gamma for a crash
    wake_up_interruptible(&gusbtransf.BulkTfout.waitq);
    PDBG("%s: wake queue (1)\n",__FUNCTION__);
    FlushRx(EPT2);
  }
  spin_lock(&DevState.lock);
  DevState.UsbState.EpState[EPT2] = EP_IN_PROGRESS;
  spin_unlock(&DevState.lock);
  
  /* ERROR (NAK handled here) */
  /****************************/
  if(statErr & ERROR)
  {
    PDBG("EP2: ERR=0x%x\n",statErr);
    FlushRx(EPT2);      
    spin_lock(&DevState.lock);
    DevState.UsbState.EpState[EPT2] = EP_ENABLED;
    spin_unlock(&DevState.lock);
    return;
  }
  
  /* data received */
  /*****************/
  if (statErr & RTOK)
  {
    
    /* toggle bit ok */
    /*****************/
    if (((statErr & DATA01)>0) == ep_stat[EPT2]->toggle_bit)
    {
      /* zero length package */
      /***********************/
      int   tLen, len;
      int   dat;
      char *buf;

      len = UsbRd2B(ReadEP2Buf);
      if (len == 0)
      {
        spin_unlock_wait(&gusbtransf.BulkTfout.lock);
        spin_lock(&gusbtransf.BulkTfout.lock);
        gusbtransf.BulkTfout.ready = YES;
        PDBG("3 got a ZLP -> gusbtransf.BulkTfout.ready = YES\n");
        FlushRx(EPT2);
        spin_unlock(&gusbtransf.BulkTfout.lock);
        
        //reset toggle bit
        ep_stat[EPT2]->toggle_bit = 0;
        
        wake_up_interruptible(&gusbtransf.BulkTfout.waitq);
        PDBG("%s: wake queue (2)\n",__FUNCTION__);
      }
      
      /* data package */
      /****************/
      else
      {
        spin_unlock_wait(&gusbtransf.BulkTfout.lock);
        spin_lock(&gusbtransf.BulkTfout.lock);
  
        if(BULK_MAX_BUFFER_SIZE  == gusbtransf.BulkTfout.buffcnt)// + BULK_2TIMES_TRANSFER_SIZE))
        {//wake up to copy to user space so we can save time
          spin_unlock(&gusbtransf.BulkTfout.lock);
          wake_up_interruptible(&gusbtransf.BulkTfout.waitq);
          PDBG("%s: wake queue (3)\n",__FUNCTION__);
          return;
        }
        
        tLen = len = len>BULK_MAX_BUFFER_SIZE?BULK_MAX_BUFFER_SIZE:len;
        buf  = gusbtransf.BulkTfout.buffer;
        PDBG("EP2: RX(%d) 0x ",len);
        while (tLen > 0)
        {
          dat = UsbRdData2B();
          *buf++ = dat & 0xff;          // 1st byte
          PDBG("%02x ", *(buf-1));
          if (tLen > 1)
          {
            *buf++ = (dat >> 8) & 0xff; // 2nd byte
            PDBG("%02x ", *(buf-1));
          }
          tLen -= 2;
        }
        PDBG("\n");
        FlushRx(EPT2);
        gusbtransf.BulkTfout.buffcnt      += len;
        gusbtransf.BulkTfout.rest2process -= len;
        
        /* transfer finished */
        /*********************/
        if(gusbtransf.BulkTfout.command == BULK_DATA_OUT_AMOUNT && 
           gusbtransf.BulkTfout.rest2process == 0)
        {
          gusbtransf.BulkTfout.ready = YES;
        }
        
        if(gusbtransf.BulkTfout.command == BULK_DATA_OUT_STREAM)
        {
          //now check if we are at end of transfer
          if((RX_BULK_EP_FIFO_SIZE - len) > 0)   
          {
            gusbtransf.BulkTfout.ready = YES;         
          }
        }
        
        spin_unlock(&gusbtransf.BulkTfout.lock);
        
        if(gusbtransf.BulkTfout.ready == YES) 
        {
          wake_up_interruptible(&gusbtransf.BulkTfout.waitq); 
          PDBG("%s: wake queue (4)\n",__FUNCTION__);
        }
        // return;
      }
      ep_stat[EPT2]->toggle_bit = ep_stat[EPT2]->toggle_bit ? 0 : 1;
    }
    else 
    {
      //response to the toggle bit error as well as to the receive error
      FlushRx(EPT2);
      PDBG("toggle bit error\n");
    }
    spin_lock(&DevState.lock);
    DevState.UsbState.EpState[EPT2] = EP_ENABLED;
    spin_unlock(&DevState.lock);
  }
  else
    PDBG("EP2: RX not rtok\n");
}

//------------------------------------------------------------------
void HandleIrqEP0IN(void)
{
  ushort statEvnt;
  ushort statErr;

  PDBG("HandleIrqEP0IN...\n");
  
  spin_lock(&DevState.lock);
  DevState.UsbState.EpState[EPT0] = EP_IN_PROGRESS;
  spin_unlock(&DevState.lock);
  
  statErr  = UsbRd2B(ReadCtrlInErr) & 0xff;
  statEvnt = UsbRd2B(ReadCtrlInStat);
  
  /* data tx done */
  /****************/
  if (statErr & RTOK)
  {
    ep_stat[EPT0]->fifo_state = EMPTY;
    if(ctrl_send_buf.byte_cnts > 0)
    {// there is additional data to be sent
      fill_ctrl_fifo();
      tx_enable_epx(EPT0);
    }
    else if(device_buffers.zero_data == 1)
    {//zero data required
      PDBG("%s: zero data required\n",__FUNCTION__);
      device_buffers.zero_data = 0;
      FlushTx(EPT0);
      tx_enable_epx(EPT0);
    }
    else
    {
      PDBG("%s: clear ctrl in buf\n",__FUNCTION__);
      FlushTx(EPT0);
      UsbWr0B(ClearCtrlOutBuf);
    }
  }
  else //there is no ACK Re-enable sending
  {
    PDBG("%s: statErr=0x%x\n",__FUNCTION__,statErr);
    UsbWr0B(ClearCtrlOutBuf); //something is wrong -> maybe a stall
    UsbWr0B(ValidateCtrlInBuf);  // 100330
  }
  spin_lock(&DevState.lock);
  DevState.UsbState.EpState[EPT0] = EP_ENABLED;
  spin_unlock(&DevState.lock);
}
//------------------------------------------------------------------
void HandleIrqEP1(void)
{
  ushort statEvnt;
  ushort statErr;
  
  // struct timeval checkTV;
  // StartCheckTimer(&checkTV);  

  PDBG("HandleIrqEP1...\n");
  if (gusbtransf.BulkTfout.DevOpen == NO)
  {
    printk("USB device not open! GO IN REMOTE!\n");
    return;
  }
  
  if(gusbtransf.BulkTfin.timer->bTimerpresent == YES)  
  {
    del_timer(&gusbtransf.BulkTfin.timer->TimerList);
  }                
  
  statErr  = UsbRd2B(ReadEP1Err)&0xff;
  statEvnt = UsbRd2B(ReadEP1Stat)&0xff;
  PDBG("%s: stat=0x%x err=0x%x\n",__FUNCTION__,statEvnt,statErr);
  
  /* data tx done */
  /****************/
  if (statErr & RTOK)
  {
    //previous data packet from current ep was received successfully by host
    SendBulkData();
  }
  else 
  {//there is no ACK -> retransmit the previous data packet
    spin_lock(&DevState.lock);
    DevState.UsbState.EpState[EPT1] = EP_IN_PROGRESS;
    spin_unlock(&DevState.lock);
    SetBulkSendDataTimeout();
    USB_Retransmit_Data(EPT1);
  }
  
  spin_lock(&DevState.lock);
  DevState.UsbState.EpState[EPT1] = EP_ENABLED;
  spin_unlock(&DevState.lock);              

  UsbWr2B(WriteMode,UsbRd2B(ReadMode)&0xff & ~DBGMOD);
  
  // printk("HandleIrqEP1 needed: %d us\n", CheckTimerUS(&checkTV)); 
}
//------------------------------------------------------------------
void HandleIrqEP3(void)
{
  ushort statEvnt;
  ushort statErr;

  PDBG("HandleIrqEP3...\n");
  
  spin_lock(&DevState.lock);
  DevState.UsbState.EpState[EPT3] = EP_IN_PROGRESS;
  spin_unlock(&DevState.lock);

  statErr  = UsbRd2B(ReadEP3Err);
  statEvnt = UsbRd2B(ReadEP3Stat);
  
  /* data tx done */
  /****************/
  if (statErr & RTOK)
  {
    // previous data packet from current ep was received successfully by host
    //there are no further data to send, if yes we have to re-visit this part
  }
  else 
  {//there is no ACK -> retransmit the previous data packet
    USB_Retransmit_Data(EPT3);
  }
  
  spin_lock(&DevState.lock);
  DevState.UsbState.EpState[EPT3] = EP_ENABLED;
  spin_unlock(&DevState.lock);

  UsbWr2B(WriteMode,UsbRd2B(ReadMode)&0xff & ~DBGMOD);
}
//------------------------------------------------------------------
void HandleIrqEP5(void)
{
  ushort statEvnt;
  ushort statErr;

  PDBG("HandleIrqEP5...\n");
  
  if (gusbtransf.IntMeasTf.DevOpen == NO)
  {
    printk("USB device not open! GO IN REMOTE!\n");
    deactivate_usb();
    reset_usb();
    return;
  }
  
  spin_unlock_wait(&gusbtransf.IntMeasTf.lock);
  spin_lock(&gusbtransf.IntMeasTf.lock);
  
  if(gusbtransf.IntMeasTf.timer->bTimerpresent == YES)   
  {
    del_timer(&gusbtransf.IntMeasTf.timer->TimerList);
  }
  spin_unlock(&gusbtransf.IntMeasTf.lock);
  
  spin_lock(&DevState.lock);
  DevState.UsbState.EpState[EPT5] = EP_IN_PROGRESS;
  spin_unlock(&DevState.lock);

  statErr  = UsbRd2B(ReadEP5Err)&0xff;
  statEvnt = UsbRd2B(ReadEP5Stat)&0xff;
  PDBG("%s: stat=0x%x err=0x%x\n",__FUNCTION__,statEvnt,statErr);
  
  /* data tx done */
  /****************/
  if (statErr & RTOK)
  {
    // previous data packet from current ep was received successfully by host
    //there are no further data to send, if yes we have to re-visit this part
    wake_up_interruptible(&gusbtransf.IntMeasTf.waitq);
  }
  else 
  {//there is no ACK -> retransmit the previous data pack
    //set time out cos retransmit
    SetMeasSendDataTimeout();
    USB_Retransmit_Data(EPT5);
  }
  
  spin_lock(&DevState.lock);
  DevState.UsbState.EpState[EPT5] = EP_ENABLED;
  spin_unlock(&DevState.lock);        
  
  UsbWr2B(WriteMode,(UsbRd2B(ReadMode) & 0xff) & ~DBGMOD);

}

/********************************************************************
 ** for all logic devices
 *********************************************************************
 ** interrupt routine
 ***********************************************************************/
static irqreturn_t USB_ISP1181B_interrupt(int irq, void* ptr)
{
  // ulong       flags;
  ulong       irq_flags;
  static bool irq_flags_EP0IN = false;

  /* disable irq */
  /***************/
  // spin_lock_irqsave(&DevState.lock,flags);
  UsbWr2B(WriteMode,(UsbRd2B(ReadMode)&0xff) & ~INTENA);

  /* read irq flags */
  /******************/
  irq_flags = UsbRd4B(ReadInt);
  // PDBG("%s: IrqFlags=0x%x\n",__FUNCTION__,irq_flags);

  /* handle ISP1181B hang up */
  /***************************/
  if (irq_flags_EP0IN && (irq_flags & (BUSTATUS|SP_EOT))) /* suspend */
  {
    irq_flags_EP0IN = false;
    deactivate_usb();
    reset_usb();
  }
  
  if (irq_flags & EP0IN)
    irq_flags_EP0IN = true;
  
  // dbg_time();
  
  /* RX/TX data */
  /**************/
  if ((irq_flags & EP0OUT) || (UsbRd2B(CheckCtrlOutStat)&(EPFULL0|EPFULL1)))
    HandleIrqEP0OUT();
  if (irq_flags & EP0IN)
    HandleIrqEP0IN();
  if (irq_flags & EP1)
    HandleIrqEP1();
  if ((irq_flags & EP2) || (UsbRd2B(CheckEP2Stat)&(EPFULL0|EPFULL1)))
    HandleIrqEP2();
  if (irq_flags & EP3)
    HandleIrqEP3();
  if (irq_flags & EP5)
    HandleIrqEP5();
  
  /* RESET */
  /*********/
  if (irq_flags & RESET)
  {
    reset_usb();
    usb_device_reset();
    // MS: GOTO_STATE(ST_OPERAT);
    
#ifdef _SELF_MODULE_UNLOAD_			
    if( UsbBusError && 100000 < ++ResetEvCnt ) 
    {
      PDBG("Error ResetEvCnt: %d -> selfCleanUp() will be invoked\n", ResetEvCnt);
      selfCleanUp();		
    }		
#endif				
  }
  
  /* SUSPEND */
  /***********/
  if (irq_flags & SUSPND)
  {
    PDBG("SUSPEND IRQ...\n");
    DevState.UsbState.BusState = USB_DISCONNECTED;
    // MS: GOTO_STATE(ST_SUSPEND);	
  }

  /* RESUME */
  /**********/
  if (irq_flags & RESUME)
  {
    PDBG("RESUME IRQ...\n");
    
#ifdef _SELF_MODULE_UNLOAD_	
    PDBG("------> RESUME_EV UsbBusError: %d   onoff: %d  no of RESUME_EV: %d\n", UsbBusError, onoff, u); 
    if( onoff ) 
    { 
      UsbBusError = 0; 
      onoff       = 0;      
    }
    else 
    { 
      UsbBusError = 1; 
      ResetEvCnt  = 0; 
    }
#endif					
    // MS: ENABLE_ALT_INTS(SD3_EV | RESET_EV);			  					
    // Operational state
    // MS: GOTO_STATE(ST_OPERAT);				

    // enable rx
    FlushRx(EPT0);
    FlushRx(EPT2);

    // enable tx
    FlushTx(EPT0);
    FlushTx(EPT1);
    FlushTx(EPT3);
    FlushTx(EPT5);
  }

  /* enable irq */
  /**************/
  if (gusbtransf.BulkTfout.ready == NO)
  {
    // nothing?
  }
  // PDBG("ENABLE IRQ\n");
  
  clearIRQ3();
  UsbWr2B(WriteMode, (UsbRd2B(ReadMode)&0xff)|INTENA);

  // spin_unlock_irqrestore(&DevState.lock,flags);
  
  return IRQ_HANDLED;
}
/**********************************************************************
 **********************************************************************/
#ifndef MOD_IN_USE
#ifdef CONFIG_MODULE_UNLOAD
#define MOD_IN_USE module_refcount(THIS_MODULE)
#else
#error "Gamma USB function modules require"
#error "  'Loadable module support  --->  Module unloading'"
#error "to be enabled in the kernel"
#endif
#endif

static int usbdev_open(struct inode *inode, struct file *file)
{
  int  minor = MINOR(inode->i_rdev);
  // int  irq   = UsbRd2B(ReadInt);

  PDBG("usbdev_open...\n");
  
  spin_lock(&DevState.lock);
  if( USB_DISCONNECTED == DevState.UsbState.BusState)
    // MS: || !(altev & 0x08) || // no valid EOP received since last readout
    // MS: (altev & 0x30))    // 5ms of idle on upstream port 
  {
    spin_unlock(&DevState.lock);
    PDBG("State => USB bus isn't connected\n");
    deactivate_usb();
    reset_usb();
    return -EIO;
  }
  spin_unlock(&DevState.lock);
  
  if( USB_ISP1181B_DEVICES <= minor )  
  {
    PDBG(__FILE__ ": invalid minor, can't open device\n");
    return -ENODEV;
  }
  else 
  { 
    PDBG("minor number: %d\n", minor); 
  }
  
  file->f_op = usbn_fop_array[minor];
  
  if( !MOD_IN_USE ) 
  {
    gusbtransf.CtrlTfout.buffer = NULL;
    gusbtransf.BulkTfout.buffer = NULL;
    
    gusbtransf.CtrlTfout.DevOpen = NO;
    gusbtransf.IntMeasTf.DevOpen = NO;
    gusbtransf.BulkTfout.DevOpen = NO;
  }
  
  switch( minor ) 
  {
    case USB_ISP1181B_CNTRL_DEVICE:
      //vendor request OUT transfer via EP0
      init_waitqueue_head(&gusbtransf.CtrlTfout.waitq);
      spin_lock_init(&gusbtransf.CtrlTfout.lock);
      
      IntCrtlOutTransfer();
      
      if( !(gusbtransf.CtrlTfout.buffer = kmalloc(CTRL_MAX_BUFFER_SIZE, GFP_KERNEL)) )   
      {
        return -ENOMEM;
      }
      
      gusbtransf.CtrlTfout.timer = NULL;
      gusbtransf.CtrlTfout.DevOpen = YES;
      PDBG("USB_ISP1181B_CNTRL_DEVICE\n");
      break;
      
    case USB_ISP1181B_MEASUREMENT_DEVICE:
      InitMeasInTransfer();
      init_waitqueue_head(&gusbtransf.IntMeasTf.waitq);
      spin_lock_init(&gusbtransf.IntMeasTf.lock);
      
      gusbtransf.IntMeasTf.timer = &meastimer;
      gusbtransf.IntMeasTf.DevOpen = YES;
      
      PDBG("USB_ISP1181B_MEASUREMENT_DEVICE\n");
      break;
      
    case USB_ISP1181B_BULK_DEVICE:
      // IN transfer
      init_waitqueue_head(&gusbtransf.BulkTfin.waitq);
      spin_lock_init(&gusbtransf.BulkTfin.lock);
      gusbtransf.BulkTfin.timer = &bulkintimer;
      InitBulkInTransfer();
      
      gusbtransf.BulkTfout.timer   = NULL;
      gusbtransf.BulkTfout.DevOpen = YES;
      
      //Init interrupt transfer variables
      gusbtransf.IntCmdTf.buffer = NULL; //don't use it
      gusbtransf.IntCmdTf.timer  = NULL; //&intcmdtimer;
      //         InitCmdInTransfer();		 
      PDBG("USB_ISP1181B_BULK_DEVICE\n");
      // HDT:
      UsbWr4B(WriteIntEnable, IERST | IESUSP | IERESM |
              IEP0IN | IEP0OUT | IEP1 | IEP2 | IEP3 | IEP5);
      break;
  }
    
  //both devices USB_ISP1181B_MEASUREMENT_DEVICE & USB_ISP1181B_BULK_DEVICE
  if( gusbtransf.IntMeasTf.DevOpen || gusbtransf.BulkTfout.DevOpen ) { 
    //bulk OUT transfer wait queue
    PDBG("bulk out transfer in open dev\n");
    InitBulkOutTransfer();
    init_waitqueue_head(&gusbtransf.BulkTfout.waitq);
    PDBG("%s: init queue\n",__FUNCTION__);
    spin_lock_init(&gusbtransf.BulkTfout.lock);
    
    if( 1 > gBulkOutTransferCnt ) 
    {	
      if( !(gusbtransf.BulkTfout.buffer = kmalloc(BULK_MAX_BUFFER_SIZE, GFP_KERNEL)) )  
      {
        return -ENOMEM;	
      }
    }
    gBulkOutTransferCnt++;
  }
  
  gBulkInTimeOut      = 0;
  gMeasurementTimeOut = 0;
  return 0;
}
/*******************************************************************/
static int usbdev_release(struct inode *inode, struct file *file)
{
  int minor = MINOR(inode->i_rdev);

  PDBG("%s...\n",__FUNCTION__);
  switch( minor ) 
  {
    case USB_ISP1181B_CNTRL_DEVICE:
      if( NULL != gusbtransf.CtrlTfout.buffer ) 
      {
        kfree(gusbtransf.CtrlTfout.buffer);
        gusbtransf.CtrlTfout.buffer = NULL;
      }
      gusbtransf.CtrlTfout.DevOpen = NO;
      // PDBG("USB_ISP1181B_CNTRL_DEVICE\n");
      break;
      
    case USB_ISP1181B_MEASUREMENT_DEVICE:
      gusbtransf.IntMeasTf.DevOpen = NO;
      gBulkOutTransferCnt--;
      // PDBG("USB_ISP1181B_MEASUREMENT_DEVICE\n");
      break;
      
    case USB_ISP1181B_BULK_DEVICE:
      gusbtransf.BulkTfout.DevOpen = NO;
      gBulkOutTransferCnt--;
      // PDBG("USB_ISP1181B_BULK_DEVICE\n");
      // HDT: deactivate IEP1 when device is released
      UsbWr4B(WriteIntEnable, IERST | IESUSP | IERESM |
              IEP0IN | IEP0OUT | IEP2 | IEP3 | IEP5 );
      break;
  }
  
  if( 0 == gBulkOutTransferCnt && NULL != gusbtransf.BulkTfout.buffer ) 
  {
    kfree(gusbtransf.BulkTfout.buffer);
    gusbtransf.BulkTfout.buffer = NULL;
  }
  
  return 0;
}

// functions in order to communicate with the USB controller
//------------------------------------------------------------------
inline unsigned short UsbRd2B(ushort UsbReg)
{
  unsigned short sTmp;
  
  writew(UsbReg, PORT_CTRL);
  sTmp = readw(PORT_DATA);
  
  // PDBG("UsbRd2B: 0x%x << 0x%x\n",UsbReg,sTmp);
  return sTmp;
}
//------------------------------------------------------------------
inline unsigned long UsbRd4B(ushort UsbReg)
{
  unsigned long  lTmp;

  writew(UsbReg, PORT_CTRL);
  lTmp  = readw(PORT_DATA);
  lTmp |= readw(PORT_DATA) << 16;
  
  // PDBG("UsbRd4B: 0x%x << 0x%lx\n",UsbReg,lTmp);
  return lTmp;
}
//------------------------------------------------------------------
inline unsigned short UsbRdData2B(void)
{
  ushort sTmp = readw(PORT_DATA);

  // PDBG("UsbRdData2B: << 0x%x\n",sTmp);
  return sTmp;
}
//------------------------------------------------------------------
inline void UsbWr2B(ushort UsbReg, ushort value)
{
  writew(UsbReg, PORT_CTRL);
  writew(value, PORT_DATA);
  // PDBG("UsbWr2B: 0x%x >> 0x%x\n",UsbReg,value);
}
//------------------------------------------------------------------
inline void UsbWr4B(ushort UsbReg, ulong value)
{
  ushort sTmp;

  writew(UsbReg, PORT_CTRL);

  sTmp = (ushort)(value & 0xffff);
  writew(sTmp, PORT_DATA);

  sTmp = (ushort)((value >> 16) & 0xffff);
  writew(sTmp, PORT_DATA);

  // PDBG("UsbWr4B: 0x%x >> 0x%lx\n",UsbReg,value);
}
//------------------------------------------------------------------
inline void UsbWr0B(ushort UsbReg)
{
  writew(UsbReg, PORT_CTRL);

  // PDBG("UsbWr0B: 0x%x\n",UsbReg);
}
//------------------------------------------------------------------
inline void UsbWrData2B(ushort value)
{
  writew(value, PORT_DATA);
  // PDBG("UsbWrData2B: >> 0x%x\n",value);
}
//------------------------------------------------------------------
inline void FlushTx(int ep_no)
{
  PDBG("FlushTx %d...\n",ep_no);
  UsbWr0B(StallCtrlInEP+ep_no);
  UsbWr0B(UnstallCtrlInEP+ep_no);
}
//------------------------------------------------------------------
inline void FlushRx(int ep_no)
{
  PDBG("FlushRx %d...\n",ep_no);
  switch (ep_no)
  {
    case EPT0:
      UsbWr0B(ClearCtrlOutBuf);
      break;
      
    case EPT2:
      UsbWr0B(ClearEP2);
      break;
      
    default:
      //nothing
      break;
  }
}
//------------------------------------------------------------------
inline void clearIRQ3(void)
{
  unsigned char cTmp;
  cTmp  = /* ctrl_inb */__raw_readb(INTREQ00);
  cTmp &= ~0x10;
  /* ctrl_outb */__raw_writeb(cTmp, INTREQ00);
}

//------------------------------------------------------------------
int __init usb_init(void)
{
  int res;
    
#ifdef CONFIG_DEVFS_FS
  if( devfs_register_chrdev(USBGAMMA_MAJOR, "USB_ISP1181B", &usbdev_fops) )
#else
    //name "USB_ISP1181B" in /proc/devices
  if( register_chrdev(USBGAMMA_MAJOR,"USB_ISP1181B", &usbdev_fops) )
#endif
  {
    PDBG("usb_init() usb_isp1181b.o: unable to get major no: %d for USB_ISP1181B device!!!!!\n", USBGAMMA_MAJOR);
    return -EIO;
  }
	
#ifdef CONFIG_DEVFS_FS
  devfs_handle = devfs_mk_dir(NULL, "USB_ISP1181B", NULL);
#endif

  DevState.UsbState.BusState = USB_DISCONNECTED;

  // set registers of sh4
  if( 0 > (res = init_sh4_reg()) )       
  {
    return res; 
  }
  
  //install IRQ handler
  if( 0 == USB_ISP1181B_interrupt_handler_installed )  
  {
    if( 0 > (request_irq(USB_ISP1181B_IRQ, USB_ISP1181B_interrupt, IRQF_DISABLED, "USB_ISP1181B", NULL)) )  
    {
      PDBG("usb_init: failed to register IRQ3 for USB device\n");
      free_irq(USB_ISP1181B_IRQ, NULL);
      return -EIO;
    }
    USB_ISP1181B_interrupt_handler_installed = 1;
  }
  
  spin_lock_init(&DevState.lock);
  //this parameter must be set here   
  
  gusbtransf.CtrlTfout.buffer = NULL;
  gusbtransf.BulkTfout.buffer = NULL;
  gusbtransf.IntCmdTf.buffer  = NULL;
  
  gusbtransf.IntCmdTf.DevOpen  = NO;
  gusbtransf.CtrlTfout.DevOpen = NO;
  gusbtransf.IntMeasTf.DevOpen = NO;
  
  //now initiliaze registers of usb controller
  if( 0 > (res = init_USB_ISP1181B_reg()) )  
  { 
    return res; 
  }
  
  PDBG("USB device USB_ISP1181B is initialized now\n");
  PDBG("USB Gamma device version: %s\nVendor ID: 0x%X \n", VERSION_STRA, 0x1670);

  return 0;
}

// #define CTRL_PHY_ADR 0x14000000
// #define DATA_PHY_ADR 0x14000004
// #define CTRL_PHY_ADR 0xA4000202
// #define DATA_PHY_ADR 0xA4000200
#define CTRL_PHY_ADR 0xB4000002
#define DATA_PHY_ADR 0xB4000000
//------------------------------------------------------------------
int init_USB_ISP1181B_reg(void)
{
  // int     i;
  ushort  sTmp;

  PDBG("%s...\n",__FUNCTION__);

  port_ctrl = ioremap(CTRL_PHY_ADR, 2);
  if (!port_ctrl)
  {
    PDBG(KERN_INFO "HL_USB_Dev: ctrl port error\n");
    return -EIO;
  }
  port_data = ioremap(DATA_PHY_ADR, 2);
  if (!port_data)
  {
    PDBG(KERN_INFO "HL_USB_Dev: data port error\n");
    return -EIO;
  }
  
  PDBG("hl_usb_viper: ctrl(phy)=0x%p -> ctrl=0x%p\n",(void*)CTRL_PHY_ADR, port_ctrl);
  PDBG("hl_usb_viper: data(phy)=0x%p -> data=0x%p\n",(void*)DATA_PHY_ADR, port_data);
  
  //get the USB device
  sTmp = UsbRd2B(ReadChipId);
  PDBG("CHIPID=0x%04x\n",sTmp);
  sTmp &= 0xFF00; // evaluate only high byte
  if ( ISP1181B_CHIP_ID != sTmp )    
  {
    PDBG(KERN_INFO "WRONG USB DEV CHIP ID: 0x%x (expected=0x%x)\n", 
           sTmp, ISP1181B_CHIP_ID);
    return -EIO;
  }
  else
  {
    PDBG("DEBUG: correct ISP1181B_CHIP_ID found\n");
  }
    
  /* config */
  /**********/
  UsbWr2B(UnlockDev,UnlockCode);
  config_usb();
  
  /* activate */
  /************/
  activate_usb();
  
  return 0;
}

//------------------------------------------------------------------
int init_sh4_reg(void)
{
  unsigned long  flags;
  unsigned short sTmp;
  
  local_save_flags(flags);
  local_irq_disable();
  
  // set PSELD to IRQ3 fct (bit 12/13: 00)
  sTmp  = /* ctrl_inw */__raw_readw(PSELD);
  sTmp &= ~0x3000;
  /* ctrl_outw */__raw_writew(sTmp,PSELD);
  
  sTmp = /* ctrl_inw */__raw_readw(ICR1);
  sTmp &= ~(0x3<<8); // reset IRQ3 sense select
  sTmp |=  (0x2<<8); // set IRQ3 to low level active
  /* ctrl_outw */__raw_writew(sTmp,ICR1);

#if 0
  PDBG("HLUSB: CMNCR:MAP=0x%x\n", (/* ctrl_inl */__raw_readl(PORT_CMNCR)>>12)&0x3);
  PDBG("HLUSB: CS5ABCR=0x%x\n", /* ctrl_inl */__raw_readl(PORT_CS5ABCR));
  PDBG("HLUSB: CS5AWCR=0x%x\n", /* ctrl_inl */__raw_readl(PORT_CS5AWCR));
  PDBG("HLUSB: DRVA[14:15]=0x%x\n", (/* ctrl_inw */__raw_readw(PORT_DRVCRA)>>14)&0x3);
  PDBG("HLUSB: HIZB[4]=0x%x\n", (/* ctrl_inw */__raw_readw(PORT_HIZCRB)>>4)&0x1);
  PDBG("HLUSB: PRCR[8:9]=0x%x\n", (/* ctrl_inw */__raw_readw(PORT_PRCR)>>8)&0x3);
#endif
  
  local_irq_restore(flags);
  
  return 0;
}

//------------------------------------------------------------------
#ifdef _SELF_MODULE_UNLOAD_
void selfCleanUp( void ) 
{
  usb_cleanup();
  EIerror = -EIO;
  PDBG("selfCleanUp() EIerror: %d = usb_cleanup() \n", EIerror);
}
#endif

//------------------------------------------------------------------
int usb_cleanup(void)
{
  int res = 0;
  
#ifdef CONFIG_DEVFS_FS
  if( (res = devfs_unregister_chrdev(USBGAMMA_MAJOR, "USB_ISP1181B")) )
  {
    PDBG("usb_cleanup() usb_isp1181b.o: unable to release major %d for USB_ISP1181B\n", USBGAMMA_MAJOR);
  }
#else
  unregister_chrdev(USBGAMMA_MAJOR, "USB_ISP1181B");
#endif

  //detach USB device -> and SW Reset
  UsbWr0B(ResetDev);
  PDBG("USB_ISP1181B is disconnected from bus now\n");      
  
  if( USB_ISP1181B_interrupt_handler_installed ) 
  {
    free_irq(USB_ISP1181B_IRQ, NULL);
  }
  
  usb_initialized--;
  PDBG( "USB_ISP1181B Module: %d is removed now \n", usb_initialized );
  
  return res;
}


static int __init init_usb_isp1181b(void)
{
  int ret;
#ifdef _SELF_MODULE_UNLOAD_
  UsbBusError = 1;
  EIerror     = 0;
#endif	
  usb_initialized = 0;
  ret             = usb_init();	
  
#ifdef _SELF_MODULE_UNLOAD_
  if( 0 > EIerror ) 
  { 
    //	    UsbBusError = 1;
    ret = EIerror; 		 
  } 
  else 
  {
    UsbBusError = 0;
  }
#endif	
  
  if( 0 > ret ) 
  { 
    PDBG("\n\nInit Viper USB device failed!!\n\n"); 
  }
  else          
  { 
    // enable_irq( USB_ISP1181B_IRQ );
    usb_initialized++;
  }
  
  return ret;
}

static void __exit exit_usb_isp1181b(void)
{
  usb_cleanup();
  return;
}

module_init(init_usb_isp1181b);
module_exit(exit_usb_isp1181b);

MODULE_LICENSE("GPL");
