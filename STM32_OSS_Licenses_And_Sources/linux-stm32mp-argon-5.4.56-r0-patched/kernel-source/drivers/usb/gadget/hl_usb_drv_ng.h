#ifndef __HL_USB_DRV_NG_H__
#define __HL_USB_DRV_NG_H__

/***********************************************************************/
/*********************** IO Control codes ******************************/
/***********************************************************************/
/* 'M' as magic number */
#define  USB_IO_MAGIC      'M'
#define  USB_IO_RESET      _IO(USB_IO_MAGIC, 0)

#define  USB_IO_CONN_USB                _IO(USB_IO_MAGIC, 1) //, connect)

#define   HL_USB_NG_MAJOR   240




#endif