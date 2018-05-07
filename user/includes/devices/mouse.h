#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "stm32f10x.h"
#include "device_manager.h"

#define MOUSE_DEBUG_EN   			CONFIG_DEBUG_LV_M 	//MOUSE_DEBUGÊ¹ÄÜ



typedef enum _MOUSE_STATE{
	E_MOUSE_IDLE = 0x00,
	E_MOUSE_INIT = 0x01,
	E_MOUSE_ERR  = 0x80,
}E_MOUSE_STATE;

#pragma pack(1)
typedef struct _MOUSE_DATA{
	u8 keys;
	s16 x;
	s16 y;
	s16 wheel;
//	s8 consumer;
}MOUSE_DATA;
#pragma pack()






extern const DEVICE Mouse;

#endif
