#include "config.h"
#include "usb.h"

#if CONFIG_USB_EN

#include "f_usb.h"
#include "f_rcc.h"
#include "f_gpio.h"
#include "f_nvic.h"

#include "usb_property.h"


//USB_DEBUG配置
#if (CONFIG_USB_DEBUG_EN >= CONFIG_DEBUG_LV_H)
#define USB_DBG(...)					DEBUG("[USB DBG] ",__VA_ARGS__)
#define USB_DBGN(...)					DEBUGN("",__VA_ARGS__)
#define DEV_DBG(...)					DEBUG("[DEV DBG] ",__VA_ARGS__)
#define DEV_DBGN(...)					DEBUGN("",__VA_ARGS__)
#else
#define USB_DBGN(...)
#define USB_DBG(...)  
#define DEV_DBG(...) 
#define DEV_DBGN(...) 
#endif
#if (CONFIG_USB_DEBUG_EN >= CONFIG_DEBUG_LV_M)
#define USB_LOG(...)					DEBUG("[USB]     ",__VA_ARGS__)
#define DEV_LOG(...)					DEBUG("[DEV]     ",__VA_ARGS__)
#else
#define USB_LOG(...)  
#define DEV_LOG(...) 
#endif
#if (CONFIG_USB_DEBUG_EN >= CONFIG_DEBUG_LV_L)
#define USB_ERR(...)					DEBUG("[USB_ERR] ",__VA_ARGS__)
#define DEV_ERR(...)					DEBUG("[DEV_ERR] ",__VA_ARGS__)
#else
#define USB_ERR(...)  
#define DEV_ERR(...) 
#endif







USB_STATUS 	Usb_status;
USB_DATA	Usb_data[8];
USB_REQUEST	Usb_request;


void usb_setup_stage(u8 ep)
{
	f_usb_read((u8*)&Usb_request, f_usb_GetEPRxAddr(ep), sizeof(USB_REQUEST));
}

void usb_in_stage(u8 ep)
{
	if((Usb_status.Ep_enable & (1 << ep)) && (Usb_data[ep].dir == E_USB_DATA_DIR_TX))
	{
		u32 send_len = Usb_data[ep].len - Usb_data[ep].offset;
		send_len = send_len > Usb_status.Max_packet[ep] ? Usb_status.Max_packet[ep] : send_len;
		f_usb_write(Usb_data[ep].buf + Usb_data[ep].offset, f_usb_GetEPTxAddr(ep), send_len);
		f_usb_SetEPTxCount(ep, send_len);	
		f_usb_SetEPTxValid(ep);
		
		if(send_len < Usb_status.Max_packet[ep])
		{
			Usb_data[ep].dir = E_USB_DATA_DIR_IDLE;
			f_usb_SetEPRxCount(ep, Usb_status.Max_packet[ep]);
			f_usb_SetEPRxValid(ep);
		}
		
		Usb_data[ep].offset += send_len;
	}
}

void usb_out_stage(u8 ep)
{
	
}

