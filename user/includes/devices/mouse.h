#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "stm32f10x.h"

typedef enum _MOUSE_STATE{
	E_MOUSE_IDLE = 0x00,
	E_MOUSE_INIT = 0x01,
	E_MOUSE_ERR  = 0x80,
}E_MOUSE_STATE;



typedef struct _MOUSE_DATA{
	u8 keys;
	s8 x;
	s8 y;
	s8 wheel;
}MOUSE_DATA;

typedef struct _MOUSE_DEV{
	u8 (*init)(void);
	u8 (*read)(MOUSE_DATA *data);
	u8 (*config)(u8 cfg);
}MOUSE_DEV;












extern const MOUSE_DEV Mouse;

#endif
