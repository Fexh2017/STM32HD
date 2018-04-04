#include "f_usb.h"

void f_usb_SetCNTR(u16 wRegValue)
{
	USB->CNTR = wRegValue;
}

u16 f_usb_GetCNTR(void)
{
	return USB->CNTR;
}

void f_usb_SetISTR(u16 wRegValue)
{
	USB->ISTR = wRegValue;
}

u16 f_usb_GetISTR(void)
{
	return USB->ISTR;
}

u16 f_usb_GetFNR(void)
{
	return USB->FNR;
}

void f_usb_SetDADDR(u16 wRegValue)
{
	USB->DADDR = wRegValue;
}

u16 f_usb_GetDADDR(void)
{
	return USB->DADDR;
}

void f_usb_SetBTABLE(u16 wRegValue)
{
	USB->BTABLE = wRegValue & 0xFFF8;
}

u16 f_usb_GetBTABLE(void)
{
	return USB->BTABLE;
}

void f_usb_SetENDPOINT(u8 bEpNum, u16 wRegValue)
{
	USB->EPxR[bEpNum].EPR = wRegValue;
}

u16 f_usb_GetENDPOINT(u8 bEpNum)
{
	return USB->EPxR[bEpNum].EPR;
}

void f_usb_SetEPType(u8 bEpNum, u16 wType)
{
	f_usb_SetENDPOINT(bEpNum,(f_usb_GetENDPOINT(bEpNum) & (~USB_EPR_EP_TYPE & USB_EPR_MASK)) | wType);
}

u16 f_usb_GetEPType(u8 bEpNum)
{
	return (f_usb_GetENDPOINT(bEpNum) & USB_EPR_EP_TYPE);
}

void f_usb_SetEPTxStatus(u8 bEpNum, u16 wState)
{
	register u16 _wRegVal;       
    _wRegVal = f_usb_GetENDPOINT(bEpNum) & (USB_EPR_STAT_TX | USB_EPR_MASK);
    /* toggle first bit ? */
    if((USB_EPR_STAT_TX_0 & wState)!= 0)
	{
      _wRegVal ^= USB_EPR_STAT_TX_0;
	}
    /* toggle second bit ?  */
    if((USB_EPR_STAT_TX_1 & wState)!= 0)
	{
      _wRegVal ^= USB_EPR_STAT_TX_1;
	}
    f_usb_SetENDPOINT(bEpNum, _wRegVal);
}

void f_usb_SetEPRxStatus(u8 bEpNum, u16 wState)
{
	register u16 _wRegVal;
    _wRegVal = f_usb_GetENDPOINT(bEpNum) & (USB_EPR_STAT_RX | USB_EPR_MASK);
    /* toggle first bit ? */
    if((USB_EPR_STAT_RX_0 & wState)!= 0)
	{
      _wRegVal ^= USB_EPR_STAT_RX_0;
	}
    /* toggle second bit ? */
    if((USB_EPR_STAT_RX_1 & wState)!= 0)
	{
      _wRegVal ^= USB_EPR_STAT_RX_1;
	}
    f_usb_SetENDPOINT(bEpNum, _wRegVal);
}

void f_usb_SetDouBleBuffEPStall(u8 bEpNum, u8 bDir)
{
  if (bDir == USB_EP_DBUF_OUT)
  { /* OUT double buffered endpoint */
    f_usb_SetENDPOINT(bEpNum, f_usb_GetENDPOINT(bEpNum) & ~USB_EPR_STAT_RX_0);
  }
  else if (bDir == USB_EP_DBUF_IN)
  { /* IN double buffered endpoint */
    f_usb_SetENDPOINT(bEpNum, f_usb_GetENDPOINT(bEpNum) & ~USB_EPR_STAT_RX_0);
  }
}

u16 f_usb_GetEPTxStatus(u8 bEpNum)
{
	return (f_usb_GetENDPOINT(bEpNum) & USB_EPR_STAT_TX);
}

u16 f_usb_GetEPRxStatus(u8 bEpNum)
{
	return (f_usb_GetENDPOINT(bEpNum) & USB_EPR_STAT_RX);
}

void f_usb_SetEPTxValid(u8 bEpNum)
{
	f_usb_SetEPTxStatus(bEpNum, USB_EPR_TX_Status_VALID);
}

void f_usb_SetEPRxValid(u8 bEpNum)
{
	f_usb_SetEPRxStatus(bEpNum, USB_EPR_RX_Status_VALID);
}

void f_usb_SetEPTxStall(u8 bEpNum)
{
	f_usb_SetEPTxStatus(bEpNum, USB_EPR_TX_Status_STALL);
}

void f_usb_SetEPRxStall(u8 bEpNum)
{
	f_usb_SetEPRxStatus(bEpNum, USB_EPR_RX_Status_STALL);
}

void f_usb_SetEPTxNak(u8 bEpNum)
{
	f_usb_SetEPTxStatus(bEpNum, USB_EPR_TX_Status_NAK);
}

void f_usb_SetEPRxNak(u8 bEpNum)
{
	f_usb_SetEPRxStatus(bEpNum, USB_EPR_RX_Status_NAK);
}

