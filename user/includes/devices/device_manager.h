#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

#include "stm32f10x.h"

typedef struct _DEVICE{
	u8 name[10];
	u8 (*init)(void);
	u8 (*read)(u32 addr, void *data, u32 len);
	u8 (*write)(u32 addr, void *data, u32 len);
}DEVICE;





















#endif
