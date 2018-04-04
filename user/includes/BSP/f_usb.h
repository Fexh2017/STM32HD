#ifndef __F_USB_H__
#define __F_USB_H__

#include "stm32f10x.h"                  // Device header

typedef enum{
	USB_ENDP0 = 0,
	USB_ENDP1,
	USB_ENDP2,
	USB_ENDP3,
	USB_ENDP4,
	USB_ENDP5,
	USB_ENDP6,
	USB_ENDP7,
}USB_EndPoints_Type;

typedef enum{
	/* double buffered endpoint direction */
	USB_EP_DBUF_ERR,
	USB_EP_DBUF_OUT,
	USB_EP_DBUF_IN
}USB_EP_DBUF_DIR_Type;

typedef enum 
{
	/* endpoint buffer number */
	USB_EP_NOBUF,
	USB_EP_BUF0,
	USB_EP_BUF1
}USB_EP_BUF_NUM_Type;

typedef enum{
	USB_EP_BULK        =(0x0000), /* EndPoint BULK */
	USB_EP_CONTROL     =(0x0200), /* EndPoint CONTROL */
	USB_EP_ISOCHRONOUS =(0x0400), /* EndPoint ISOCHRONOUS */
	USB_EP_INTERRUPT   =(0x0600), /* EndPoint INTERRUPT */
}USB_EndPoint_Type;


typedef enum{
	USB_EPR_TX_Status_DIS      =(0x0000), /* EndPoint TX DISabled */
	USB_EPR_TX_Status_STALL    =(0x0010), /* EndPoint TX STALLed */
	USB_EPR_TX_Status_NAK      =(0x0020), /* EndPoint TX NAKed */
	USB_EPR_TX_Status_VALID    =(0x0030), /* EndPoint TX VALID */
}USB_EPR_TX_Status_TypeDef;

typedef enum{
	USB_EPR_RX_Status_DIS      =(0x0000), /* EndPoint RX DISabled */
	USB_EPR_RX_Status_STALL    =(0x1000), /* EndPoint RX STALLed */
	USB_EPR_RX_Status_NAK      =(0x2000), /* EndPoint RX NAKed */
	USB_EPR_RX_Status_VALID    =(0x3000), /* EndPoint RX VALID */
}USB_EPR_RX_Status_TypeDef;

/* External variables --------------------------------------------------------*/
extern volatile u16 wIstr;  /* ISTR register last read value */


/* Exported functions ------------------------------------------------------- */
extern void f_usb_SetCNTR(u16 wRegValue);
extern void f_usb_SetISTR(u16 wRegValue);
extern void f_usb_SetDADDR(u16 wRegValue);
extern void f_usb_SetBTABLE(u16 wRegValue);
extern void f_usb_SetBTABLE(u16 wRegValue);
extern u16  f_usb_GetCNTR(void);
extern u16  f_usb_GetISTR(void);
extern u16  f_usb_GetFNR(void);
extern u16  f_usb_GetDADDR(void);
extern u16  f_usb_GetBTABLE(void);
extern void f_usb_SetENDPOINT(u8 bEpNum, u16 wRegValue);
extern u16  f_usb_GetENDPOINT(u8 bEpNum);
extern void f_usb_SetEPType(u8 bEpNum, u16 wType);
extern u16  f_usb_GetEPType(u8 bEpNum);
extern void f_usb_SetEPTxStatus(u8 bEpNum, u16 wState);
extern void f_usb_SetEPRxStatus(u8 bEpNum, u16 wState);
extern void f_usb_SetDouBleBuffEPStall(u8 bEpNum, u8 bDir);
extern u16  f_usb_GetEPTxStatus(u8 bEpNum);
extern u16  f_usb_GetEPRxStatus(u8 bEpNum);
extern void f_usb_SetEPTxValid(u8 bEpNum);
extern void f_usb_SetEPRxValid(u8 bEpNum);
extern void f_usb_SetEPTxStall(u8 bEpNum);
extern void f_usb_SetEPRxStall(u8 bEpNum);
extern void f_usb_SetEPTxNak(u8 bEpNum);
extern void f_usb_SetEPRxNak(u8 bEpNum);
extern u16  f_usb_GetTxStallStatus(u8 bEpNum);
extern u16  f_usb_GetRxStallStatus(u8 bEpNum);
extern void f_usb_SetEP_KIND(u8 bEpNum);
extern void f_usb_ClearEP_KIND(u8 bEpNum);
extern void f_usb_Set_Status_Out(u8 bEpNum);
extern void f_usb_Clear_Status_Out(u8 bEpNum);
extern void f_usb_SetEPDoubleBuff(u8 bEpNum);
extern void f_usb_ClearEPDoubleBuff(u8 bEpNum);
extern void f_usb_ClearEP_CTR_RX(u8 bEpNum);
extern void f_usb_ClearEP_CTR_TX(u8 bEpNum);
extern void f_usb_ToggleDTOG_RX(u8 bEpNum);
extern void f_usb_ToggleDTOG_TX(u8 bEpNum);
extern void f_usb_ClearDTOG_RX(u8 bEpNum);
extern void f_usb_ClearDTOG_TX(u8 bEpNum);
extern void f_usb_SetEPAddress(u8 bEpNum, u8 bAddr);
extern u8   f_usb_GetEPAddress(u8 bEpNum);
extern void f_usb_SetEPTxAddr(u8 bEpNum, u16 wAddr);
extern void f_usb_SetEPRxAddr(u8 bEpNum, u16 wAddr);
extern u16  f_usb_GetEPTxAddr(u8 bEpNum);
extern u16  f_usb_GetEPRxAddr(u8 bEpNum);
extern void f_usb_SetEPCountRxReg(u32 * pdwReg, u16 wCount);
extern void f_usb_SetEPTxCount(u8 bEpNum, u16 wCount);
extern void f_usb_SetEPRxCount(u8 bEpNum, u16 wCount);
extern u16  f_usb_GetEPTxCount(u8 bEpNum);
extern u16  f_usb_GetEPRxCount(u8 bEpNum);
extern void f_usb_SetEPDblBuf0Addr(u8 bEpNum, u16 wBuf0Addr);
extern void f_usb_SetEPDblBuf1Addr(u8 bEpNum, u16 wBuf1Addr);
extern void f_usb_SetEPDblBuffAddr(u8 bEpNum, u16 wBuf0Addr, u16 wBuf1Addr);
extern u16  f_usb_GetEPDblBuf0Addr(u8 bEpNum);
extern u16  f_usb_GetEPDblBuf1Addr(u8 bEpNum);
extern void f_usb_SetEPDblBuffCount(u8 bEpNum, u8 bDir, u16 wCount);
extern void f_usb_SetEPDblBuf0Count(u8 bEpNum, u8 bDir, u16 wCount);
extern void f_usb_SetEPDblBuf1Count(u8 bEpNum, u8 bDir, u16 wCount);
extern u16  f_usb_GetEPDblBuf0Count(u8 bEpNum);
extern u16  f_usb_GetEPDblBuf1Count(u8 bEpNum);
extern USB_EP_DBUF_DIR_Type f_usb_GetEPDblBufDir(u8 bEpNum);
extern void f_usb_FreeUserBuffer(u8 bEpNumbEpNum, u8 bDir);

extern void f_usb_write(u8 *buf, u16 addr, u16 len);
extern void f_usb_read(u8 *buf, u16 addr, u16 len);


#endif

