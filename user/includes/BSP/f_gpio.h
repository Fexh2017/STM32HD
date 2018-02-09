#ifndef __F_GPIO_H__
#define __F_GPIO_H__

#include "stm32f10x.h"                  // Device header

void f_gpio_init(GPIO_TypeDef* port,u16 pins,GPIOMode_TypeDef mode);
void f_gpio_ouput(GPIO_TypeDef* port,u16 value);
void f_gpio_set(GPIO_TypeDef* port,u16 pins);
void f_gpio_reset(GPIO_TypeDef* port,u16 pins);
void f_gpio_toggle(GPIO_TypeDef* port,u16 pins);
u16 f_gpio_read(GPIO_TypeDef* port,u16 pins);
u16 f_gpio_readoutput(GPIO_TypeDef* port,u16 pins);

#endif

