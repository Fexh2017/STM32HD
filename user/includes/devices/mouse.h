#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "stm32f10x.h"
#include "device_manager.h"

#define MOUSE_DEBUG_EN   			CONFIG_DEBUG_LV_M 	//MOUSE_DEBUGÊ¹ÄÜ

#define MOUSE_MACRO_ITEM_SIZE		8
#define MOUSE_MACRO_PAGE_COUNT		32		//256B
#define MOUSE_MACRO_ITEM_COUNT		16384	//128KB

#define MOUSE_MACRO_COUNT			7
#define MOUSE_MACRO_ADDR_RECORD		0x10000
#define MOUSE_MACRO_ADDR_1			0x30000
#define MOUSE_MACRO_ADDR_2			0x50000
#define MOUSE_MACRO_ADDR_3			0x70000
#define MOUSE_MACRO_ADDR_4			0x90000
#define MOUSE_MACRO_ADDR_5			0xB0000
#define MOUSE_MACRO_ADDR_6			0xD0000

typedef enum _MOUSE_STATE{
	e_MOUSE_IDLE = 0x00,
	e_MOUSE_NORMAL,
	e_MOUSE_RECORD,
	e_MOUSE_REPLAY,
	e_MOUSE_MACRO_EDIT,
	e_MOUSE_MACRO,
	e_MOUSE_ERR  = 0xFF,
}E_MOUSE_STATE;

typedef enum _MOUSE_FUN_STATE{
	e_MOUSE_FUN_STATE_EMPTY = 0,
	e_MOUSE_FUN_STATE_CPI,			//6d 6u
	e_MOUSE_FUN_STATE_RECORD,		//6d 3d 3u 6u
	e_MOUSE_FUN_STATE_REPLAY,		//6d 4d 4u 6u
	e_MOUSE_FUN_STATE_MACRO_EDIT,	//6d 4d 4u 4d 4u 6u
	e_MOUSE_FUN_STATE_MACRO,		//6d 3d 3u 3d 3u 6u
	e_MOUSE_FUN_STATE_DET = 0xFE,
	e_MOUSE_FUN_STATE_ERR = 0xFF,
}E_MOUSE_FUN_STATE;

#pragma pack(1)
typedef struct _MOUSE_DATA{
	u8 keys;
	s16 x;
	s16 y;
	s16 wheel;
}MOUSE_DATA;
#pragma pack()






extern const DEVICE Mouse;

#endif
