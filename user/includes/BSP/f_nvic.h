#ifndef __F_NVIC_H__
#define __F_NVIC_H__

#include "stm32f10x.h"                  // Device header

//中断优先级 0-15 小值优先级高，可同级
#define USART1_IRQPriority		2
#define ADC1_2_IRQPriority		1




void f_nvic_config(uint8_t NVIC_IRQChannel,u8 IRQPriority);


#endif