void usb_setup0(void)
{
	usb_setup_stage(USB_ENDP0);
	USB_DBG("SETUP0");
	USB_DBGN("%02x %02x %04x %04x %04x", Usb_request.bmType, Usb_request.bRequest, Usb_request.wValue, Usb_request.wIndex, Usb_request.wLength);
	if((Usb_request.bmType & 0x60) == 0)
	{
		//标准请求
		switch(Usb_request.bRequest)
		{
			case E_USB_REQ_GET_STATUS:
				if(Usb_request.bmType == 0x80 && Usb_request.wLength == 2)
				{
					if(Usb_data[USB_ENDP0].dir == E_USB_DATA_DIR_IDLE)
					{
						Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
						Usb_data[USB_ENDP0].buf = (u8*)&Usb_status.Dev_state;
						Usb_data[USB_ENDP0].len = Usb_request.wLength;
						Usb_data[USB_ENDP0].offset = 0;
						usb_in_stage(USB_ENDP0);
					}
				}
				break;
			case E_USB_REQ_CLEAR_FEATURE:
				USB_ERR("E_USB_REQ_CLEAR_FEATURE");
				break;
			case E_USB_REQ_SET_FEATURE:
				USB_ERR("E_USB_REQ_SET_FEATURE");
				break;
			case E_USB_REQ_SET_ADDRESS:
				if(Usb_request.bmType == 0x00 && Usb_status.State != E_USB_STATE_CONFIGURED)
				{
					if(Usb_request.wValue != 0)
					{
						Usb_status.DAddr = Usb_request.wValue;
						Usb_status.State = E_USB_STATE_ADDRESS;
						Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
						Usb_data[USB_ENDP0].buf = NULL;
						Usb_data[USB_ENDP0].len = 0;
						Usb_data[USB_ENDP0].offset = 0;
						usb_in_stage(USB_ENDP0);
					}else
					{
						Usb_status.DAddr = Usb_request.wValue;
						Usb_status.State = E_USB_STATE_DEFAULT;
						Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
						Usb_data[USB_ENDP0].buf = NULL;
						Usb_data[USB_ENDP0].len = 0;
						Usb_data[USB_ENDP0].offset = 0;
						usb_in_stage(USB_ENDP0);
					}
				}
				break;
			case E_USB_REQ_GET_DESCRIPTOR:
				if(Usb_request.bmType == 0x80)
				{
					switch(Usb_request.wValue & 0xFF00)
					{
						case E_USB_GD_DEVICE:
							Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
							Usb_data[USB_ENDP0].buf = (u8*)&Usb_device_descriptor;
							Usb_data[USB_ENDP0].len = Usb_device_descriptor.bLength;
							Usb_data[USB_ENDP0].offset = 0;
							usb_in_stage(USB_ENDP0);
							break;
						case E_USB_GD_CONFIGURATION:
							Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
							Usb_data[USB_ENDP0].buf = (u8*)&Usb_configuration;
							if(Usb_request.wLength < Usb_configuration.configuration.wTotalLength)
							{
								Usb_data[USB_ENDP0].len = Usb_request.wLength;
							}else
							{
								Usb_data[USB_ENDP0].len = Usb_configuration.configuration.wTotalLength;
							}
							Usb_data[USB_ENDP0].offset = 0;
							usb_in_stage(USB_ENDP0);
							break;
						case E_USB_GD_STRING:
							switch(Usb_request.wValue & 0x00FF)
							{
								case E_USB_STRING_LANGUAGE:
									Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
									Usb_data[USB_ENDP0].buf = (u8*)&Usb_language;
									Usb_data[USB_ENDP0].len = Usb_language.bLength;
									Usb_data[USB_ENDP0].offset = 0;
									usb_in_stage(USB_ENDP0);
									break;
								case E_USB_STRING_MANUFACTURER:
									Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
									Usb_data[USB_ENDP0].buf = (u8*)&Usb_string_manufacturer;
									Usb_data[USB_ENDP0].len = Usb_string_manufacturer.bLength;
									Usb_data[USB_ENDP0].offset = 0;
									usb_in_stage(USB_ENDP0);
									break;
								case E_USB_STRING_PRODUCT:
									Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
									Usb_data[USB_ENDP0].buf = (u8*)&Usb_string_product;
									Usb_data[USB_ENDP0].len = Usb_string_product.bLength;
									Usb_data[USB_ENDP0].offset = 0;
									usb_in_stage(USB_ENDP0);
									break;
								case E_USB_STRING_SERIALNUMBER:
									Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
									Usb_data[USB_ENDP0].buf = (u8*)&Usb_string_serialnumber;
									Usb_data[USB_ENDP0].len = Usb_string_serialnumber.bLength;
									Usb_data[USB_ENDP0].offset = 0;
									usb_in_stage(USB_ENDP0);
									break;
								default:
									USB_ERR("UNKNOWN STRING");
							}
							break;
						case E_USB_GD_DEVICE_QUALIFIER:
							f_usb_SetEPTxStall(USB_ENDP0);
							break;
						case E_USB_GD_OTHER_SPEED_CONFIGURATION:
							USB_ERR("E_USB_GD_OTHER_SPEED_CONFIGURATION");
							break;
						default:
							USB_ERR("UNKNOWN DESCRIPTOR");
					}
				}else if(Usb_request.bmType == 0x81)
				{
					#if CONFIG_USB_CLASS_HID
					switch(Usb_request.wValue & 0xFF00)
					{
						case E_USB_GD_HID_REPORT:
							Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
							Usb_data[USB_ENDP0].buf = (u8*)Usb_hid_report_descriptor;
							Usb_data[USB_ENDP0].len = USB_HID_REPORT_LEN;
							Usb_data[USB_ENDP0].offset = 0;
							usb_in_stage(USB_ENDP0);
							break;
						default:
							USB_ERR("UNKNOWN HID DESCRIPTOR");
					}
					#endif
					
				}else{
					USB_ERR("UNKNOWN DESCRIPTOR TYPE");
				}
				break;
			case E_USB_REQ_SET_DESCRIPTOR:
				USB_ERR("E_USB_REQ_SET_DESCRIPTOR");
				break;
			case E_USB_REQ_GET_CONFIGURATION:
				USB_ERR("E_USB_REQ_GET_CONFIGURATION");
				break;
			case E_USB_REQ_SET_CONFIGURATION:
				if(Usb_request.bmType == 0x00 && Usb_status.State != E_USB_STATE_DEFAULT)
				{
					if(Usb_property.set_configuration)
					{
						if(Usb_property.set_configuration(Usb_request.wValue))
						{
							Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
							Usb_data[USB_ENDP0].buf = NULL;
							Usb_data[USB_ENDP0].len = 0;
							Usb_data[USB_ENDP0].offset = 0;
							usb_in_stage(USB_ENDP0);
						}
						else
						{
							USB_DBG("STALL");
							f_usb_SetEPTxStall(USB_ENDP0);
						}
					}else{
						USB_ERR("SET_CONFIGURATION UNDEFINED");
					}
				}else
				{
					USB_DBG("STALL");
					f_usb_SetEPTxStall(USB_ENDP0);
				}
				break;
			case E_USB_REQ_GET_INTERFACE:
				USB_ERR("E_USB_REQ_GET_INTERFACE");
				break;
			case E_USB_REQ_SET_INTERFACE:
				USB_ERR("E_USB_REQ_SET_INTERFACE");
				break;
			case E_USB_REQ_SYNCH_FRAME:
				USB_ERR("E_USB_REQ_SYNCH_FRAME");
				break;
			default:
				USB_ERR("UNKNOWN REQUEST");
				break;
		}
	}else if((Usb_request.bmType & 0x60) == 0x20)
	{
		//类请求
#if CONFIG_USB_CLASS_HID
		switch(Usb_request.bRequest)
		{
			case E_USB_REQ_GET_REPORT:
				USB_ERR("E_USB_REQ_GET_REPORT");
				break;
			case E_USB_REQ_GET_IDLE:
				USB_ERR("E_USB_REQ_GET_IDLE");
				break;
			case E_USB_REQ_GET_PROTOCOL:
				USB_ERR("E_USB_REQ_GET_PROTOCOL");
				break;
			case E_USB_REQ_SET_REPORT:
				USB_ERR("E_USB_REQ_SET_REPORT");
				break;
			case E_USB_REQ_SET_IDLE:
				if(Usb_hid_property.set_idle)
				{
					Usb_hid_property.set_idle(Usb_request.wIndex, Usb_request.wValue);
				}
				Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
				Usb_data[USB_ENDP0].buf = NULL;
				Usb_data[USB_ENDP0].len = 0;
				Usb_data[USB_ENDP0].offset = 0;
				usb_in_stage(USB_ENDP0);
				break;
			case E_USB_REQ_SET_PROTOCOL:
				USB_ERR("E_USB_REQ_SET_PROTOCOL");
				break;
			default:
				USB_ERR("UNKNOWN REQUEST");
		}
#endif
	}else{
		USB_ERR("UNKNOWN REQUEST TYPE");
	}
}