void f_usb_SetEP_KIND(u8 bEpNum)
{
	f_usb_SetENDPOINT(bEpNum,(f_usb_GetENDPOINT(bEpNum) | USB_EPR_EP_KIND) & USB_EPR_MASK);
}

void f_usb_ClearEP_KIND(u8 bEpNum)
{
	f_usb_SetENDPOINT(bEpNum,(f_usb_GetENDPOINT(bEpNum) & USB_EPR_MASK));
}

void f_usb_Clear_Status_Out(u8 bEpNum)
{
	f_usb_ClearEP_KIND(bEpNum);
}

void f_usb_Set_Status_Out(u8 bEpNum)
{
	f_usb_SetEP_KIND(bEpNum);
}

void f_usb_SetEPDoubleBuff(u8 bEpNum)
{
	f_usb_SetEP_KIND(bEpNum);
}

void f_usb_ClearEPDoubleBuff(u8 bEpNum)
{
	f_usb_ClearEP_KIND(bEpNum);
}

u16 f_usb_GetTxStallStatus(u8 bEpNum)
{
	return (f_usb_GetEPTxStatus(bEpNum) == USB_EPR_TX_Status_STALL);
}

u16 f_usb_GetRxStallStatus(u8 bEpNum)
{
	return (f_usb_GetEPRxStatus(bEpNum) == USB_EPR_RX_Status_STALL);
}

void f_usb_ClearEP_CTR_RX(u8 bEpNum)
{
	f_usb_SetENDPOINT(bEpNum, ~USB_EPR_CTR_RX & (f_usb_GetENDPOINT(bEpNum) & USB_EPR_MASK));
}

void f_usb_ClearEP_CTR_TX(u8 bEpNum)
{
	f_usb_SetENDPOINT(bEpNum, ~USB_EPR_CTR_TX & (f_usb_GetENDPOINT(bEpNum) & USB_EPR_MASK));
}

void f_usb_ToggleDTOG_RX(u8 bEpNum)
{
	f_usb_SetENDPOINT(bEpNum, USB_EPR_DTOG_RX | (f_usb_GetENDPOINT(bEpNum) & USB_EPR_MASK));
}

void f_usb_ToggleDTOG_TX(u8 bEpNum)
{
	f_usb_SetENDPOINT(bEpNum, USB_EPR_DTOG_TX | (f_usb_GetENDPOINT(bEpNum) & USB_EPR_MASK));
}

void f_usb_ClearDTOG_RX(u8 bEpNum)
{
	if((f_usb_GetENDPOINT(bEpNum) & USB_EPR_DTOG_RX) != 0)
	{
		f_usb_ToggleDTOG_RX(bEpNum);
	}
}

void f_usb_ClearDTOG_TX(u8 bEpNum)
{
	if((f_usb_GetENDPOINT(bEpNum) & USB_EPR_DTOG_TX) != 0)
	{
		f_usb_ToggleDTOG_TX(bEpNum);
	}
}

void f_usb_SetEPAddress(u8 bEpNum, u8 bAddr)
{
	f_usb_SetENDPOINT(bEpNum, (f_usb_GetENDPOINT(bEpNum) & USB_EPR_MASK) | bAddr);
}

u8 f_usb_GetEPAddress(u8 bEpNum)
{
	return (u8)(f_usb_GetENDPOINT(bEpNum) & USB_EPR_EA);
}

void f_usb_SetEPTxAddr(u8 bEpNum, u16 wAddr)
{
	USB_PMA->EPxR[bEpNum].TxAddr = ((wAddr >> 1) << 1);
}

void f_usb_SetEPRxAddr(u8 bEpNum, u16 wAddr)
{
	USB_PMA->EPxR[bEpNum].RxAddr = ((wAddr >> 1) << 1);
}

u16 f_usb_GetEPTxAddr(u8 bEpNum)
{
	return (u16)USB_PMA->EPxR[bEpNum].TxAddr;
}

u16 f_usb_GetEPRxAddr(u8 bEpNum)
{
	return (u16)USB_PMA->EPxR[bEpNum].RxAddr;
}

void f_usb_SetEPTxCount(u8 bEpNum, u16 wCount)
{
	USB_PMA->EPxR[bEpNum].TxCount = wCount;
}

void f_usb_SetEPCountRxReg(u32 *pdwReg, u16 wCount)
{
	u16 wNBlocks;
    if(wCount > 62)
	{
		wNBlocks = wCount >> 5;
		if((wCount & 0x1f) == 0)
		{
			wNBlocks--;
		}
		*pdwReg = (u32)((wNBlocks << 10) | 0x8000);
	}
    else 
	{
		wNBlocks = wCount >> 1;
		if((wCount & 0x1) != 0)
		{
			wNBlocks++;
		}
		*pdwReg = (u32)(wNBlocks << 10);
	}
}

void f_usb_SetEPRxCount(u8 bEpNum, u16 wCount)
{
	f_usb_SetEPCountRxReg((u32 *)&USB_PMA->EPxR[bEpNum].RxCount, wCount);
}

