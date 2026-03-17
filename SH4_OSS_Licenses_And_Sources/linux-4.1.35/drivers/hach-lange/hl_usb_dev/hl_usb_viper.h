/***************************************************************************
                          usbn9603.h  -  description
                             -------------------
    begin                : Mon Jun 7 2004
    copyright            : (C) 2004 by Thomas Siegmund
    email                : tsiegmund@hach-lange.de
***************************************************************************/
#ifndef _USBN9603_H_
#define _USBN9603_H_

// #define DEBUG

#ifdef DEBUG
#ifdef __KERNEL__  //Debug output 
#define PDBG(fmt, args...) printk(fmt, ## args) 
#endif

#else //No debug output
#define PDBG(fmt, args...)
#endif
//////////////

//#define EVENT_CONTROL
//#undef  EVENT_CONTROL 
/*******************************************************/

//4KByte are neccessary because the device doesn't come back from NAK!!!!
#define BULK_MAX_BUFFER_SIZE   1024
#define CTRL_MAX_BUFFER_SIZE   1024

#define CTRL_2TIMES_TRANSFER_SIZE     (2*  8)
#define BULK_2TIMES_TRANSFER_SIZE     (2* 64)
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
/*
#pragma pack(push, old_pack_val, 1)
typedef struct _INT_IN_TRANSFER_8_BYTE {
    //now 8 bytes for size
    DWORD   size_MSB;
    DWORD   size_LSB;
    } int_transfer;
#pragma pack(pop, old_pack_val)
*/
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
//  int       exitSendZLP;
//  int       userbreak;
  int       TransfActive;
  int       DevOpen;
spinlock_t         lock;
wait_queue_head_t  waitq;
struct UsbInTransferTimer_st  *timer;
};

struct Usbn9603Transfer {
//struct UsbTransfer_st   CtrlTfin;
struct UsbTransfer_st   CtrlTfout;

struct UsbTransfer_st   BulkTfout;
struct UsbTransfer_st   BulkTfin;

struct UsbTransfer_st   IntCmdTf;
struct UsbTransfer_st   IntMeasTf;
//struct timer_list       AliveTimer;
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
inline unsigned short UsbRd2B(ushort UsbReg);
inline unsigned long  UsbRd4B(ushort UsbReg);
inline unsigned short UsbRdData2B(void);
inline void UsbWr0B(ushort UsbReg);
inline void UsbWr2B(ushort UsbReg, ushort value);
inline void UsbWr4B(ushort UsbReg, ulong value);
inline void UsbWrData2B(ushort value);

inline void SendBulkData(void);
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