void usb_in0(void)
{
	//USB_DBG("IN0 %3d/%d", Usb_data[USB_ENDP0].offset, Usb_data[USB_ENDP0].len);
	if(Usb_data[USB_ENDP0].dir == E_USB_DATA_DIR_TX)
	{
		usb_in_stage(USB_ENDP0);
	}else if(Usb_data[USB_ENDP0].dir == E_USB_DATA_DIR_IDLE)
	{
		if(Usb_request.bRequest == E_USB_REQ_SET_ADDRESS)
		{
			f_usb_SetDADDR(Usb_status.DAddr | USB_DADDR_EF);
		}
	}
}

void usb_out0(void)
{
	//USB_DBG("OUT0 ");
}

void usb_ctr(void)
{
	u8  _ep;
	u16  _val;
	u16 _istr;
	while(((_istr = f_usb_GetISTR()) & USB_ISTR_CTR) != 0)
	{
		f_usb_SetISTR((u16)(~USB_ISTR_CTR));
		
		_ep = _istr & USB_ISTR_EP_ID;
		_val = f_usb_GetENDPOINT(_ep);
				
		if(_val & USB_EPR_SETUP)
		{
			f_usb_ClearEP_CTR_RX(_ep);
			if(Usb_property.setup[_ep])
			{
				Usb_property.setup[_ep]();
			}else
			{
				USB_ERR("FUNCTION UNDEFINED:SETUP%d",_ep);
			}
		}else if(_val & USB_EPR_CTR_RX)
		{
			f_usb_ClearEP_CTR_RX(_ep);
			if(Usb_property.out[_ep])
			{
				Usb_property.out[_ep]();
			}else
			{
				USB_ERR("FUNCTION UNDEFINED:OUT%d",_ep);
			}
		}else if(_val & USB_EPR_CTR_TX)
		{
			f_usb_ClearEP_CTR_TX(_ep);
			if(Usb_property.in[_ep])
			{
				Usb_property.in[_ep]();
			}else
			{
				USB_ERR("FUNCTION UNDEFINED:IN%d",_ep);
			}
		}else
		{
			USB_ERR("UNKNOWN CTR");
		}
	}
}

