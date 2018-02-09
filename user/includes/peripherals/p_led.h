#ifndef __P_LED_H__
#define __P_LED_H__

#include "stm32f10x.h"                  // Device header

enum e_LED{
	e_LED_ALL = 0,
	e_LED_0 	 ,
	e_LED_1		 ,
};

void p_led_init(u8 id);
void p_led_on(u8 id);
void p_led_off(u8 id);
void p_led_toggle(u8 id);

#endif
