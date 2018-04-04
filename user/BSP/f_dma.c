#include "f_dma.h"
#include "f_rcc.h"

/*DMA通道分配表
----------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
		DMA1	|	CH1		|	CH2		|	CH3		|	CH4		|	CH5		|	CH6		|	CH7		|
----------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
ADC		ADC1	|ADC1		|			|			|			|			|			|			|
----------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
SPI		SPI1	|			|SPI1_RX	|SPI1_TX	|			|			|			|			|
		SPI2	|			|			|			|SPI2_RX	|SPI2_TX	|			|			|
----------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
USART	USART1	|			|			|			|USART1_TX	|USART1_RX	|			|			|
		USART2	|			|			|			|			|			|USART2_TX	|USART2_RX	|
		USART3	|			|USART3_TX	|USART3_RX	|			|			|			|			|
----------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
I2C		I2C1	|			|			|			|			|			|I2C1_TX	|	I2C1_RX	|
		I2C2	|			|			|			|I2C2_TX	|	I2C2_RX	|			|			|
----------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+
TIM		TIM1	|			|TIM1_CH1	|TIM1_CH2	|TIM1_CH4	|TIM1_UP	|TIM1_CH3	|			|
				|			|			|			|TIM1_TRIG	|			|			|			|
				|			|			|			|TIM1_COM	|			|			|			|
		TIM2	|TIM2_CH3	|TIM2_UP	|			|			|TIM2_CH1	|			|TIM2_CH2	|
				|			|			|			|			|			|			|TIM2_CH4	|
		TIM3	|			|TIM3_CH3	|TIM3_CH4	|			|TIM3_CH1	|			|			|
				|			|			|TIM3_UP	|			|TIM3_TRIG	|			|			|
		TIM4	|TIM4_CH1	|			|			|TIM4_CH2	|TIM4_CH3	|			|TIM4_UP	|
----------------+-----------+-----------+-----------+-----------+-----------+-----------+-----------+

----------------+-----------+-----------+-----------+-----------+-----------+
		DMA2	|	CH1		|	CH2		|	CH3		|	CH4		|	CH5		|
----------------+-----------+-----------+-----------+-----------+-----------+
ADC		ADC3	|   		|			|			|			|ADC3		|
----------------+-----------+-----------+-----------+-----------+-----------+
SPI/I2S	SPI/I2S3|SPI/I2S3_RX|SPI/I2S3_TX|       	|			|			|
----------------+-----------+-----------+-----------+-----------+-----------+
UART	UART4	|			|			|UART4_RX	|			|UART4_TX	|
----------------+-----------+-----------+-----------+-----------+-----------+
SDIO	SDIO	|			|			|			|SDIO		|			|
----------------+-----------+-----------+-----------+-----------+-----------+
DAC		DAC		|			|			|DAC_CH1	|DAC_CH2	|			|
----------------+-----------+-----------+-----------+-----------+-----------+
TIM		TIM5	|TIM5_CH4	|TIM5_CH3	|			|TIM5_CH2	|TIM5_CH1	|
				|TIM5_TRIG	|TIM5_UP	|			|			|			|
		TIM6	|			|			|TIM6_UP	|			|			|
		TIM7	|			|			|			|TIM7_UP	|			|
		TIM8	|TIM8_CH3	|TIM8_CH4	|TIM8_CH1	|			|TIM8_CH2	|
				|TIM8_UP	|TIM8_TRIG	|			|			|			|
				|			|TIM8_COM	|			|			|			|
----------------+-----------+-----------+-----------+-----------+-----------+
*/

void f_dma_init(u32 peripheral, u32 memory, u32 len, DMA_Channel_TypeDef* channel, u32 dir, u32 mode)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	switch((u32)channel)
	{
		case (u32)DMA1_Channel1:
		case (u32)DMA1_Channel2:
		case (u32)DMA1_Channel3:
		case (u32)DMA1_Channel4:
		case (u32)DMA1_Channel5:
		case (u32)DMA1_Channel6:
		case (u32)DMA1_Channel7:
			f_rcc_enable(e_RCC_DMA1);
			break;
		case (u32)DMA2_Channel1:
		case (u32)DMA2_Channel2:
		case (u32)DMA2_Channel3:
		case (u32)DMA2_Channel4:
		case (u32)DMA2_Channel5:
			f_rcc_enable(e_RCC_DMA2);
			break;
	}
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)peripheral;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)memory;
	DMA_InitStructure.DMA_DIR = dir;
	DMA_InitStructure.DMA_BufferSize = len;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = mode;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(channel, &DMA_InitStructure);  
}