void usb_istr(void)
{
	u16 _istr = f_usb_GetISTR();
	
#if (USB_IMR_MSK & USB_ISTR_RESET)
	if(_istr & USB_ISTR_RESET)
	{
		USB_DBG("RESET  ");
		if(Usb_property.reset)
		{
			Usb_property.reset();
		}
		Usb_status.State = E_USB_STATE_DEFAULT;
		Usb_status.DAddr = 0;
		Usb_status.Ep_enable = (1 << USB_ENDP0);
		Usb_status.Ep_halt = 0;
		Usb_status.Config_val = 0;
		f_usb_SetISTR((u16)(~USB_ISTR_RESET));
	}
#endif
	
#if (USB_IMR_MSK & USB_ISTR_PMAOVR)
	if(_istr & USB_ISTR_PMAOVR)
	{
		USB_DBG("PMAOVR ");
		USB_ERR("TODO");
		f_usb_SetISTR((u16)(~USB_ISTR_PMAOVR));
	}
#endif
	
#if (USB_IMR_MSK & USB_ISTR_ERR)
	if(_istr & USB_ISTR_ERR)
	{
		USB_DBG("ERR    ");
		USB_ERR("TODO");
		f_usb_SetISTR((u16)(~USB_ISTR_ERR));
	}
#endif
	
#if (USB_IMR_MSK & USB_ISTR_WKUP)
	if(_istr & USB_ISTR_WKUP)
	{
		USB_DBG("WKUP   ");
		if(Usb_property.wakeup)
		{
			Usb_property.wakeup();
			Usb_status.State &= ~E_USB_STATE_SUSPENDED;
		}
		f_usb_SetISTR((u16)(~USB_ISTR_WKUP));
	}
#endif
	
#if (USB_IMR_MSK & USB_ISTR_SUSP)
	if(_istr & USB_ISTR_SUSP)
	{
		USB_DBG("SUSP   ");
		if(Usb_property.suspend)
		{
			Usb_property.suspend();
			Usb_status.State |= E_USB_STATE_SUSPENDED;
		}
		f_usb_SetISTR((u16)(~USB_ISTR_SUSP));
	}
#endif

#if (USB_IMR_MSK & USB_ISTR_SOF)
	if(_istr & USB_ISTR_SOF)
	{
		USB_DBG("SOF    ");
		USB_ERR("TODO");
		f_usb_SetISTR((u16)(~USB_ISTR_SOF));
	}
#endif
	
#if (USB_IMR_MSK & USB_ISTR_ESOF)
	if(_istr & USB_ISTR_ESOF)
	{
		USB_DBG("ESOF   ");
		USB_ERR("TODO");
		f_usb_SetISTR((u16)(~USB_ISTR_ESOF));
	}
#endif
	
#if (USB_IMR_MSK & USB_ISTR_CTR)
	if(_istr & USB_ISTR_CTR)
	{
		//USB_DBG("CTR    ");
		usb_ctr();
	}
#endif
	
}
























