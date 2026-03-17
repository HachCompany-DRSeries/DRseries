/***************************************************************************
                         m66291_usb_viper.c  -  description
                             -------------------
    copyright            : (C) 2004 by Thomas Siegmund
    email                : tsiegmund@hach-lange.de
    Author               :Nikhil Varghese
**********Function Edited for support of M66291***************
inline void FlushRx(int ep_no)
inline void FlushTx(int ep_no)
static int usb_m66291_write_bulk(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
void HandleIrqEP0OUT(void)
******************Function Added for support of m66291*************
inline unsigned short m66291_read_reg(ushort UsbReg)          ->Reading M66291 Register
inline void m66291_set_reg(ushort UsbReg, ushort value)      -> Writing M66291 Register
static irqreturn_t usb_m66291_interrupt(int irq, void* ptr) -> Interrupt Rotine of M66291
void INTR_int(U16 status, U16 int_enbl)                       -> Interrupt Decession function
void read_data_in_epx(U8 EPnum)                               -> Read the data in endpoint
void BusSuspend(void)                                         ->Suspend the Bus
void bus_reset(void)                                          ->Reset the Bus
void EpBuffClear(unsigned char EP_Num)                        -> Clear the EP_Num Endpoint
void BusResume(void)                                          ->Resume the suspended Bus
void VBUScheck( void )                                        ->Set the Vbus of the m66291
***************************************************************************/

#ifndef __KERNEL__
#define __KERNEL__
#endif

/* IRQF_DISABLED not supported in Linux 4.1 (not needed either) */
#if !defined(IRQF_DISABLED)
#define IRQF_DISABLED 0x00
#endif

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/slab.h>
// #include <linux/smp_lock.h>
#include <linux/spinlock.h>
#ifdef CONFIG_DEVFS_FS
#include <linux/devfs_fs_kernel.h>
#endif
#include <linux/proc_fs.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/ioctl.h>
#include <asm/irq.h>
#include <linux/usb.h>    
#include <asm/io.h>

/**** own headers ************/
#include "m66291_usb_reg.h"
#include "m66291_usb_drv.h"

#include "m66291_usb_ioctl.h"
#include "m66291_usb_viper.h"
#include "m66291_usb_state.h"
#include "m66291_usb_core.h"
#include "m66291_usb_version.h"
#include "m66291_drv.c"

/*********************************
 ** defines
 *********************************/
#define USB_M66291_IRQ evt2irq(IRQEVT_IRQ3)
#define INTREQ00         0xA4140024
#define ICR1             0xA414001C
#define PSELD            0xA4050154
#define IRQEVT_IRQ3	 0x660
#define USB_NAME "m66291"
#define CTRL_PHY_ADR 0xB4000002
#define DATA_PHY_ADR 0xB4000000
#define LOWBYTE 0x00FF
#define MSBBYTE 0xFF00

#define CHECK 0
static unsigned short BackUp;
USB_request *req;
unsigned char *recive_data=0;   

#define DBG_TIME printk("%s[%d]: TIME=%ld\n", __FUNCTION__, __LINE__, jiffies);

/*******************************
 ** externs
 *******************************/
extern const int usb_m66291_tx_endpoint_addr[];
extern const int usb_m66291_rx_endpoint_addr[];

/***********************************************/
static int usb_initialized = 0; 
static int usb_m66291_interrupt_handler_installed = 0; 
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

#endif
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
	gusbtransf.CtrlTfout.TransfActive = NO;
}

