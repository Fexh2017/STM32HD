#include "config.h"
#include "usb_property.h"
#include "f_usb.h"
#include "usb.h"

//USB_DEBUG≈‰÷√
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



#if (USB_PROP == USB_PROP_JOYSTICK)

void usb_joystick_init(void)
{
	f_usb_SetCNTR(USB_IMR_MSK);
	Usb_status.State = E_USB_STATE_UNCONNECT;
}

void usb_joystick_reset(void)
{
	f_usb_SetBTABLE(USB_BTABLE_ADDRESS);
	
	/* Initialize Endpoint 0 */
	f_usb_SetEPType(USB_ENDP0, USB_EP_CONTROL);
	f_usb_SetEPTxStall(USB_ENDP0);
	f_usb_SetEPRxAddr(USB_ENDP0, USB_ENDP0_RXADDR);
	f_usb_SetEPTxAddr(USB_ENDP0, USB_ENDP0_TXADDR);
	f_usb_Clear_Status_Out(USB_ENDP0);
	f_usb_SetEPRxCount(USB_ENDP0, USB_EP0_MAX_PACKET_SIZE);
	f_usb_SetEPRxValid(USB_ENDP0);
	
	/* Initialize Endpoint 1 */
	f_usb_SetEPType(USB_ENDP1, USB_EP_INTERRUPT);
	f_usb_SetEPTxAddr(USB_ENDP1, USB_ENDP1_TXADDR);
	f_usb_SetEPTxCount(USB_ENDP1, USB_EP1_MAX_PACKET_SIZE);
	f_usb_SetEPRxStatus(USB_ENDP1, USB_EPR_RX_Status_DIS);
	f_usb_SetEPTxStatus(USB_ENDP1, USB_EPR_TX_Status_NAK);
	
	/* Set this device to response on default address */
	f_usb_SetEPAddress(USB_ENDP0, 0);
	f_usb_SetEPAddress(USB_ENDP1, 1);
	
	f_usb_SetDADDR(0 | USB_DADDR_EF);
	
	Usb_status.Max_packet[0] = USB_EP0_MAX_PACKET_SIZE;
	Usb_status.Max_packet[1] = USB_EP1_MAX_PACKET_SIZE;
	Usb_status.Dev_state = 0x02;
}

u8 usb_joystick_set_configuration(u16 val)
{
	if(val == 1)
	{
		Usb_status.Ep_enable |= (1 << USB_ENDP1);
		Usb_status.State = E_USB_STATE_CONFIGURED;
		Usb_status.Config_val = val;
		USB_LOG("configured");
		return 1;
	}else if(val == 0)
	{
		Usb_status.Ep_enable &= ~(1 << USB_ENDP1);
		Usb_status.State = E_USB_STATE_ADDRESS;
		Usb_status.Config_val = val;
		USB_LOG("rebuild");
		return 1;
	}
	return 0;
}

void usb_joystick_send_event(u8* buf, u32 len)
{
	if(len > USB_EP1_MAX_PACKET_SIZE)
	{
		USB_ERR("EP1 OVERFLOW");
	}
	f_usb_write(buf, f_usb_GetEPTxAddr(USB_ENDP1), len);
	f_usb_SetEPTxValid(USB_ENDP1);
}

void usb_joystick_in1(void)
{
	
}

const USB_PROPERTY Usb_property = {
	.init = usb_joystick_init,
	.reset = usb_joystick_reset,
	.setup = {
		usb_setup0,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	},
	.in = {
		usb_in0,
		usb_joystick_in1,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	},
	.out = {
		usb_out0,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	},
	.suspend = NULL,
	.wakeup = NULL,
	.set_configuration = usb_joystick_set_configuration,
};

const USB_HID_PROPERTY Usb_hid_property = {
	.set_idle = NULL,
	.send_event = usb_joystick_send_event,
};