#if CONFIG_USB_USE_LIB
#include "usb_lib.h"
#include "usb_istr.h"
#endif
void usb_PowerOn(void)
{
	f_usb_SetCNTR(USB_CNTR_FRES);
	f_usb_SetCNTR(0);
	f_usb_SetISTR(0);
	f_usb_SetCNTR(USB_CNTR_RESETM | USB_CNTR_SUSPM | USB_CNTR_WKUPM);
}

void usb_PowerOff(void)
{
	f_usb_SetCNTR(USB_CNTR_FRES);
	f_usb_SetISTR(0);
	f_usb_SetCNTR(USB_CNTR_FRES | USB_CNTR_PDWN);
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	usb_istr();
}

void USBWakeUp_IRQHandler(void)
{
	EXTI->PR|=1<<18; //清除USB唤醒中断挂起位
}

void USB_HP_CAN_TX_IRQHandler(void)
{
}




u8 usb_init(void)
{
	DEV_LOG("usb init");
	
#if CONFIG_USB_PWR_EN
	f_gpio_init(CONFIG_USB_PWR_PIN,GPIO_Mode_Out_PP);

	f_gpio_reset(CONFIG_USB_PWR_PIN);
	delayms(200);
	f_gpio_set(CONFIG_USB_PWR_PIN);
	
#endif
	
	f_nvic_config(USB_LP_CAN1_RX0_IRQn, CONFIG_NVIC_USB_LP);
	f_nvic_config(USBWakeUp_IRQn, CONFIG_NVIC_USB_WakeUp);
	
	f_rcc_enable(e_RCC_USB);
	
	//USB interrupts initialization
	usb_PowerOn();
	f_usb_SetISTR(0);
	
	Usb_property.init();
	
	while(Usb_status.State != E_USB_STATE_CONFIGURED)
	{
	}
	delayms(10);
	return 0;
}

u8 usb_read(u32 addr, void *data, u32 len)
{
	return 0;
}

u8 usb_write(u32 addr, void *data, u32 len)
{
	if(addr == 1)
	{
		Usb_hid_property.send_event(data, len);
	}
	return 0;
}

#else
u8 usb_init(void){return 0xFF;}
u8 usb_read(u32 addr, void *data){return 0xFF;}
u8 usb_write(u32 addr, void *data){return 0xFF;}
#endif

	
	
const DEVICE Usb = 
{
	.name = "usb",
	.init = usb_init,
	.read = usb_read,
	.write = usb_write,
};
