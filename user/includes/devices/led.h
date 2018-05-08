#ifndef __P_LED_H__
#define __P_LED_H__

#include "stm32f10x.h"                  // Device header
#include "device_manager.h"


#define LED_DEBUG_EN 			CONFIG_DEBUG_LV_M		//LED_DEBUGÊ¹ÄÜ

#define LED_ON	999
#define LED_OFF	0

typedef struct _LED_VALUE{
	u16 R;
	u16 G;
	u16 B;
}LED_VALUE;

typedef enum _LED_SEL{
	e_LED_R = 1,
	e_LED_G = 2,
	e_LED_B = 4,
	e_LED_ALL = 0xFF,
	e_LED_FUN = 0x1FFFFFFE,
}e_LED_SEL;

typedef enum _LED_FUNS{
	e_LED_FUN_CONSUMER = 0,
	e_LED_FUN_RAINBOW,
}e_LED_FUNS;

extern const DEVICE Led;

#endif
