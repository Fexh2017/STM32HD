#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "stm32f10x.h"

typedef struct _MOUSE_DEV{
	u8 (*init)(void);
	
}MOUSE_DEV;

#endif
