#include "f_gpio.h"


void f_gpio_init(GPIO_TypeDef* port,u16 pins,GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
					 
	GPIO_InitStructure.GPIO_Pin = pins;	
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_Init(port, &GPIO_InitStructure);
}

void f_gpio_ouput(GPIO_TypeDef* port,u16 value)
{
    port -> ODR = value;
}

void f_gpio_set(GPIO_TypeDef* port,u16 pins)
{
	port -> BSRR = pins;
}

void f_gpio_reset(GPIO_TypeDef* port,u16 pins)
{
	port -> BRR = pins;
}

void f_gpio_toggle(GPIO_TypeDef* port,u16 pins)
{
	port -> ODR ^= pins;
}

u16 f_gpio_read(GPIO_TypeDef* port,u16 pins)
{
	return ((port -> IDR)&(pins));
}

u16 f_gpio_readoutput(GPIO_TypeDef* port,u16 pins)
{
	return ((port -> ODR)&(pins));
}

