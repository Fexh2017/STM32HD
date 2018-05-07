#include "f_usart.h"
#include "f_rcc.h"
#include "f_gpio.h"
#include "f_nvic.h"


void f_usart_init(u8 id, u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	
	switch(id)
	{
		case 1:
			f_rcc_enable(e_RCC_USART1);
			f_rcc_enable(e_RCC_AFIO);
		
			f_gpio_init(GPIO_PA_09,GPIO_Mode_AF_PP);
			f_gpio_init(GPIO_PA_10,GPIO_Mode_IN_FLOATING);
		
			USART_InitStructure.USART_BaudRate = baudrate;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No ;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
			USART_Init(USART1, &USART_InitStructure);
			
			USART_Cmd(USART1, ENABLE);
		
			break;
	}
	
}

void f_usart_send(u8 id,u8 ch)
{
	switch(id)
	{
		case 1:
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
			USART_SendData(USART1,ch);
			break;
	}
}

void f_usart_send_buf(u8 id,u8* buf,u32 len)
{
	u32 i;
	for(i=0; i<len ; i++)
	{
		f_usart_send(id,buf[i]);
	}
}





