#ifndef __PWM_H__
#define __PWM_H__

#include "stm32f10x.h"
#include "config.h"

#define PWM_TIM 		TIM3
#define PWM_PIN_REMAP	GPIO_PartialRemap_TIM3
#define PWM_CH0 		GPIO_PB_04
#define PWM_CH1 		GPIO_PB_05
#define PWM_CH2 		GPIO_PB_00
#define PWM_CH3 		0	//GPIO_PB_01

#define PWM_PRESCALER	6	// ·ÖÆµ   72M/(x+1)
#define PWM_PERIOD		999	// ÖØ×°   (x+1)

void f_pwm_init(void);
u16 f_pwm_get(u16 ch);
void f_pwm_set(u16 ch, u16 value);

#endif
