#include "f_rcc.h"


void f_rcc_enable(enum RCC_APB_E rcc)
{
	switch(rcc)
	{
		case e_RCC_AFIO:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
			break;
		case e_RCC_GPIOA:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
			break;
		case e_RCC_GPIOB:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
			break;
		case e_RCC_GPIOC:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
			break;
		case e_RCC_GPIOD:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
			break;
		case e_RCC_GPIOE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
			break;
		case e_RCC_GPIOF:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
			break;
		case e_RCC_GPIOG:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
			break;
		case e_RCC_ADC1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
			break;
		case e_RCC_ADC2:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
			break;
		case e_RCC_TIM1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
			break;
		case e_RCC_SPI1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
			break;
		case e_RCC_TIM8:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
			break;
		case e_RCC_USART1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
			break;
		case e_RCC_ADC3:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
			break;
		case e_RCC_TIM15:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,ENABLE);
			break;
		case e_RCC_TIM16:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,ENABLE);
			break;
		case e_RCC_TIM17:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17,ENABLE);
			break;
		case e_RCC_TIM9:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
			break;
		case e_RCC_TIM10:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
			break;
		case e_RCC_TIM11:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);
			break;
		case e_RCC_TIM2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
			break;
		case e_RCC_TIM3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
			break;
		case e_RCC_TIM4:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
			break;
		case e_RCC_TIM5:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
			break;
		case e_RCC_TIM6:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
			break;
		case e_RCC_TIM7:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
			break;
		case e_RCC_TIM12:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
			break;
		case e_RCC_TIM13:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);
			break;
		case e_RCC_TIM14:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
			break;
		case e_RCC_WWDG:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
			break;
		case e_RCC_SPI2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
			break;
		case e_RCC_SPI3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
			break;
		case e_RCC_USART2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
			break;
		case e_RCC_USART3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
			break;
		case e_RCC_UART4:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
			break;
		case e_RCC_UART5:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
			break;
		case e_RCC_I2C1:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
			break;
		case e_RCC_I2C2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
			break;
		case e_RCC_USB:
			RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB,ENABLE);
			break;
		case e_RCC_CAN1:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
			break;
		case e_RCC_CAN2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,ENABLE);
			break;
		case e_RCC_BKP:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
			break;
		case e_RCC_PWR:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
			break;
		case e_RCC_DAC:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
			break;
		case e_RCC_CEC:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_CEC,ENABLE);
			break;
		case e_RCC_DMA1:
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
			break;
		case e_RCC_DMA2:
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
			break;
		case e_RCC_SRAM:
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM,ENABLE);
			break;
		case e_RCC_FLITF:
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FLITF,ENABLE);
			break;
		case e_RCC_CRC:
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
			break;
		default:
			break;
	}
}



void f_rcc_disable(enum RCC_APB_E rcc)
{
	switch(rcc)
	{
		case e_RCC_AFIO:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,DISABLE);
			break;
		case e_RCC_GPIOA:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,DISABLE);
			break;
		case e_RCC_GPIOB:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,DISABLE);
			break;
		case e_RCC_GPIOC:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,DISABLE);
			break;
		case e_RCC_GPIOD:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,DISABLE);
			break;
		case e_RCC_GPIOE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,DISABLE);
			break;
		case e_RCC_GPIOF:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,DISABLE);
			break;
		case e_RCC_GPIOG:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,DISABLE);
			break;
		case e_RCC_ADC1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,DISABLE);
			break;
		case e_RCC_ADC2:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,DISABLE);
			break;
		case e_RCC_TIM1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,DISABLE);
			break;
		case e_RCC_SPI1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,DISABLE);
			break;
		case e_RCC_TIM8:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,DISABLE);
			break;
		case e_RCC_USART1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,DISABLE);
			break;
		case e_RCC_ADC3:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,DISABLE);
			break;
		case e_RCC_TIM15:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,DISABLE);
			break;
		case e_RCC_TIM16:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,DISABLE);
			break;
		case e_RCC_TIM17:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17,DISABLE);
			break;
		case e_RCC_TIM9:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,DISABLE);
			break;
		case e_RCC_TIM10:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,DISABLE);
			break;
		case e_RCC_TIM11:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,DISABLE);
			break;
		case e_RCC_TIM2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE);
			break;
		case e_RCC_TIM3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,DISABLE);
			break;
		case e_RCC_TIM4:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,DISABLE);
			break;
		case e_RCC_TIM5:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,DISABLE);
			break;
		case e_RCC_TIM6:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,DISABLE);
			break;
		case e_RCC_TIM7:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,DISABLE);
			break;
		case e_RCC_TIM12:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,DISABLE);
			break;
		case e_RCC_TIM13:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,DISABLE);
			break;
		case e_RCC_TIM14:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,DISABLE);
			break;
		case e_RCC_WWDG:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,DISABLE);
			break;
		case e_RCC_SPI2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,DISABLE);
			break;
		case e_RCC_SPI3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,DISABLE);
			break;
		case e_RCC_USART2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,DISABLE);
			break;
		case e_RCC_USART3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,DISABLE);
			break;
		case e_RCC_UART4:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,DISABLE);
			break;
		case e_RCC_UART5:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,DISABLE);
			break;
		case e_RCC_I2C1:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,DISABLE);
			break;
		case e_RCC_I2C2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,DISABLE);
			break;
		case e_RCC_USB:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB,DISABLE);
			break;
		case e_RCC_CAN1:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,DISABLE);
			break;
		case e_RCC_CAN2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,DISABLE);
			break;
		case e_RCC_BKP:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,DISABLE);
			break;
		case e_RCC_PWR:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,DISABLE);
			break;
		case e_RCC_DAC:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,DISABLE);
			break;
		case e_RCC_CEC:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_CEC,DISABLE);
			break;
		case e_RCC_DMA1:
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,DISABLE);
			break;
		case e_RCC_DMA2:
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,DISABLE);
			break;
		case e_RCC_SRAM:
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM,DISABLE);
			break;
		case e_RCC_FLITF:
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FLITF,DISABLE);
			break;
		case e_RCC_CRC:
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,DISABLE);
			break;
		default:
			break;
	}
}
