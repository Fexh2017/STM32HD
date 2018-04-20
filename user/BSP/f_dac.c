#include "f_dac.h"
#include "f_rcc.h"
#include "f_gpio.h"


void f_dac_init(u8 channel)
{
	DAC_InitTypeDef  DAC_InitStructure;
	
	f_rcc_enable(e_RCC_GPIOA);
	f_rcc_enable(e_RCC_DAC);
	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1,&DAC_InitStructure);
	
	if(channel == 1)
	{
		f_gpio_init(GPIO_PA_04,GPIO_Mode_AIN);
		DAC_Cmd(DAC_Channel_1,ENABLE);
	}
	else if(channel == 2)
	{
		f_gpio_init(GPIO_PA_05,GPIO_Mode_AIN);
		DAC_Cmd(DAC_Channel_2,ENABLE);
	}
}

void f_dac_setvalue(u8 channel,u16 value)
{
	if(channel == 1)
	{
		DAC_SetChannel1Data(DAC_Align_12b_R,value);
		DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
	}
	else if(channel == 2)
	{
		DAC_SetChannel2Data(DAC_Align_12b_R,value);
		DAC_SoftwareTriggerCmd(DAC_Channel_2,ENABLE);
	}
}



