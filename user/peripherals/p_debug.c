#include "config.h"

#if CONFIG_DEBUG_UART_EN

#include "f_usart.h"
#include "f_dma.h"
#include "f_nvic.h"
#include "p_debug.h"

#if CONFIG_DEBUG_DMA

#define DEBUG_MAX_LEN 	5120
#define DEBUG_IDLE 		0x00
#define DEBUG_SEND		0x01
#define DEBUG_CYCLE		0x02

u8 debug_buf[DEBUG_MAX_LEN];

u8  debug_status    = DEBUG_IDLE;
u32 debug_cache_cnt = 0;
u32 debug_send_cnt  = 0;
u32 debug_send_len  = 0;
u8* debug_send_buf  = debug_buf;

void _debug_send(void)
{
	debug_status |= DEBUG_SEND;
	if(debug_status & DEBUG_CYCLE){
		debug_send_buf = &debug_buf[debug_send_cnt];
		debug_send_len = DEBUG_MAX_LEN - debug_send_cnt;
		debug_send_cnt = 0;
		debug_status &= ~DEBUG_CYCLE;
	}else{
		debug_send_buf = &debug_buf[debug_send_cnt];
		debug_send_len = debug_cache_cnt - debug_send_cnt;
		debug_send_cnt = debug_cache_cnt;
	}
	if(debug_send_len == 0)
	{
		debug_status &= ~DEBUG_SEND;
		return;
	}
	
	f_dma_init((u32)&USART1->DR, (u32)debug_send_buf, debug_send_len, DMA1_Channel4, DMA_DIR_PeripheralDST, DMA_Mode_Normal);
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4))
	{
		debug_status &= ~DEBUG_SEND;
		DMA_Cmd(DMA1_Channel4, DISABLE);
		_debug_send();
		DMA_ClearITPendingBit(DMA1_FLAG_TC4);
	}
}
#endif

void p_debug_init(void)
{
	f_usart_init(DEBUG_UART, DEBUG_UART_BAUDRATE);
	
#if CONFIG_DEBUG_DMA
	debug_status = DEBUG_IDLE;
	debug_cache_cnt = 0;
	debug_send_cnt = 0;
	debug_send_buf = debug_buf;
	debug_send_len = 0;
	
	f_dma_init((u32)&USART1->DR, (u32)debug_send_buf, debug_send_len, DMA1_Channel4, DMA_DIR_PeripheralDST, DMA_Mode_Normal);
	f_nvic_config(DMA1_Channel4_IRQn, CONFIG_NVIC_DEBUG_DMA);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_ClearITPendingBit(DMA1_FLAG_TC4);
#endif
}

int fputc(int ch, FILE *f)
{
#if CONFIG_DEBUG_DMA
	debug_buf[debug_cache_cnt] = ch;
	debug_cache_cnt ++;
	if(debug_cache_cnt >= DEBUG_MAX_LEN)
	{
		debug_cache_cnt = 0;
		debug_status |= DEBUG_CYCLE;
	}
	if((debug_status & DEBUG_SEND) == 0)
	{
		_debug_send();
	}
#else
	f_usart_send(DEBUG_UART , (uint8_t) ch);
#endif
	return (ch);
}

























#else

void p_debug_init(void){}

#endif

