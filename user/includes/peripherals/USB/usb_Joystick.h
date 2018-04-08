#ifndef __USB_JOYSTICK_H__
#define __USB_JOYSTICK_H__

#include "stm32f10x.h"                  // Device header
#include "usb_property.h"

#if (USB_PROP == USB_PROP_JOYSTICK)

#include "usb_hid.h"

#define CONFIG_USB_CLASS_HID	1

//#define USB_IMR_MSK (USB_CNTR_CTRM | USB_CNTR_WKUPM | USB_CNTR_SUSPM | USB_CNTR_ERRM  | USB_CNTR_SOFM | USB_CNTR_ESOFM | USB_CNTR_RESETM )
#define USB_IMR_MSK (USB_CNTR_CTRM | USB_CNTR_RESETM)

/* buffer table base address */
#define USB_BTABLE_ADDRESS      (0x00)
/* EP0 rx/tx buffer base address */
#define USB_ENDP0_RXADDR        (0x18)
#define USB_ENDP0_TXADDR        (0x58)
/* EP1 tx buffer base address */
#define USB_ENDP1_TXADDR        (0x100)
/* MAX PACKET SIZE*/
#define USB_MAX_PACKET_SIZE		(0x40)
/* USB SUSPEND ENABLE*/
#define USB_SUSPEND_ENABLE		TRUE

#define USB_HID_DESCRIPTOR_NUM 	1

#pragma pack(1)
typedef struct _USB_CONFIGURATION{
	USB_CONFIGURATION_DESCRIPTOR 	configuration;
	USB_INTERFACES_DESCRIPTOR 		interfaces;
	USB_HID_DESCRIPTOR				hid;
	USB_ENDPOINT_DESCRIPTOR			endpoints;
}USB_CONFIGURATION;
#pragma pack()

#endif

#endif //__USB_JOYSTICK_H__

