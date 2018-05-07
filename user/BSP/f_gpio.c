#include "f_gpio.h"
#include "f_rcc.h"

#include "stdio.h"
void f_gpio_init(u32 pins,GPIOMode_TypeDef mode)
{
	switch((u32)GPIO_PORT(pins))
	{
		case (u32)GPIOA:
			f_rcc_enable(e_RCC_GPIOA);
			break;
		case (u32)GPIOB:
			f_rcc_enable(e_RCC_GPIOB);
			break;
		case (u32)GPIOC:
			f_rcc_enable(e_RCC_GPIOC);
			break;
		case (u32)GPIOD:
			f_rcc_enable(e_RCC_GPIOD);
			break;
		case (u32)GPIOE:
			f_rcc_enable(e_RCC_GPIOE);
			break;
		case (u32)GPIOF:
			f_rcc_enable(e_RCC_GPIOF);
			break;
		case (u32)GPIOG:
			f_rcc_enable(e_RCC_GPIOG);
			break;
	}
	
	GPIO_InitTypeDef GPIO_InitStructure;
					 
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN(pins);	
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_Init(GPIO_PORT(pins), &GPIO_InitStructure);
}

void f_gpio_ouput(GPIO_TypeDef* port,u16 value)
{
    port -> ODR = value;
}

void f_gpio_set(u32 pins)
{
	GPIO_PORT(pins) -> BSRR = GPIO_PIN(pins);
}

void f_gpio_reset(u32 pins)
{
	GPIO_PORT(pins) -> BRR = GPIO_PIN(pins);
}

void f_gpio_toggle(u32 pins)
{
	GPIO_PORT(pins) -> ODR ^= GPIO_PIN(pins);
}

u16 f_gpio_read(u32 pins)
{
	return ((GPIO_PORT(pins) -> IDR)&(GPIO_PIN(pins)));
}

u16 f_gpio_readoutput(u32 pins)
{
	return ((GPIO_PORT(pins) -> ODR)&(GPIO_PIN(pins)));
}

