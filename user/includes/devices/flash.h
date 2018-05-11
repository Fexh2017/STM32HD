#ifndef __FLASH_H__
#define __FLASH_H__

#include "stm32f10x.h"                  // Device header
#include "device_manager.h"


#define FLASH_DEBUG_EN 			CONFIG_DEBUG_LV_M		//LED_DEBUGÊ¹ÄÜ

typedef struct _FLASH_INFO{
	u16 id;
	u16 status;
	u32 size;
	u32 page_size;
	u32 sector_size;
	u32 block_size;
	u32 block_count;
}FLASH_INFO;


extern const DEVICE Flash;

#endif
