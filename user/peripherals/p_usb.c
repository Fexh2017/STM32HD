#include "config.h"

#if CONFIG_USB_EN

#include "f_usb.h"
#include "f_rcc.h"
#include "f_gpio.h"
#include "f_nvic.h"

#include "p_usb.h"

#include "usb_property.h"


USB_STATUS 	Usb_status;
USB_DATA	Usb_data[8];
USB_REQUEST	Usb_request;


void p_usb_setup_stage(u8 ep)
{
	f_usb_read((u8*)&Usb_request, f_usb_GetEPRxAddr(ep), sizeof(USB_REQUEST));
}

void p_usb_in_stage(u8 ep)
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

void p_usb_out_stage(u8 ep)
{
	
}

void p_usb_setup0(void)
{
	p_usb_setup_stage(USB_ENDP0);
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
						p_usb_in_stage(USB_ENDP0);
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
						p_usb_in_stage(USB_ENDP0);
					}else
					{
						Usb_status.DAddr = Usb_request.wValue;
						Usb_status.State = E_USB_STATE_DEFAULT;
						Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
						Usb_data[USB_ENDP0].buf = NULL;
						Usb_data[USB_ENDP0].len = 0;
						Usb_data[USB_ENDP0].offset = 0;
						p_usb_in_stage(USB_ENDP0);
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
							p_usb_in_stage(USB_ENDP0);
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
							p_usb_in_stage(USB_ENDP0);
							break;
						case E_USB_GD_STRING:
							switch(Usb_request.wValue & 0x00FF)
							{
								case E_USB_STRING_LANGUAGE:
									Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
									Usb_data[USB_ENDP0].buf = (u8*)&Usb_language;
									Usb_data[USB_ENDP0].len = Usb_language.bLength;
									Usb_data[USB_ENDP0].offset = 0;
									p_usb_in_stage(USB_ENDP0);
									break;
								case E_USB_STRING_MANUFACTURER:
									Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
									Usb_data[USB_ENDP0].buf = (u8*)&Usb_string_manufacturer;
									Usb_data[USB_ENDP0].len = Usb_string_manufacturer.bLength;
									Usb_data[USB_ENDP0].offset = 0;
									p_usb_in_stage(USB_ENDP0);
									break;
								case E_USB_STRING_PRODUCT:
									Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
									Usb_data[USB_ENDP0].buf = (u8*)&Usb_string_product;
									Usb_data[USB_ENDP0].len = Usb_string_product.bLength;
									Usb_data[USB_ENDP0].offset = 0;
									p_usb_in_stage(USB_ENDP0);
									break;
								case E_USB_STRING_SERIALNUMBER:
									Usb_data[USB_ENDP0].dir = E_USB_DATA_DIR_TX;
									Usb_data[USB_ENDP0].buf = (u8*)&Usb_string_serialnumber;
									Usb_data[USB_ENDP0].len = Usb_string_serialnumber.bLength;
									Usb_data[USB_ENDP0].offset = 0;
									p_usb_in_stage(USB_ENDP0);
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
							p_usb_in_stage(USB_ENDP0);
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
							p_usb_in_stage(USB_ENDP0);
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
				p_usb_in_stage(USB_ENDP0);
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

void p_usb_in0(void)
{
	//USB_DBG("IN0 %3d/%d", Usb_data[USB_ENDP0].offset, Usb_data[USB_ENDP0].len);
	if(Usb_data[USB_ENDP0].dir == E_USB_DATA_DIR_TX)
	{
		p_usb_in_stage(USB_ENDP0);
	}else if(Usb_data[USB_ENDP0].dir == E_USB_DATA_DIR_IDLE)
	{
		if(Usb_request.bRequest == E_USB_REQ_SET_ADDRESS)
		{
			f_usb_SetDADDR(Usb_status.DAddr | USB_DADDR_EF);
		}
	}
}

void p_usb_out0(void)
{
	//USB_DBG("OUT0 ");
}

void p_usb_ctr(void)
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

void p_usb_istr(void)
{
	u16 _istr = f_usb_GetISTR();
	
#if (USB_IMR_MSK & USB_ISTR_RESET)
	if(_istr & USB_ISTR_RESET)
	{
		USB_DBG("RESET  ");
		if(Usb_property.reset)
		{
			Usb_property.reset();
			
			Usb_status.State = E_USB_STATE_DEFAULT;
			Usb_status.DAddr = 0;
			Usb_status.Ep_enable = (1 << USB_ENDP0);
			Usb_status.Ep_halt = 0;
			Usb_status.Config_val = 0;
		}
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
		p_usb_ctr();
	}
#endif
	
}
























#if CONFIG_USB_USE_LIB
#include "usb_lib.h"
#include "usb_istr.h"
#endif

void p_usb_init(void)
{
#if CONFIG_USB_PWR_EN
	f_rcc_enable(CONFIG_USB_PWR_CLK);
	f_gpio_init(CONFIG_USB_PWR_PORT,CONFIG_USB_PWR_PIN,GPIO_Mode_Out_PP);
	
	f_gpio_reset(CONFIG_USB_PWR_PORT,CONFIG_USB_PWR_PIN);
	delayms(200);
	f_gpio_set(CONFIG_USB_PWR_PORT,CONFIG_USB_PWR_PIN);
#endif
	
	f_nvic_config(USB_LP_CAN1_RX0_IRQn, CONFIG_NVIC_USB_LP);
	f_nvic_config(USBWakeUp_IRQn, CONFIG_NVIC_USB_WakeUp);
	
	f_rcc_enable(e_RCC_USB);
	
	//USB interrupts initialization
	p_usb_PowerOn();
	f_usb_SetISTR(0);
	
	Usb_property.init();
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	p_usb_istr();
}

void USBWakeUp_IRQHandler(void)
{
	EXTI->PR|=1<<18; //清除USB唤醒中断挂起位
}

void USB_HP_CAN_TX_IRQHandler(void)
{
}

void p_usb_PowerOn(void)
{
	f_usb_SetCNTR(USB_CNTR_FRES);
	f_usb_SetCNTR(0);
	f_usb_SetISTR(0);
	f_usb_SetCNTR(USB_CNTR_RESETM | USB_CNTR_SUSPM | USB_CNTR_WKUPM);
}

void p_usb_PowerOff(void)
{
	f_usb_SetCNTR(USB_CNTR_FRES);
	f_usb_SetISTR(0);
	f_usb_SetCNTR(USB_CNTR_FRES | USB_CNTR_PDWN);
}




#else
void p_usb_init(void){}
void p_usb_PowerOn(void){}
void p_usb_PowerOff(void){}
	
#endif
