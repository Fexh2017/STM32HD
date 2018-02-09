#ifndef __F_TIMER_H__
#define __F_TIMER_H__

#include "stm32f10x.h"                  // Device header


void f_timer_init(TIM_TypeDef* tim,u16 TIM_Period,u16 TIM_Prescaler);
void f_timer_enable(TIM_TypeDef* tim);
void f_timer_disable(TIM_TypeDef* tim);
void f_timer_it_config(TIM_TypeDef* tim, int IRQPriority, void (*fun)(void));
void f_timer_dma_config(TIM_TypeDef* tim);

#endif

