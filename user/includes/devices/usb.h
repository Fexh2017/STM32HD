#ifndef __USB_H__
#define __USB_H__

#include "stm32f10x.h"                  // Device header
#include "device_manager.h"

#define CONFIG_USB_DEBUG_EN				CONFIG_DEBUG_LV_M			//USB_DEBUG使能
#define CONFIG_USB_PWR_EN				1								//USB 使能脚

#if CONFIG_USB_PWR_EN
//USB 使能脚配置
#define CONFIG_USB_PWR_CLK				e_RCC_GPIOC
#define CONFIG_USB_PWR_PIN				GPIO_PC_15
#endif //CONFIG_USB_PWR_EN

typedef enum{
	E_USB_STATE_UNCONNECT 	= 0x00,		//未连接
	E_USB_STATE_ATTACHED	= 0x01,		//已连接
	E_USB_STATE_POWERED		= 0x02,		//上电
	E_USB_STATE_DEFAULT		= 0x04,		//默认
	E_USB_STATE_ADDRESS		= 0x08,		//地址
	E_USB_STATE_CONFIGURED	= 0x10,		//配置
	E_USB_STATE_SUSPENDED	= 0x80,		//挂起
}E_USB_State;

typedef enum{
	E_USB_REQ_GET_STATUS        = 0,
	E_USB_REQ_CLEAR_FEATURE     = 1,
	E_USB_REQ_SET_FEATURE       = 3,
	E_USB_REQ_SET_ADDRESS       = 5,
	E_USB_REQ_GET_DESCRIPTOR    = 6,
	E_USB_REQ_SET_DESCRIPTOR    = 7,
	E_USB_REQ_GET_CONFIGURATION = 8,
	E_USB_REQ_SET_CONFIGURATION = 9,
	E_USB_REQ_GET_INTERFACE     = 10,
	E_USB_REQ_SET_INTERFACE     = 11,
	E_USB_REQ_SYNCH_FRAME       = 12,
}E_USB_Requset;

typedef enum{
	E_USB_REQ_GET_REPORT		= 1,
	E_USB_REQ_GET_IDLE			= 2,
	E_USB_REQ_GET_PROTOCOL		= 3,
	E_USB_REQ_SET_REPORT		= 9,
	E_USB_REQ_SET_IDLE			= 10,
	E_USB_REQ_SET_PROTOCOL		= 11,
}E_USB_HID_Request;

typedef enum{
	E_USB_GD_DEVICE				= 0x0100,
	E_USB_GD_CONFIGURATION		= 0x0200,
	E_USB_GD_STRING				= 0x0300,
	E_USB_GD_INTERFACE			= 0x0400,
	E_USB_GD_ENDPOINT			= 0x0500,
	E_USB_GD_DEVICE_QUALIFIER	= 0x0600,
	E_USB_GD_OTHER_SPEED_CONFIGURATION	= 0x0700,
	E_USB_GD_INTERFACE_POWER	= 0x0800,
	E_USB_GD_HID_REPORT			= 0x2200,
}E_USB_Get_Descriptor;

typedef enum{
	E_USB_DATA_DIR_IDLE = 0,
	E_USB_DATA_DIR_RX,
	E_USB_DATA_DIR_TX,
}E_USB_DATA_DIR;

typedef enum{
	E_USB_STRING_LANGUAGE = 0,
	E_USB_STRING_MANUFACTURER,
	E_USB_STRING_PRODUCT,
	E_USB_STRING_SERIALNUMBER,
}E_USB_STRING;

typedef struct _USB_REQUEST{
	u8  bmType;
	u8  bRequest;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
}USB_REQUEST;

typedef struct _USB_DATA{
	u8  dir;		//方向
	u8* buf;		//数据
	u32 len;		//总长度
	u32 offset;		//本次发送的偏移
}USB_DATA;

typedef struct _USB_INFO{
	u8 	State;			//USB状态
	u16 DAddr;			//设备地址
	u16 Dev_state;		//设备状态 D0.自供电 D1.远程唤醒
	u16 Config_val;		//配置识别码
	u16 Interface_val;	//接口识别码
	u16 Ep_enable;		//端点使能
	u16	Ep_halt;		//端点停用
	u32 Max_packet[8]; 	//最大包长度
}USB_STATUS;

extern USB_STATUS 	Usb_status;
extern USB_DATA		Usb_data[8];
extern USB_REQUEST	Usb_request;

extern void usb_setup0(void);
extern void usb_in0(void);
extern void usb_out0(void);

extern const DEVICE Usb;

#endif

