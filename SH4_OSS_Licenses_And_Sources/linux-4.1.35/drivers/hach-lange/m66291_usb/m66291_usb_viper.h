/***************************************************************************
                          usbn9603.h  -  description
                             -------------------
    begin                : Mon Jun 7 2004
    copyright            : (C) 2004 by Thomas Siegmund
    email                : tsiegmund@hach-lange.de
    Author               : Nikhil Varghese
***************************************************************************/
#ifndef _USBN9603_H_
#define _USBN9603_H_

#define PDBG(fmt, args...) // printk(fmt, ## args)
//4KByte are neccessary because the device doesn't come back from NAK!!!!
#define BULK_MAX_BUFFER_SIZE   1024
#define CTRL_MAX_BUFFER_SIZE   1024

#define CTRL_2TIMES_TRANSFER_SIZE     (2*  8)
#define BULK_2TIMES_TRANSFER_SIZE     (2* 64)
#define not_last_pack 1
#define last_pack 2
static DECLARE_WAIT_QUEUE_HEAD(wq);
static int flag = 0;
/***************************************/
typedef enum {
	USB_NO_COMMAND = 0x00,

	BULK_DATA_OUT_AMOUNT = 0x10, //Host sends a predeterminted amount of data to device
	BULK_DATA_OUT_STREAM = 0x11, //Host sends a stream of data to device and finish with a ZERO LENGTH PACKAGE

	BULK_DATA_IN_AMOUNT  = 0x12, //Device sends a predeterminted amount of data to host
	BULK_DATA_IN_STREAM  = 0x13, //Device sends a stream of data to host and finish with a ZERO LENGTH PACKAGE

	CTRL_DATA_OUT        = 0x14, //Host sends data via control transfer to device
	CTRL_DATA_IN         = 0x15  //Device sends data via control transfer to host
} CtrlCommand;
/*****************************************************************
 * This structure represents last bulk command status
 ******************************************************************/
struct UsbInTransferTimer_st {
	int      bTimeOut;
	int      bTimerpresent;
	struct timer_list  TimerList;
};

struct UsbTransfer_st {
	CtrlCommand command;        /* current command */
	DWORD     dataSize;       /* number of bytes to be sent/received in current command */
	DWORD     rest2process;   /* remained number of bytes to be received/sent */
	BYTE      *buffer;
	DWORD     buffcnt;        /* current data position in buffer */
	int       ready;
	int       exitOnZLP;
	int       TransfActive;
	int       DevOpen;
	spinlock_t         lock;
	wait_queue_head_t  waitq;
	struct UsbInTransferTimer_st  *timer;
};

struct Usbn9603Transfer {
	struct UsbTransfer_st   CtrlTfout;
	struct UsbTransfer_st   BulkTfout;
	struct UsbTransfer_st   BulkTfin;
	struct UsbTransfer_st   IntCmdTf;
	struct UsbTransfer_st   IntMeasTf;
};

//int gDeviceOpen;
int gBulkInTimeOut;
int gMeasurementTimeOut;
/************* new ************************************/
struct Usbn9603Transfer  gusbtransf;
#ifdef _SELF_MODULE_UNLOAD_
int UsbBusError;
#endif

/*******************************************************/
/* RTX */
/*******************************************************/
unsigned char rtxEP1Len;
unsigned char rtxEP1Buf[TX_BULK_EP_FIFO_SIZE];
unsigned char rtxEP3Len;
unsigned char rtxEP3Buf[TX_INTR_EP_FIFO_SIZE_COMM];
unsigned char rtxEP5Len;
unsigned char rtxEP5Buf[TX_INTR_EP_FIFO_SIZE_MEAS];

/********************************************************
 **                    Prototypes
 *********************************************************/
inline unsigned short m66291_read_reg(ushort UsbReg);
inline unsigned long  UsbRd4B(ushort UsbReg);
inline unsigned short UsbRdData2B(void);
inline void UsbWr0B(ushort UsbReg);
inline void m66291_set_reg(ushort UsbReg, ushort value);
inline void UsbWr4B(ushort UsbReg, ulong value);
inline void UsbWrData2B(ushort value);

inline void SendBulkData(unsigned int count_t, unsigned char *buff,int packet);
inline int  SendInterruptData(BYTE *idata, DWORD size, int ep_no);

inline void FlushTx(int ep_no);
inline void FlushRx(int ep_no);
inline void clearIRQ3(void);

void rx_event_handler(void);
void alt_event_handler(void);
void tx_event_handler(void);
void nak_event_handler(void);

void zero_length_data_responseEP0(void);
void reset_usb(void);
void clear_usb_status(void);
void dbg_time(void);

void reinitiate_epx(int ep_no);
void SetMeasSendDataTimeout(void);
void SetBulkSendDataTimeout(void);

extern ushort *port_ctrl;
extern ushort *port_data;

#endif /* _USBN9603_H_ */
