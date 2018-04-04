#include "usb_property.h"
#include "f_usb.h"
#include "p_usb.h"
#include "config.h"

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
	f_usb_SetEPRxCount(USB_ENDP0, USB_MAX_PACKET_SIZE);
	f_usb_SetEPRxValid(USB_ENDP0);
	
	/* Initialize Endpoint 1 */
	f_usb_SetEPType(USB_ENDP1, USB_EP_INTERRUPT);
	f_usb_SetEPTxAddr(USB_ENDP1, USB_ENDP1_TXADDR);
	f_usb_SetEPTxCount(USB_ENDP1, 4);
	f_usb_SetEPRxStatus(USB_ENDP1, USB_EPR_RX_Status_DIS);
	f_usb_SetEPTxStatus(USB_ENDP1, USB_EPR_TX_Status_NAK);
	
	/* Set this device to response on default address */
	f_usb_SetEPAddress(USB_ENDP0, 0);
	f_usb_SetEPAddress(USB_ENDP1, 1);
	
	f_usb_SetDADDR(0 | USB_DADDR_EF);
	
	Usb_status.Max_packet[0] = USB_MAX_PACKET_SIZE;
	Usb_status.Max_packet[1] = 4;
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
		p_usb_setup0,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	},
	.in = {
		p_usb_in0,
		usb_joystick_in1,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	},
	.out = {
		p_usb_out0,
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
    0x05,          /*Usage Page(Generic Desktop)*/
    0x01,
    0x09,          /*Usage(Mouse)*/
    0x02,
    0xA1,          /*Collection(Logical)*/
    0x01,
    0x09,          /*Usage(Pointer)*/
    0x01,
    /* 8 */
    0xA1,          /*Collection(Linked)*/
    0x00,
    0x05,          /*Usage Page(Buttons)*/
    0x09,
    0x19,          /*Usage Minimum(1)*/
    0x01,
    0x29,          /*Usage Maximum(3)*/
    0x03,
    /* 16 */
    0x15,          /*Logical Minimum(0)*/
    0x00,
    0x25,          /*Logical Maximum(1)*/
    0x01,
    0x95,          /*Report Count(3)*/
    0x03,
    0x75,          /*Report Size(1)*/
    0x01,
    /* 24 */
    0x81,          /*Input(Variable)*/
    0x02,
    0x95,          /*Report Count(1)*/
    0x01,
    0x75,          /*Report Size(5)*/
    0x05,
    0x81,          /*Input(Constant,Array)*/
    0x01,
    /* 32 */
    0x05,          /*Usage Page(Generic Desktop)*/
    0x01,
    0x09,          /*Usage(X axis)*/
    0x30,
    0x09,          /*Usage(Y axis)*/
    0x31,
    0x09,          /*Usage(Wheel)*/
    0x38,
    /* 40 */
    0x15,          /*Logical Minimum(-127)*/
    0x81,
    0x25,          /*Logical Maximum(127)*/
    0x7F,
    0x75,          /*Report Size(8)*/
    0x08,
    0x95,          /*Report Count(3)*/
    0x03,
    /* 48 */
    0x81,          /*Input(Variable, Relative)*/
    0x06,
    0xC0,          /*End Collection*/
    0x09,
    0x3c,
    0x05,
    0xff,
    0x09,
    /* 56 */
    0x01,
    0x15,
    0x00,
    0x25,
    0x01,
    0x75,
    0x01,
    0x95,
    /* 64 */
    0x02,
    0xb1,
    0x22,
    0x75,
    0x06,
    0x95,
    0x01,
    0xb1,
    /* 72 */
    0x01,
    0xc0									
};

const u32 Usb_hid_report_len = sizeof(Usb_hid_report_descriptor);

const USB_DEVICE_DESCRIPTOR Usb_device_descriptor = {
	.bLength 			= 18,
	.bDescriptorType 	= 0x01,
	.bcdUSB 			= 0x0200,
	.bDeviceClass 		= 0,
	.bDeviceSubClass 	= 0,
	.bDeviceProtocol 	= 0,
	.bMaxPacketSize 	= USB_MAX_PACKET_SIZE,
	.idVender 			= 0x0483,
	.idProduct 			= 0x5714,
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
		.bInterfaceNumber		= 0x00,
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
		.bcdHID					= 0x0100,
		.bCountyCode			= 33,
		.bNumDescriptors		= 0x01,
		.bDescriptorType 		= 0x22,
		.wDescriptorLength 		= USB_HID_REPORT_LEN,
	},
	.endpoints			= {
		.bLength				= 7,
		.bDescripterType		= 0x05,
		.bEndpointAddress		= 0x81,
		.bmAttributes			= 0x03,
		.wMaxPacketSize			= 0x0004,
		.bInterval				= 0x20,
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
	.wString			= L"ÐéÄâÊó±ê",
};

const USB_STRING_DESCRIPTOR Usb_string_serialnumber = {
	.bLength			= 20,
	.bDescripterType	= 0x03,
	.wString			= L"STM32F103",
};



#endif