u16 f_usb_GetEPTxCount(u8 bEpNum)
{
	return (u16)(USB_PMA->EPxR[bEpNum].TxCount & 0x03FF);
}

u16 f_usb_GetEPRxCount(u8 bEpNum)
{
	return (u16)(USB_PMA->EPxR[bEpNum].RxCount & 0x03FF);
}

void f_usb_SetEPDblBuffAddr(u8 bEpNum, u16 wBuf0Addr, u16 wBuf1Addr)
{
	f_usb_SetEPTxAddr(bEpNum, wBuf0Addr);
	f_usb_SetEPRxAddr(bEpNum, wBuf1Addr);
}

void f_usb_SetEPDblBuf0Addr(u8 bEpNum, u16 wBuf0Addr)
{
	f_usb_SetEPTxAddr(bEpNum, wBuf0Addr);
}

void f_usb_SetEPDblBuf1Addr(u8 bEpNum, u16 wBuf1Addr)
{
	f_usb_SetEPRxAddr(bEpNum, wBuf1Addr);
}

u16 f_usb_GetEPDblBuf0Addr(u8 bEpNum)
{
	return f_usb_GetEPTxAddr(bEpNum);
}

u16 f_usb_GetEPDblBuf1Addr(u8 bEpNum)
{
	return f_usb_GetEPRxAddr(bEpNum);
}

void f_usb_SetEPDblBuffCount(u8 bEpNum, u8 bDir, u16 wCount)
{
	if(bDir == USB_EP_DBUF_OUT)
    { 
		f_usb_SetEPCountRxReg((u32 *)&USB_PMA->EPxR[bEpNum].TxCount, wCount);
		f_usb_SetEPCountRxReg((u32 *)&USB_PMA->EPxR[bEpNum].RxCount, wCount);
	} 
    else if(bDir == USB_EP_DBUF_IN)
	{
		USB_PMA->EPxR[bEpNum].TxCount = (u32)wCount;
		USB_PMA->EPxR[bEpNum].RxCount = (u32)wCount;
	}
}

void f_usb_SetEPDblBuf0Count(u8 bEpNum, u8 bDir, u16 wCount)
{
	if(bDir == USB_EP_DBUF_OUT)
    { 
		f_usb_SetEPCountRxReg((u32 *)&USB_PMA->EPxR[bEpNum].TxCount, wCount);
	} 
    else if(bDir == USB_EP_DBUF_IN)
	{
		USB_PMA->EPxR[bEpNum].TxCount = (u32)wCount;
	}
}

void f_usb_SetEPDblBuf1Count(u8 bEpNum, u8 bDir, u16 wCount)
{
	if(bDir == USB_EP_DBUF_OUT)
    { 
		f_usb_SetEPCountRxReg((u32 *)&USB_PMA->EPxR[bEpNum].RxCount, wCount);
	} 
    else if(bDir == USB_EP_DBUF_IN)
	{
		USB_PMA->EPxR[bEpNum].RxCount = (u32)wCount;
	}
}

u16 f_usb_GetEPDblBuf0Count(u8 bEpNum)
{
	return USB_PMA->EPxR[bEpNum].TxCount;
}

u16 f_usb_GetEPDblBuf1Count(u8 bEpNum)
{
	return USB_PMA->EPxR[bEpNum].RxCount;
}

USB_EP_DBUF_DIR_Type f_usb_GetEPDblBufDir(u8 bEpNum)
{
	if ((f_usb_GetEPDblBuf1Count(bEpNum) & 0xFC00) != 0)
	{
		return(USB_EP_DBUF_OUT);
	}
	else if ((f_usb_GetEPDblBuf0Count(bEpNum) & 0x03FF) != 0)
	{
		return(USB_EP_DBUF_IN);
	}
	else
	{
		return(USB_EP_DBUF_ERR);
	}
}

void f_usb_FreeUserBuffer(u8 bEpNum, u8 bDir)
{
	if (bDir == USB_EP_DBUF_OUT)
	{ /* OUT double buffered endpoint */
		f_usb_ToggleDTOG_TX(bEpNum);
	}
	else if (bDir == USB_EP_DBUF_IN)
	{ /* IN double buffered endpoint */
		f_usb_ToggleDTOG_RX(bEpNum);
	}
}


void f_usb_write(u8 *buf, u16 addr, u16 len)
{
	u32 n = (len + 1) >> 1;
	u32 i, temp1, temp2;
	u16 *pdwVal;
	pdwVal = (u16 *)(addr * 2 + USB_PMA_BASE);
	for (i = n; i != 0; i--)
	{
		temp1 = (u16) * buf;
		buf++;
		temp2 = temp1 | (u16) * buf << 8;
		*pdwVal++ = temp2;  
		pdwVal++;
		buf++;
	}
}

void f_usb_read(u8 *buf, u16 addr, u16 len)
{
	u32 n = (len + 1) >> 1;
	u32 i;
	u32 *pdwVal;
	pdwVal = (u32 *)(addr * 2 + USB_PMA_BASE);
	for (i = n; i != 0; i--)
	{
		*(u16*)buf++ = *pdwVal++;
		buf++;
	}
}