const u8 Usb_hid_report_descriptor[] = {
USB_HID_REPORT_USAGE_PAGE(e_USB_HID_UP_GENERIC_DESKTOP)
USB_HID_REPORT_USAGE(e_USB_HID_USAGE_MOUSE)
USB_HID_REPORT_COLLECTION(e_USB_HID_COL_APPLICATION)
	USB_HID_REPORT_USAGE(e_USB_HID_USAGE_POINTER)
	USB_HID_REPORT_COLLECTION(e_USB_HID_COL_PHYSICAL)
		USB_HID_REPORT_USAGE_PAGE(e_USB_HID_UP_BUTTON)
		USB_HID_REPORT_USAGE_MIN(1)
		USB_HID_REPORT_USAGE_MAX(5)
		USB_HID_REPORT_LOGICAL_MIN(0)
		USB_HID_REPORT_LOGICAL_MAX(1)
		USB_HID_REPORT_REPORT_SIZE(1)
		USB_HID_REPORT_REPORT_CNT(5)
		USB_HID_REPORT_INPUT(e_USB_HID_VAL_DATA | e_USB_HID_VAL_VAR | e_USB_HID_VAL_ABS)
		
		USB_HID_REPORT_REPORT_SIZE(3)
		USB_HID_REPORT_REPORT_CNT(1)
		USB_HID_REPORT_INPUT(e_USB_HID_VAL_CONST | e_USB_HID_VAL_ARRAY | e_USB_HID_VAL_ABS)
	
//		USB_HID_REPORT_USAGE_PAGE(e_USB_HID_UP_GENERIC_DESKTOP)
//		USB_HID_REPORT_USAGE(e_USB_HID_USAGE_X)
//		USB_HID_REPORT_USAGE(e_USB_HID_USAGE_Y)
//		USB_HID_REPORT_LOGICAL_MIN_16(-32767)
//		USB_HID_REPORT_LOGICAL_MAX_16(32767)
//		USB_HID_REPORT_REPORT_SIZE(16)
//		USB_HID_REPORT_REPORT_CNT(2)

		USB_HID_REPORT_USAGE_PAGE(e_USB_HID_UP_GENERIC_DESKTOP)
		USB_HID_REPORT_USAGE(e_USB_HID_USAGE_X)
		USB_HID_REPORT_USAGE(e_USB_HID_USAGE_Y)
		USB_HID_REPORT_USAGE(e_USB_HID_USAGE_WHEEL)
		USB_HID_REPORT_LOGICAL_MIN_16(-32767)
		USB_HID_REPORT_LOGICAL_MAX_16(32767)
		USB_HID_REPORT_REPORT_SIZE(16)
		USB_HID_REPORT_REPORT_CNT(3)
		USB_HID_REPORT_INPUT(e_USB_HID_VAL_DATA | e_USB_HID_VAL_VAR | e_USB_HID_VAL_REL)
	USB_HID_REPORT_ENDCOLLECTION()
	
USB_HID_REPORT_ENDCOLLECTION()
};

const u32 Usb_hid_report_len = sizeof(Usb_hid_report_descriptor);

const USB_DEVICE_DESCRIPTOR Usb_device_descriptor = {
	.bLength 			= 18,
	.bDescriptorType 	= 0x01,
	.bcdUSB 			= 0x0200,
	.bDeviceClass 		= 0,
	.bDeviceSubClass 	= 0,
	.bDeviceProtocol 	= 0,
	.bMaxPacketSize 	= USB_EP0_MAX_PACKET_SIZE,
	.idVender 			= 0x0483,
	.idProduct 			= 0x5711,
	.bcdDevice 			= 0x0200,
	.iManufacturer 		= 1,
	.iProduct 			= 2,
	.iSerialNumber 		= 3,
	.bNumConfigurations = 1,
};

const USB_CONFIGURATION Usb_configuration = {
	.configuration		= {
		.bLength 				= 9,
		.bDescriptorType 		= 0x02,
		.wTotalLength 			= 34,
		.bNumberInterfaces 		= 0x01,
		.bConfigurationValue 	= 0x01,
		.iConfiguration 		= 0x00,
		.bmAttributes 			= 0xE0,
		.bMaxPower 				= 0x32,
	},
	.interfaces			= {
		.bLength				= 9,
		.bDescriptorType		= 0x04,
		.bInterfaceNumber		= 0x01,
		.bAlternateSettiog		= 0x00,
		.bNumEndpoints			= 0x01,
		.bInterfaceClass		= 0x03,
		.bInterfaceSubClass		= 0x01,
		.bInterfaceProtocol		= 0x02,
		.iInterface				= 0,
	},
	.hid				= {
		.bLength				= 9,
		.bDescripterType		= 0x21,
		.bcdHID					= 0x0110,
		.bCountyCode			= 0,
		.bNumDescriptors		= 0x01,
		.bDescriptorType 		= 0x22,
		.wDescriptorLength 		= USB_HID_REPORT_LEN,
	},
	.endpoints			= {
		.bLength				= 7,
		.bDescripterType		= 0x05,
		.bEndpointAddress		= 0x81,
		.bmAttributes			= 0x03,
		.wMaxPacketSize			= (u16)USB_EP1_MAX_PACKET_SIZE,
		.bInterval				= 0x02,
	},
};

const USB_LANGUAGE_DESCRIPTOR Usb_language = {
	.bLength			= 4,
	.bDescripterType	= 0x03,
	.wLangID			= 0x0409,
};

const USB_STRING_DESCRIPTOR Usb_string_manufacturer = {
	.bLength			= 14,
	.bDescripterType	= 0x03,
	.wString			= L"DDFexh",
};

const USB_STRING_DESCRIPTOR Usb_string_product = {
	.bLength			= 10,
	.bDescripterType	= 0x03,
	.wString			= L"–Èƒ‚ Û±Í",
};

const USB_STRING_DESCRIPTOR Usb_string_serialnumber = {
	.bLength			= 20,
	.bDescripterType	= 0x03,
	.wString			= L"STM32F103",
};



#endif

