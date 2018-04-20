#include "f_timer.h"
#include "f_rcc.h"

static void (*timer_irq[8])(void) = {0,0,0,0,0,0,0,0};

void f_timer_init(TIM_TypeDef* tim,u16 TIM_Period,u16 TIM_Prescaler)
{
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	switch((u32)tim)
	{
		case (u32)TIM1:
			f_rcc_enable(e_RCC_TIM1);
			break;
		case (u32)TIM2:
			f_rcc_enable(e_RCC_TIM2);
			break;
		case (u32)TIM3:
			f_rcc_enable(e_RCC_TIM3);
			break;
		case (u32)TIM4:
			f_rcc_enable(e_RCC_TIM4);
			break;
		case (u32)TIM5:
			f_rcc_enable(e_RCC_TIM5);
			break;
		case (u32)TIM6:
			f_rcc_enable(e_RCC_TIM6);
			break;
		case (u32)TIM7:
			f_rcc_enable(e_RCC_TIM7);
			break;
		case (u32)TIM8:
			f_rcc_enable(e_RCC_TIM8);
			break;
		default:
			break;
	}
	
	
    TIM_TimeBaseStructure.TIM_Period = TIM_Period;       			//定时周期
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler;       		//预分频，不分频 72M / (0+1) = 72M
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    		//时钟分频系数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(tim, &TIM_TimeBaseStructure);
	
}

void f_timer_enable(TIM_TypeDef* tim)
{
	TIM_Cmd(tim, ENABLE);
}

void f_timer_disable(TIM_TypeDef* tim)
{
	TIM_Cmd(tim, DISABLE);
}

void f_timer_it_config(TIM_TypeDef* tim, int IRQPriority, void (*fun)(void))
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 

	switch((u32)tim)
	{
		case (u32)TIM1:
			NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
			timer_irq[0] = fun;
			break;
		case (u32)TIM2:
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	 
			timer_irq[1] = fun;
			break;
		case (u32)TIM3:
			NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
			timer_irq[2] = fun;
			break;
		case (u32)TIM4:
			NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  
			timer_irq[3] = fun;
			break;
		case (u32)TIM5:
			NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;	  
			timer_irq[4] = fun;
			break;
		case (u32)TIM6:
			NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;	  
			timer_irq[5] = fun;
			break;
		case (u32)TIM7:
			NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;	  
			timer_irq[6] = fun;
			break;
		case (u32)TIM8:
			NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;	  
			timer_irq[7] = fun;
			break;
		default:
			break;
	}
	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = IRQPriority;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(tim, TIM_FLAG_Update);
	TIM_ITConfig(tim,TIM_IT_Update,ENABLE);
}


void f_timer_dma_config(TIM_TypeDef* tim)
{
	TIM_DMACmd(tim,TIM_DMA_Update,ENABLE);
}


/*TIM Interrupt Handler*/
void TIM1_UP_IRQHandler()
{
	(*timer_irq[0])();
	TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
}

void TIM2_IRQHandler()
{
	(*timer_irq[1])();
	TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
}

void TIM3_IRQHandler()
{
	(*timer_irq[0])();
	TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
}

void TIM4_IRQHandler()
{
	(*timer_irq[3])();
	TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
}

void TIM5_IRQHandler()
{
	(*timer_irq[4])();
	TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);
}

void TIM6_IRQHandler()
{
	(*timer_irq[5])();
	TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);
}

void TIM7_IRQHandler()
{
	(*timer_irq[6])();
	TIM_ClearITPendingBit(TIM7 , TIM_FLAG_Update);
}

void TIM8_UP_IRQHandler()
{
	(*timer_irq[7])();
	TIM_ClearITPendingBit(TIM8 , TIM_FLAG_Update);
}
