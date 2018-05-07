#ifndef __USB_PROPERTY_H__
#define __USB_PROPERTY_H__

#include "stm32f10x.h"                  // Device header

#define USB_PROP_NONE		0
#define USB_PROP_JOYSTICK	1

#if CONFIG_USB_EN

#define USB_PROP			USB_PROP_JOYSTICK

typedef struct _USB_PROPERTY{
	void (*init)(void);
	void (*reset)(void);
	void (*setup[8])(void);
	void (*in[8])(void);
	void (*out[8])(void);
	void (*suspend)(void);
	void (*wakeup)(void);
	u8 (*set_configuration)(u16 val);
}USB_PROPERTY;

#pragma pack(1)
typedef struct _USB_DEVICE_DESCRIPTOR{
	u8  bLength;
	u8  bDescriptorType;
	u16 bcdUSB;
	u8  bDeviceClass;
	u8  bDeviceSubClass;
	u8  bDeviceProtocol;
	u8  bMaxPacketSize;
	u16 idVender;
	u16 idProduct;
	u16 bcdDevice;
	u8  iManufacturer;
	u8  iProduct;
	u8  iSerialNumber;
	u8  bNumConfigurations;
}USB_DEVICE_DESCRIPTOR;
#pragma pack()

#pragma pack(1)
typedef struct _USB_CONFIGURATION_DESCRIPTOR{
	u8  bLength;
	u8  bDescriptorType;
	u16 wTotalLength;
	u8  bNumberInterfaces;
	u8  bConfigurationValue;
	u8  iConfiguration;
	u8  bmAttributes;
	u8  bMaxPower;
}USB_CONFIGURATION_DESCRIPTOR;
#pragma pack()

#pragma pack(1)
typedef struct _USB_INTERFACES_DESCRIPTOR{
	u8  bLength;
	u8  bDescriptorType;
	u8  bInterfaceNumber;
	u8  bAlternateSettiog;
	u8  bNumEndpoints;
	u8  bInterfaceClass;
	u8  bInterfaceSubClass;
	u8  bInterfaceProtocol;
	u8  iInterface;
}USB_INTERFACES_DESCRIPTOR;
#pragma pack()

#pragma pack(1)
typedef struct _USB_ENDPOINT_DESCRIPTOR{
	u8  bLength;
	u8  bDescripterType;
	u8  bEndpointAddress;
	u8  bmAttributes;
	u16 wMaxPacketSize;
	u8  bInterval;
}USB_ENDPOINT_DESCRIPTOR;
#pragma pack()

#pragma pack(1)
typedef struct _USB_LANGUAGE_DESCRIPTOR{
	u8  bLength;
	u8  bDescripterType;
	u16 wLangID;
}USB_LANGUAGE_DESCRIPTOR;
#pragma pack()

#pragma pack(1)
typedef struct _USB_STRING_DESCRIPTOR{
	u8  bLength;
	u8  bDescripterType;
	u16 wString[31];
}USB_STRING_DESCRIPTOR;
#pragma pack()


#include "usb_Joystick.h"

extern const USB_PROPERTY 				Usb_property;
extern const USB_DEVICE_DESCRIPTOR 		Usb_device_descriptor;
extern const USB_CONFIGURATION			Usb_configuration;
extern const USB_LANGUAGE_DESCRIPTOR	Usb_language;
extern const USB_STRING_DESCRIPTOR		Usb_string_manufacturer;
extern const USB_STRING_DESCRIPTOR 		Usb_string_product;
extern const USB_STRING_DESCRIPTOR 		Usb_string_serialnumber;

#endif

#endif