static unsigned int usb_m66291_poll_cntrl(struct file *filp, poll_table *wait)
{
	unsigned int mask = 0;
	poll_wait(filp, &gusbtransf.CtrlTfout.waitq, wait);

	spin_unlock_wait(&gusbtransf.CtrlTfout.lock);
	spin_lock(&gusbtransf.CtrlTfout.lock);

	if(gusbtransf.CtrlTfout.buffcnt)
		mask = POLLIN | POLLRDNORM;

	/*  //write
	 */
	if(gusbtransf.CtrlTfout.buffcnt == CTRL_MAX_BUFFER_SIZE)
		mask |= POLLPRI;

	//all data were read
	if(gusbtransf.CtrlTfout.ready == YES) { mask |= POLLHUP; }

	spin_lock(&DevState.lock);
	if(DevState.UsbState.BusState == USB_DISCONNECTED) 
	{
		mask |= POLLERR;
		PDBG("Polling Error Happened\n");
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
static int usb_m66291_read_cntrl(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	static DWORD len =0;
	wait_queue_t wait;
	ssize_t read=0, ret=0;

	//for blocking read!!
	init_waitqueue_entry(&wait, current);
	add_wait_queue(&gusbtransf.CtrlTfout.waitq, &wait);
	current->state = TASK_INTERRUPTIBLE;

	// from "pipe.c" (MS: this was removed from pipe.c)
	// Seeks are not allowed on pipes.
	// Always return 0 on null read.
	if(count == 0)   goto out_nolock;
	//get the spin lock
	spin_unlock_wait(&gusbtransf.CtrlTfout.lock);
	spin_lock_irq(&gusbtransf.CtrlTfout.lock);

	do 
	{

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
	//when there is a vendor request with an actually size to read or send
	//this will be set to zero      default command
	gusbtransf.BulkTfout.command      = BULK_DATA_OUT_STREAM; //USB_NO_COMMAND;
	gusbtransf.BulkTfout.dataSize     = 0;
	gusbtransf.BulkTfout.rest2process = 0;
	gusbtransf.BulkTfout.buffcnt      = 0;
	gusbtransf.BulkTfout.ready        = NO;
	gusbtransf.BulkTfout.exitOnZLP    = NO;
	gusbtransf.BulkTfout.TransfActive = NO;

	InitCmdInTransfer();
}
/***************************************************************************/
static unsigned int usb_m66291_poll_bulk(struct file *filp, poll_table *wait)
{   //no data to read
	unsigned int mask = 0;
	poll_wait(filp, &gusbtransf.BulkTfout.waitq, wait);

	spin_unlock_wait(&gusbtransf.BulkTfout.lock);
	spin_lock_irq(&gusbtransf.BulkTfout.lock);

	if(gusbtransf.BulkTfout.buffcnt)  
	{
		mask = POLLIN | POLLRDNORM;  
	}

	/*  //write
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
		printk("Poll Error Happened\n");
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
static int usb_m66291_read_bulk(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	//unsigned long flags = 0;
	static DWORD len =0;

	wait_queue_t wait;
	ssize_t read, ret;
#ifdef DEBUG
	package = 0;
#endif


	//for blocking read!!
	init_waitqueue_entry(&wait, current);
	add_wait_queue(&gusbtransf.BulkTfout.waitq, &wait);
	current->state = TASK_INTERRUPTIBLE;

	// from "pipe.c" (MS: this was removed from pipe.c)
	// Seeks are not allowed on pipes.
	read = 0;
	// Always return 0 on null read.
	ret = 0;
	if(count == 0)
		goto out_nolock;
	spin_unlock_wait(&gusbtransf.BulkTfout.lock);      
	spin_lock_irq(&gusbtransf.BulkTfout.lock);

	do
	{ 
		gusbtransf.BulkTfout.TransfActive = YES;

		//check the bulk command maybe superfluous
		if(gusbtransf.BulkTfout.command != BULK_DATA_OUT_STREAM && 
				gusbtransf.BulkTfout.command != BULK_DATA_OUT_AMOUNT)
		{ //wrong command
			printk("ERR: %s: wrong cmd\n",__FUNCTION__);
			ret = -EFAULT;
			goto spin_unlock_and_out;
		}

		if(gusbtransf.BulkTfout.buffcnt)
		{
			if(copy_to_user(buf+len, gusbtransf.BulkTfout.buffer, gusbtransf.BulkTfout.buffcnt))
			{
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
		//we finished with a zero length package
		ReadDataStreamSize = len;
		//we MUST return with 0
		len = 0;
		//poll should give back POLLHUP
		read = len;

		if(gusbtransf.BulkTfout.command == BULK_DATA_OUT_AMOUNT)
		{
			InitBulkOutTransfer();
		}
	}

spin_unlock_and_out:
	//give back spin lock
	spin_unlock_irq(&gusbtransf.BulkTfout.lock);

out_nolock:
	current->state = TASK_RUNNING;
	remove_wait_queue(&gusbtransf.BulkTfout.waitq, &wait);
	if(read) ret = read;
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

	PDBG("Error: USB disconnected or application was not started!!!\n");
	PDBG("MeasDataTimeOut  time elapsed: %Lu ms\n",(unsigned long long) gMeasurementTimeOut*10 -
			(jiffies - gusbtransf.IntMeasTf.timer->TimerList.expires));
	deactivate_usb();
	reset_usb();

	spin_unlock(&gusbtransf.IntMeasTf.lock);
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
		gusbtransf.IntMeasTf.timer->TimerList.expires = jiffies + gMeasurementTimeOut;

		gusbtransf.IntMeasTf.timer->bTimeOut = NO;
		add_timer(&gusbtransf.IntMeasTf.timer->TimerList);
	}
}
/*******************************************************************************************************/
static int usb_m66291_write_measurement(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int  bytes2transfer;
	int  ret;
	if( 0 <= gusbtransf.IntMeasTf.dataSize ) 
	{
		//1st entry in write
		ret = bytes2transfer = 0;

		if( TX_INTR_EP_FIFO_SIZE_MEAS < gusbtransf.IntMeasTf.dataSize ) 
		{
			//optimization done here
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
	//give back spin lock
	ret = SendInterruptData(gusbtransf.IntMeasTf.buffer, bytes2transfer, EPT5);

	//InitMeasInTransfer();
	//return ret;

cleanup_out:
	if( NULL != gusbtransf.IntMeasTf.buffer )  
	{
		kfree(gusbtransf.IntMeasTf.buffer);
		gusbtransf.IntMeasTf.buffer = NULL;
	}

	InitMeasInTransfer();
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
	gusbtransf.BulkTfin.TransfActive = NO;
}
/*****************************************************
 ** BulkDataTimeOut
 ******************************************************/
void BulkDataTimeOut(unsigned long arg)
{
	spin_unlock_wait(&gusbtransf.BulkTfin.lock);
	spin_lock(&gusbtransf.BulkTfin.lock);

	gusbtransf.BulkTfin.timer->bTimeOut = YES;

	deactivate_usb();
	reset_usb();

	PDBG("Error: USB disconnected or application was not started!!!\n");
	PDBG("BulkDataTimeOut  time elapsed: %Lu ms\n",(unsigned long long) gBulkInTimeOut*10 -
			(jiffies - gusbtransf.BulkTfin.timer->TimerList.expires));

	spin_unlock(&gusbtransf.BulkTfin.lock);
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

int gSendBulkErr;
/*****************************************************************************************************/
static int usb_m66291_write_bulk(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	DWORD  bytes2copy        = 0;
	DWORD  bytestransferred  = 0;
	static int   if_pos      = 0;
	static int   first       = YES;  
	static int   temp_count,i, rem_count;  
	unsigned char *temp_send=0;
	unsigned char *hold=0;

	spin_unlock_wait(&gusbtransf.BulkTfin.lock);
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

		if( !(recive_data = kmalloc(count,GFP_KERNEL)) )   
		{
			printk("%s kmalloc Failed in temp...\n",__FUNCTION__);
			spin_unlock_irq(&gusbtransf.BulkTfin.lock);
			return -ENOMEM;
		}
		hold=recive_data;//Hold is just a memory pointer to hold inital address of recive_data
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
	memset(recive_data, 0, count);
	gusbtransf.BulkTfin.buffcnt = 0;
        gSendBulkErr = 0;
        
	//get user data
	if( copy_from_user(recive_data, buf, count) )	
	{
		kfree(recive_data);  
		bytestransferred = -ETIME; 
		goto out;
	}
	//give back spin lock
	spin_unlock_irq(&gusbtransf.BulkTfin.lock);
	temp_count=count/BULK_MAX_BUFFER_SIZE; /*Find out How many packets to send*/
	for(i=0;i<temp_count;i++){
          temp_send=recive_data;
          recive_data=recive_data+BULK_MAX_BUFFER_SIZE;
          SendBulkData(bytes2copy,temp_send,not_last_pack); /*Send each 64bytes Packet*/
          if (gSendBulkErr)
            break;
	}
	rem_count=count-(temp_count*1024);
	temp_send=recive_data;
        if (!gSendBulkErr)
          SendBulkData(rem_count,temp_send,last_pack);
        if (!bytestransferred)
          bytestransferred = gSendBulkErr;
	wait_event_interruptible(wq, flag != 0);/*Wait to complete transfer*/
	temp_send=NULL; 
    	flag = 0;
	recive_data=hold;//Put back the inital addess of recive_data we will free it in release 
	
	if(recive_data!=NULL){ //Free the bulk transfer buffer
          mdelay(10);           //This delay is very much required as the operation may happening
          kfree(recive_data);
          recive_data=NULL;
	}
	return bytestransferred;
        
out:
	gBulkInTimeOut = 0;
	return bytestransferred;

}

typedef unsigned int io_data;

/******************************************************************************************************/
static long usbdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	// DWORD io_data;
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
		case USB_IO_GET_READ_STREAM_SIZE:
			ret = __put_user(ReadDataStreamSize, (DWORD*)arg);
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
		case USB_IO_SET_MEAS_DATA_SIZE:
			//size of data
			spin_unlock_wait(&gusbtransf.IntMeasTf.lock);
			spin_lock(&gusbtransf.IntMeasTf.lock);

			ret = __get_user(gusbtransf.IntMeasTf.dataSize, (DWORD*)arg);
			gusbtransf.IntMeasTf.rest2process = gusbtransf.IntMeasTf.dataSize;
			gusbtransf.IntMeasTf.command = BULK_DATA_IN_AMOUNT;

			gusbtransf.IntMeasTf.TransfActive = YES;

			spin_unlock(&gusbtransf.IntMeasTf.lock);

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
			ret = __put_user(VERSION_USB, (DWORD*)arg);
			break;
		default:
			return -EINVAL;
			//break;
	}

	return ret;
}

void VBUScheck( void ){
	unsigned short buffer;
	m66291_set_reg(INT_STATUS0,m66291_read_reg(INT_STATUS0)&(~VBUS));
	buffer = m66291_read_reg(USB_ENABLE);
	if ((buffer & SCKE)==0) {
		m66291_set_reg(INT_STATUS0,0xffff);
	}
	buffer = m66291_read_reg(INT_ENABLE0);
	if (buffer & RSME ){
		buffer = m66291_read_reg(POLARITY_CNT);
		if ((~buffer & INTL)==0){
			m66291_set_reg(INT_ENABLE0,m66291_read_reg(INT_ENABLE0) &(~RSME));
			m66291_set_reg(INT_ENABLE0,m66291_read_reg(INT_ENABLE0) |(RSME));
		}
	}
}

void BusResume(void) {
	unsigned short buffer;
	clear_usb_status();
	do {
		buffer = m66291_read_reg(INT_STATUS0);
	} while (buffer & RQ_SPDP);

	m66291_set_reg(INT_STATUS0,m66291_read_reg(INT_STATUS0) &(~RESM));
	m66291_set_reg(INT_ENABLE0,BackUp);
	buffer = m66291_read_reg(USB_ENABLE);

	if ((buffer & SCKE)==0) {
		m66291_set_reg(INT_STATUS0,0xffff);
	}
	buffer = m66291_read_reg(INT_ENABLE0);

	if (buffer & VBSE ){
		buffer = m66291_read_reg(POLARITY_CNT);

		if ((~buffer & INTL)==0){
			m66291_set_reg(INT_ENABLE0,m66291_read_reg(INT_ENABLE0) &(~VBSE));
			m66291_set_reg(INT_ENABLE0,m66291_read_reg(INT_ENABLE0) |(VBSE));
		}
	}
	m66291_set_reg(INT_ENABLE0,m66291_read_reg(INT_ENABLE0) &(~RSME));
}

/*Clear the Buffer*/
void EpBuffClear(unsigned char EP_Num)
{
	unsigned int adr;
	m66291_set_reg(CPU_FIFO_SELECT, EP_Num);
	adr = (unsigned int )(EP1_1CONFIG + ((EP_Num-1)*4));
	m66291_set_reg(adr, EPi_PID_NAK);
	m66291_set_reg(adr, EPi_ACLR);
	m66291_set_reg(adr, m66291_read_reg(adr) & ~EPi_ACLR);
	m66291_set_reg(CPU_FIFO_SELECT, m66291_read_reg(CPU_FIFO_SELECT) & ~CPU_EP);
}


/*Reset BusOnly*/
void bus_reset(void)
{
	clear_control_buffer(&ctrl_receive_buf);
	clear_control_buffer(&ctrl_send_buf);
	m66291_set_reg(INT_STATUS1, 0x0001);
	m66291_set_reg(INT_STATUS2, 0x0001);
	m66291_set_reg(INT_STATUS3, 0x0001);
	EpBuffClear(EP1);
	EpBuffClear(EP2);
	EpBuffClear(EP3);
	EpBuffClear(EP4);
	EpBuffClear(EP5);
	EpBuffClear(EP6);
	m66291_set_reg(INT_STATUS0, m66291_read_reg(INT_STATUS0) & (~RESM));
}
void BusSuspend(void) {
	m66291_set_reg(USB_ENABLE, m66291_read_reg(USB_ENABLE)&(~( SCKE | USBPC)) );
	m66291_set_reg(USB_ENABLE, m66291_read_reg(USB_ENABLE)&(~PLLC));
	m66291_set_reg(USB_ENABLE, m66291_read_reg(USB_ENABLE)&(~XCKE));
	BackUp = m66291_read_reg(INT_ENABLE0);
	m66291_set_reg(INT_ENABLE0,0);
	m66291_set_reg(INT_ENABLE0,m66291_read_reg(INT_ENABLE0) |  RSME);
}
void HandleIrqEP0OUT(void)
{
	unsigned char ReqType;
	unsigned char ReqRequest;
	unsigned short ReqValue;
	unsigned short ReqIndex;
	unsigned short ReqLength;
	unsigned short int_sts0;
	unsigned short buffer;
	unsigned char temp[8]={0};

	int_sts0 = m66291_read_reg(INT_STATUS0);
	clear_control_buffer(&ctrl_receive_buf);


	spin_lock(&DevState.lock);
	DevState.UsbState.EpState[EPT0] = EP_IN_PROGRESS;
	spin_unlock(&DevState.lock);

	if(int_sts0 & VALID) //Valid setup packet recived
	{
		m66291_set_reg(INT_STATUS0,m66291_read_reg(INT_STATUS0)&(~ VALID));
		gusbtransf.CtrlTfout.dataSize     = 0;
		gusbtransf.CtrlTfout.rest2process = 0;

		buffer = m66291_read_reg(REQUEST_TYPE);
		ReqType =  buffer & LOWBYTE;
		ReqRequest = (buffer & MSBBYTE) >> 8;
		ReqValue = m66291_read_reg(REQUEST_VALUE);
		ReqIndex = m66291_read_reg(REQUEST_INDEX);
		ReqLength = m66291_read_reg(REQUEST_LENGTH);

		temp[0]=ReqType;
		temp[1]=ReqRequest;
		temp[2]=(ReqValue&LOWBYTE);
		temp[3]=(ReqValue&MSBBYTE)>>8;
		temp[4]=(ReqIndex&LOWBYTE);
		temp[5]=(ReqIndex&MSBBYTE)>>8;
		temp[6]=(ReqLength&LOWBYTE);
		temp[7]=(ReqLength&MSBBYTE)>>8;
		ctrl_receive_buf.data      =temp ; 
		ctrl_receive_buf.byte_cnts = ReqLength;
		ep_stat[EPT0]->toggle_bit  = 0x01;
		spin_lock(&DevState.lock);
		DevState.UsbState.EpState[EPT0] = EP_ENABLED;
		spin_unlock(&DevState.lock);
		dev_req_handler();
	}
}
void read_data_in_epx(U8 EPnum) {
	unsigned static short buffer;
	unsigned char count,Size;
	int len,tLen;
	char *buf;
	//Enable EP buffer
	//Select the pipe

	if(gusbtransf.BulkTfout.DevOpen == NO &&
			gusbtransf.IntMeasTf.DevOpen == NO)
	{
		PDBG("USB device not open! GO IN REMOTE!\n");
		FlushRx(EPT2);
		return;
	}

	if( /* (statEvnt & EPFULL0) || */ BufferFull())
	{
		//buffer is full -> now send a NAK to prevent the gamma for a crash
		wake_up_interruptible(&gusbtransf.BulkTfout.waitq);
		PDBG("%s: wake queue (1)\n",__FUNCTION__);
		FlushRx(EPT2);
	}
	spin_lock(&DevState.lock);
	DevState.UsbState.EpState[EPT2] = EP_IN_PROGRESS;
	spin_unlock(&DevState.lock);



	m66291_set_reg(CPU_FIFO_SELECT, m66291_read_reg(CPU_FIFO_SELECT)|EP2); 
	do {
		buffer=m66291_read_reg(CPU_FIFO_CONTROL);
	} while (buffer & Creq);

	buffer=m66291_read_reg(CPU_FIFO_CONTROL);
	count = buffer & CPU_DTLN;

	Size=PCK_SIZE;
	count = m66291_read_reg(CPU_FIFO_CONTROL)&CPU_DTLN;
	len=count;

	if (len == 0)
	{
		spin_unlock_wait(&gusbtransf.BulkTfout.lock);
		spin_lock(&gusbtransf.BulkTfout.lock);
		gusbtransf.BulkTfout.ready = YES;
		FlushRx(EPT2);
		spin_unlock(&gusbtransf.BulkTfout.lock);

		//reset toggle bit
		ep_stat[EPT2]->toggle_bit = 0;

		wake_up_interruptible(&gusbtransf.BulkTfout.waitq);
	}
	else
	{
		spin_unlock_wait(&gusbtransf.BulkTfout.lock);
		spin_lock(&gusbtransf.BulkTfout.lock);

		if(BULK_MAX_BUFFER_SIZE  == gusbtransf.BulkTfout.buffcnt)// + BULK_2TIMES_TRANSFER_SIZE))
		{//wake up to copy to user space so we can save time
			spin_unlock(&gusbtransf.BulkTfout.lock);
			wake_up_interruptible(&gusbtransf.BulkTfout.waitq);
			return;
		}

		tLen = len = len>BULK_MAX_BUFFER_SIZE?BULK_MAX_BUFFER_SIZE:len;
		buf  = gusbtransf.BulkTfout.buffer;
		while(tLen) {
			do {
				buffer=m66291_read_reg(CPU_FIFO_CONTROL);
			} while (buffer & Creq);
			buffer=m66291_read_reg(CPU_FIFO_DATA);
			*buf = buffer & 0xff;
			tLen--;
			buf++;
		}

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
		}
	}
	ep_stat[EPT2]->toggle_bit = ep_stat[EPT2]->toggle_bit ? 0 : 1;
	spin_lock(&DevState.lock);
	DevState.UsbState.EpState[EPT2] = EP_ENABLED;
	spin_unlock(&DevState.lock);
}


void INTR_int(U16 status, U16 int_enbl) {
	register U16 buffer;

	if( (status & EPB0_REQ) && (int_enbl & EPB0_ENB) ) {    /* endpoint0 interrupt request & enabled ? */
	} else {
		if( (status & EPB1_REQ) && (int_enbl & EPB1_ENB) ) {  /* endpoint1 interrupt request & enabled ? */
		}
		if( (status & EPB2_REQ) && (int_enbl & EPB2_ENB) ) {  /* endpoint2 interrupt request & enabled ? */
			read_data_in_epx(2);
		}
		if( (status & EPB3_REQ) && (int_enbl & EPB3_ENB) ) {  /* endpoint3 interrupt request & enabled ? */
		}
		if( (status & EPB4_REQ) && (int_enbl & EPB4_ENB) ) {  /* endpoint4 interrupt request & enabled ? */
		}
		if( (status & EPB5_REQ) && (int_enbl & EPB5_ENB) ) {  /* endpoint5 interrupt request & enabled ? */
			buffer = m66291_read_reg(EP5_0CONFIG);

		}
	}
}






/*Main Interrupt Rotine*/
static irqreturn_t usb_m66291_interrupt(int irq, void* ptr)
{
	struct UsbTransfer_st *m66291 = ptr;
	static unsigned short int_sts0, int_sts1, int_sts2, int_sts3;   /* USB interrupt status */
	static unsigned short int_enb0, int_enb1, int_enb2, int_enb3;   /* USB interrupt enable */
	unsigned short buffer;
	spinlock_t mr_lock=__SPIN_LOCK_UNLOCKED(mr_lock);

	m66291_set_reg(AUTO_RESPONSE_CONTROL,0x0001);
	spin_lock(&m66291->lock);
	int_sts0 = m66291_read_reg(INT_STATUS0);
	int_sts1 = m66291_read_reg(INT_STATUS1);
	int_sts2 = m66291_read_reg(INT_STATUS2);
	int_sts3 = m66291_read_reg(INT_STATUS3);
	int_enb0 = m66291_read_reg(INT_ENABLE0);
	int_enb1 = m66291_read_reg(INT_ENABLE1);
	int_enb2 = m66291_read_reg(INT_ENABLE2);
	int_enb3 = m66291_read_reg(INT_ENABLE3);

	if ((int_sts0 & VBUS) && (int_enb0 & VBSE)) {
          reset_usb();
          VBUScheck();

	}
	else if ((int_sts0 & RESM) && (int_enb0 & RSME)) {
		BusResume();
	}
	else if ((int_sts0 & SOFR) && (int_enb0 & SOFE)) {
		m66291_set_reg(INT_STATUS0, m66291_read_reg(INT_STATUS0)&(~SOFR)); 
	}
	else if ((int_sts0 & DVST) && (int_enb0 & DVSE)){
		m66291_set_reg(INT_STATUS0, m66291_read_reg(INT_STATUS0)& (~DVST));  //Clear the DVST

		switch ( int_sts0 & DVSQ ) {
			case RQ_POWR:
				break;
			case RQ_DFLT:
				bus_reset();
				break;
			case RQ_ADDS:
				buffer = m66291_read_reg(REQUEST_VALUE);
				break;
			case RQ_CNFG:
				buffer = m66291_read_reg(REQUEST_VALUE);
			case RQ_SPDP:
			case RQ_SPDD:
			case RQ_SPDA:
			case RQ_SPDC:
				BusSuspend();
				break;
			default:                /* error */					  
				break;
		}
	}

	/* Data processing of EP0 */
	else if( (int_sts0 & BEMP) && (int_enb0 & BEMPE) && (int_sts3 & (int_enb3 & EPB0_REQ))) {
		PDBG( "buffer overrun  interrupt recived in ep0");
	}
	else if((int_sts0 & INTN) && (int_enb0 & INTNE) && (int_sts2 & (int_enb2 & EPB0_REQ)) )  {
		PDBG( "buffer Not ready interrupt recived in ep0");
	}
	else if( (int_sts0 & INTR) && (int_enb0 & INTRE) && (int_sts1 & (int_enb1 & EPB0_REQ)) ) {
		PDBG( "buffer ready interrupt recived in ep0");
	}
	else if ((int_sts0 & CTRT) && (int_enb0 & CTRE)) {
		m66291_set_reg(INT_STATUS0, m66291_read_reg(INT_STATUS0) & (~CTRT));//Clear The CTRT Interrupt
		switch( int_sts0 & CTSQ ) {
			case RQ_RDDS:
			case RQ_WRND:
			case RQ_WRDS:
			spin_lock_irq(&mr_lock);
			HandleIrqEP0OUT();
			spin_unlock_irq(&mr_lock);
			break;

			case RQ_RDSS:
			m66291_set_reg(INT_STATUS3,m66291_read_reg(INT_STATUS3) & (~EPB0_REQ));
			m66291_set_reg(INT_ENABLE3,m66291_read_reg(INT_ENABLE3) & (~EPB0_ENB));
			m66291_set_reg(EP0_FIFO_CONTROL,m66291_read_reg(EP0_FIFO_CONTROL) | (EP0_CCPL));
			break;

			case RQ_SQER:
			m66291_set_reg(INT_STATUS3,m66291_read_reg(INT_STATUS3) & (~EPB0_REQ));
			m66291_set_reg(INT_ENABLE3,m66291_read_reg(INT_ENABLE3) & (~EPB0_ENB));
			m66291_set_reg(INT_STATUS3,m66291_read_reg(INT_STATUS1) & (~EPB0_REQ));
			m66291_set_reg(INT_ENABLE3,m66291_read_reg(INT_ENABLE1) & (~EPB0_ENB));
			m66291_set_reg(EP0_FIFO_CONTROL, m66291_read_reg(EP0_FIFO_CONTROL) | CCPL);
			break;

			case RQ_UNDF:
			case RQ_IDST:
			case RQ_WRSS:
			default:
			break;
		}
	}
	else if( (int_sts0 & INTN) && (int_enb0 & INTNE) && (int_sts2 & int_enb2) ) {
		m66291_set_reg(INT_ENABLE2,m66291_read_reg(INT_ENABLE2)&(~EPB2_ENB));
	}
	else if( (int_sts0 & INTR) && (int_enb0 & INTRE) && (int_sts1 & int_enb1) ) {

		m66291_set_reg(INT_ENABLE1,m66291_read_reg(INT_ENABLE1)&(~EPB2_ENB));
		INTR_int(int_sts1, int_enb1);
		m66291_set_reg(INT_ENABLE1,m66291_read_reg(INT_ENABLE1)|EPB2_ENB);

	}
	else if( (int_sts0 & BEMP) && (int_enb0 & BEMPE) && (int_sts3 & int_enb3) ) {
		m66291_set_reg(INT_ENABLE3,m66291_read_reg(INT_ENABLE3)&(~EPB2_ENB));
	}

	EP0clear();
	spin_unlock(&m66291->lock);

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

  spin_lock(&DevState.lock);
  
  if( USB_DISCONNECTED == DevState.UsbState.BusState)
    // MS: || !(altev & 0x08) || // no valid EOP received since last readout
    // MS: (altev & 0x30))    // 5ms of idle on upstream port 
  {
    spin_unlock(&DevState.lock);
    // deactivate_usb();
    // reset_usb(); // needs 2.5 secs in case of missing peer (PC)
    return -EIO;
  }
  spin_unlock(&DevState.lock);
  
  if( USB_M66291_DEVICES <= minor )  
    return -ENODEV;
  else 
    PDBG("minor number: %d\n", minor); 
  
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
    case USB_M66291_CNTRL_DEVICE:
      //vendor request OUT transfer via EP0
      init_waitqueue_head(&gusbtransf.CtrlTfout.waitq);
      spin_lock_init(&gusbtransf.CtrlTfout.lock);
      
      IntCrtlOutTransfer();
      
      if( !(gusbtransf.CtrlTfout.buffer = kmalloc(CTRL_MAX_BUFFER_SIZE, GFP_KERNEL)) )   
        return -ENOMEM;
      
      gusbtransf.CtrlTfout.timer = NULL;
      gusbtransf.CtrlTfout.DevOpen = YES;
      break;

    case USB_M66291_MEASUREMENT_DEVICE:
      InitMeasInTransfer();
      spin_lock_init(&gusbtransf.IntMeasTf.lock);
      gusbtransf.IntMeasTf.timer = &meastimer;
      gusbtransf.IntMeasTf.DevOpen = YES;
      break;

    case USB_M66291_BULK_DEVICE:
      // IN transfer
      spin_lock_init(&gusbtransf.BulkTfin.lock);
      gusbtransf.BulkTfin.timer = &bulkintimer;
      InitBulkInTransfer();

      gusbtransf.BulkTfout.timer   = NULL;
      gusbtransf.BulkTfout.DevOpen = YES;

      //Init interrupt transfer variables
      gusbtransf.IntCmdTf.buffer = NULL; //don't use it
      gusbtransf.IntCmdTf.timer  = NULL; //&intcmdtimer;
      break;
  }
  
  //both devices USB_M66291_MEASUREMENT_DEVICE & USB_ISP1181B_BULK_DEVICE
  if( gusbtransf.IntMeasTf.DevOpen || gusbtransf.BulkTfout.DevOpen ) { 
    //bulk OUT transfer wait queue
    InitBulkOutTransfer();
    init_waitqueue_head(&gusbtransf.BulkTfout.waitq);
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
	switch( minor ) 
	{
		case USB_M66291_CNTRL_DEVICE:
			if( NULL != gusbtransf.CtrlTfout.buffer ) 
			{
				kfree(gusbtransf.CtrlTfout.buffer);
				gusbtransf.CtrlTfout.buffer = NULL;
			}
			gusbtransf.CtrlTfout.DevOpen = NO;
			break;

		case USB_M66291_MEASUREMENT_DEVICE:
			gusbtransf.IntMeasTf.DevOpen = NO;
			gBulkOutTransferCnt--;
			break;

		case USB_M66291_BULK_DEVICE:
			gusbtransf.BulkTfout.DevOpen = NO;
			gBulkOutTransferCnt--;
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
inline unsigned short m66291_read_reg(ushort UsbReg)
{
	unsigned short sTmp;
	sTmp = readw(PORT_DATA + (UsbReg>>1)); 
	return sTmp;
}
//------------------------------------------------------------------
inline void m66291_set_reg(ushort UsbReg, ushort value)
{
	writew(value, PORT_DATA + (UsbReg >> 1));
}
//------------------------------------------------------------------
inline void FlushTx(int ep_no)
{
	if(ep_no == EPT0)
	{
		m66291_set_reg(EP0_FIFO_CONTROL, EP0_PID_STALL);
		m66291_set_reg(EP0_FIFO_CONTROL, EP0_PID_BUF);
	}
	else
	{
		m66291_set_reg(EP1_1CONFIG + ((ep_no-1)*4), EPi_PID_STALL);
		m66291_set_reg(EP1_1CONFIG + ((ep_no-1)*4), EPi_PID_BUF);
	}
}
//------------------------------------------------------------------
inline void FlushRx(int ep_no)
{
	switch (ep_no)
	{
		case EPT0:
			m66291_set_reg(EP0_FIFO_CONTROL, BCLR);
			break;

		case EPT2:
			m66291_set_reg(CPU_FIFO_CONTROL, BCLR);
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
	if( devfs_register_chrdev(USBGAMMA_MAJOR, USB_NAME, &usbdev_fops) )
#else
		//name "USB_ISP1181B" in /proc/devices
		if( register_chrdev(USBGAMMA_MAJOR,USB_NAME, &usbdev_fops) )
#endif
		{
			printk("usb_init() unable to get major no: %d for m66291 device!!!!!\n", USBGAMMA_MAJOR);
			return -EIO;
		}
	printk("M66291 registered\n");

#ifdef CONFIG_DEVFS_FS
	devfs_handle = devfs_mk_dir(NULL, USB_NAME, NULL);
#endif

	DevState.UsbState.BusState = USB_DISCONNECTED;

	// set registers of sh4
	if( 0 > (res = init_sh4_reg()) )       
	{
		return res; 
	}

	//install IRQ handler
	if( 0 == usb_m66291_interrupt_handler_installed )  
	{
		if( 0 > (request_irq(USB_M66291_IRQ, usb_m66291_interrupt, IRQF_DISABLED, USB_NAME, NULL)) )  
		{
			printk("usb_init: failed to register IRQ3 for USB device\n");
			free_irq(USB_M66291_IRQ, NULL);
			return -EIO;
		}
		usb_m66291_interrupt_handler_installed = 1;
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
	if( 0 > (res = init_USB_m66291_reg()) )  
	{ 
		return res; 
	}

	printk("USB device m66291 is initialized now\n");
	printk("M66291 USB Gamma device version: %s\nVendor ID: 0x%X \n", VERSION_STRA, 0x1670);

	return 0;
}

//------------------------------------------------------------------
int init_USB_m66291_reg(void)
{
	port_ctrl = ioremap(CTRL_PHY_ADR, 2);
	if (!port_ctrl)
	{
		PDBG( "HL_USB_Dev: ctrl port error\n");
		return -EIO;
	}
	port_data = ioremap(DATA_PHY_ADR, 2);
	if (!port_data)
	{
		PDBG( "HL_USB_Dev: data port error\n");
		return -EIO;
	}

	PDBG("m66291_usb_viper: ctrl(phy)=0x%p -> ctrl=0x%p\n",(void*)CTRL_PHY_ADR, port_ctrl);
	PDBG("m66291_usb_viper: data(phy)=0x%p -> data=0x%p\n",(void*)DATA_PHY_ADR, port_data);

	/* config */
	/**********/
	m66291_set_reg(REMOTE_WAKEUP, WKUP);
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

	local_irq_restore(flags);

	return 0;
}

//------------------------------------------------------------------
#ifdef _SELF_MODULE_UNLOAD_
void selfCleanUp( void ) 
{
	usb_cleanup();
	EIerror = -EIO;
}
#endif

//------------------------------------------------------------------
int usb_cleanup(void)
{
	int res = 0;

#ifdef CONFIG_DEVFS_FS
	if( (res = devfs_unregister_chrdev(USBGAMMA_MAJOR, USB_NAME)) )
	{
		printk("usb_cleanup() unable to release major %d for m66291\n", USBGAMMA_MAJOR);
	}
#else
	unregister_chrdev(USBGAMMA_MAJOR, USB_NAME);
#endif

	//detach USB device -> and SW Reset
	m66291_set_reg(USB_ENABLE, m66291_read_reg(USB_ENABLE) & ~USBE);
	m66291_set_reg(USB_ENABLE, m66291_read_reg(USB_ENABLE) & USBE);
	printk("USB_m66291 is disconnected from bus now\n");      

	if( usb_m66291_interrupt_handler_installed ) 
	{
		free_irq(USB_M66291_IRQ, NULL);
	}

	usb_initialized--;
	printk( "M66291 Module: %d is removed now \n", usb_initialized );
	return res;
}


static int __init init_usb_m66291(void)
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
		// enable_irq( USB_M66291_IRQ );
		usb_initialized++;
	}

	return ret;
}

static void __exit exit_usb_m66291(void)
{
	usb_cleanup();
	return;
}

module_init(init_usb_m66291);
module_exit(exit_usb_m66291);

MODULE_LICENSE("GPL");
