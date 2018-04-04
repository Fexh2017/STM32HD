#ifndef __F_RCC_H__
#define __F_RCC_H__

#include "stm32f10x.h"                  // Device header

typedef enum {
  e_RCC_AFIO =1,
  e_RCC_GPIOA  ,
  e_RCC_GPIOB  ,
  e_RCC_GPIOC  ,
  e_RCC_GPIOD  ,
  e_RCC_GPIOE  ,
  e_RCC_GPIOF  ,
  e_RCC_GPIOG  ,
  e_RCC_ADC1   ,
  e_RCC_ADC2   ,
  e_RCC_TIM1   ,
  e_RCC_SPI1   ,
  e_RCC_TIM8   ,
  e_RCC_USART1 ,
  e_RCC_ADC3   ,
  e_RCC_TIM15  ,
  e_RCC_TIM16  ,
  e_RCC_TIM17  ,
  e_RCC_TIM9   ,
  e_RCC_TIM10  ,
  e_RCC_TIM11  ,
  e_RCC_TIM2   ,
  e_RCC_TIM3   ,
  e_RCC_TIM4   ,
  e_RCC_TIM5   ,
  e_RCC_TIM6   ,
  e_RCC_TIM7   ,
  e_RCC_TIM12  ,
  e_RCC_TIM13  ,
  e_RCC_TIM14  ,
  e_RCC_WWDG   ,
  e_RCC_SPI2   ,
  e_RCC_SPI3   ,
  e_RCC_USART2 ,
  e_RCC_USART3 ,
  e_RCC_UART4  ,
  e_RCC_UART5  ,
  e_RCC_I2C1   ,
  e_RCC_I2C2   ,
  e_RCC_USB    ,
  e_RCC_CAN1   ,
  e_RCC_CAN2   ,
  e_RCC_BKP    ,
  e_RCC_PWR    ,
  e_RCC_DAC    ,
  e_RCC_CEC    ,
  e_RCC_DMA1   ,
  e_RCC_DMA2   ,
  e_RCC_SRAM   ,
  e_RCC_FLITF  ,
  e_RCC_CRC    
}E_RCC_APB;


void f_rcc_enable(E_RCC_APB rcc);
void f_rcc_disable(E_RCC_APB rcc);


#endif
