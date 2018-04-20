#ifndef __USB_HID_H__
#define __USB_HID_H__

#include "stm32f10x.h"                  // Device header

#define USB_HID_REPORT_LEN		Usb_hid_report_len

typedef struct _USB_HID_PROPERTY{
	void (*set_idle)(u16 interface, u16 value);
	void (*send_event)(u8* buf,u32 len);
}USB_HID_PROPERTY;

#pragma pack(1)
typedef struct _USB_HID_DESCRIPTOR{
	u8  bLength;
	u8  bDescripterType;
	u16 bcdHID;
	u8  bCountyCode;
	u8  bNumDescriptors;
	u8  bDescriptorType;
	u16 wDescriptorLength;
}USB_HID_DESCRIPTOR;
#pragma pack()

#define USB_HID_REPORT_INPUT(v) 		0x81,v,
#define USB_HID_REPORT_OUTPUT(v)		0x91,v,
#define USB_HID_REPORT_FEATURE(v)		0xB1,v,
typedef enum _USB_HID_REPORT_VAL{
	e_USB_HID_VAL_DATA 	= 0x00,	//变量
	e_USB_HID_VAL_CONST	= 0x01,	//常量
	e_USB_HID_VAL_ARRAY	= 0x00,	//数组
	e_USB_HID_VAL_VAR	= 0x02,	//变量
	e_USB_HID_VAL_ABS	= 0x00,	//绝对值
	e_USB_HID_VAL_REL	= 0x04,	//相对值
}E_USB_HID_REPORT_VAL;

#define USB_HID_REPORT_COLLECTION(v)	0xA1,v,
typedef enum _USB_HID_REPORT_COL{
	e_USB_HID_COL_PHYSICAL		= 0x00,	//物理
	e_USB_HID_COL_APPLICATION	= 0x01,	//应用
	e_USB_HID_COL_LOGICAL		= 0x02,	//逻辑
}E_USB_HID_REPORT_COL;

#define USB_HID_REPORT_ENDCOLLECTION()	0xC0,

#define USB_HID_REPORT_USAGE_PAGE(v)	0x05,v,
typedef enum _USB_HID_REPORT_UP{
	e_USB_HID_UP_GENERIC_DESKTOP	= 0x01,	//桌面
	e_USB_HID_UP_KEYBOARD			= 0x07,	//键盘
	e_USB_HID_UP_LEDS				= 0x08, //灯
	e_USB_HID_UP_BUTTON				= 0x09, //按钮
}E_USB_HID_REPORT_UP;

#define USB_HID_REPORT_LOGICAL_MIN(v)	0x15,(u8)v,
#define USB_HID_REPORT_LOGICAL_MAX(v)	0x25,(u8)v,
#define USB_HID_REPORT_LOGICAL_MIN_16(v)	0x16,(u8)v,(u8)(v >> 8),
#define USB_HID_REPORT_LOGICAL_MAX_16(v)	0x26,(u8)v,(u8)(v >> 8),
#define USB_HID_REPORT_LOGICAL_MIN_32(v)	0x17,(u8)v,(u8)(v >> 8),(u8)(v >> 16),(u8)(v >> 24),
#define USB_HID_REPORT_LOGICAL_MAX_32(v)	0x27,(u8)v,(u8)(v >> 8),(u8)(v >> 16),(u8)(v >> 24),
#define USB_HID_REPORT_REPORT_SIZE(v)	0x75,v,
#define USB_HID_REPORT_REPORT_CNT(v)	0x95,v,

#define USB_HID_REPORT_USAGE(v)			0x09,v,
typedef enum _USB_HID_REPORT_USAGE{
	e_USB_HID_USAGE_POINTER			= 0x01,	//指针
	e_USB_HID_USAGE_MOUSE			= 0x02, //鼠标
	e_USB_HID_USAGE_KEYBOARD		= 0x06, //键盘
	e_USB_HID_USAGE_X				= 0x30, //X轴
	e_USB_HID_USAGE_Y				= 0x31, //Y轴
	e_USB_HID_USAGE_WHEEL			= 0x38,	//滚轮
}E_USB_HID_REPORT_USAGE;

#define USB_HID_REPORT_USAGE_MIN(v)		0x19,v,
#define USB_HID_REPORT_USAGE_MAX(v)		0x29,v,


extern const USB_HID_PROPERTY Usb_hid_property;
extern const u8 Usb_hid_report_descriptor[];
extern const u32 Usb_hid_report_len;

#endif
