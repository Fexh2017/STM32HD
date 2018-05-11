#include "f_pwm.h"

#include "f_gpio.h"
#include "f_rcc.h"
#include "f_timer.h"

#include "public.h"

void f_pwm_init(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	f_rcc_enable(e_RCC_AFIO);
	f_gpio_init(PWM_CH0, GPIO_Mode_AF_PP);
	f_gpio_init(PWM_CH1, GPIO_Mode_AF_PP);
	f_gpio_init(PWM_CH2, GPIO_Mode_AF_PP);
	f_gpio_init(PWM_CH3, GPIO_Mode_AF_PP);
	if(PWM_PIN_REMAP)GPIO_PinRemapConfig(PWM_PIN_REMAP, ENABLE);
	
	f_timer_init(PWM_TIM, PWM_PERIOD, PWM_PRESCALER);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	if(PWM_CH0)
	{
		TIM_OC1Init(PWM_TIM, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(PWM_TIM, ENABLE);
	}
	if(PWM_CH1)
	{
		TIM_OC2Init(PWM_TIM, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(PWM_TIM, ENABLE);
	}
	if(PWM_CH2)
	{
		TIM_OC3Init(PWM_TIM, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(PWM_TIM, ENABLE);
	}
	if(PWM_CH3)
	{
		TIM_OC4Init(PWM_TIM, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(PWM_TIM, ENABLE);
	}
	
	f_timer_enable(PWM_TIM);
	
}

u16 f_pwm_get(u16 ch)
{
	switch(ch)
	{
		case 0:
			return PWM_TIM->CCR1;
		case 1:
			return PWM_TIM->CCR2;
		case 2:
			return PWM_TIM->CCR3;
		case 3:
			return PWM_TIM->CCR4;
		default:
			return 0;
	}
}

void f_pwm_set(u16 ch, u16 value)
{
	if(PWM_CH0 && ch== 0)PWM_TIM->CCR1 = value;
	else if(PWM_CH1 && ch== 1)PWM_TIM->CCR2 = value;
	else if(PWM_CH2 && ch== 2)PWM_TIM->CCR3 = value;
	else if(PWM_CH3 && ch== 3)PWM_TIM->CCR4 = value;
}



