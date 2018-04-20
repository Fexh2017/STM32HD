#include "f_adc.h"
#include "f_rcc.h"
#include "f_gpio.h"
#include "f_nvic.h"

u16 ADC1_value[16];

void f_adc_init(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	//打开时钟
	f_rcc_enable(e_RCC_ADC1);
	f_rcc_enable(e_RCC_GPIOA);
	
	//配置GPIO
	f_gpio_init(GPIO_PA_00, GPIO_Mode_AIN);
	
	//配置中断优先级
	f_nvic_config(ADC1_2_IRQn,ADC1_2_IRQPriority);
	
	//配置ADC
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent ;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE ;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right ;
	ADC_InitStructure.ADC_NbrOfChannel = 1 ;	
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);  	// 72 M / 8 = 9 M
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);	//( 1s / 9 M ) * 239.5 = 0.0266 ms
	
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	
	ADC_Cmd(ADC1, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	// ADC开始校准
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	//软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
}


void ADC1_2_IRQHandler(void)
{
	static u8 cnt = 0;
  // 读取ADC的转换值
  ADC1_value[cnt++] = ADC_GetConversionValue(ADC1);
	if(cnt >= 16)
	{
		cnt = 0;
	}
}

u16 f_adc_getvalue(u8 adc_id,u8 channel)
{
	u8 i;
	u32 tmp = 0;
	for(i = 0; i < 16; i++)
	{
		tmp += ADC1_value[i];
	}
	tmp /= 16;
	return tmp;
}

