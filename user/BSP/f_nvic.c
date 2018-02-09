#include "f_nvic.h"

void f_nvic_config(uint8_t NVIC_IRQChannel,u8 IRQPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = IRQPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




