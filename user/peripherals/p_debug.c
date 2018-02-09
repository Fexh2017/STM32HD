#include "config.h"

#if CONFIG_DEBUG_UART_EN

#include "f_usart.h"
#include "p_debug.h"

void p_debug_init(void)
{
	f_usart_init(DEBUG_UART, DEBUG_UART_BAUDRATE);
}

int fputc(int ch, FILE *f)
{
	f_usart_send(DEBUG_UART , (uint8_t) ch);
	return (ch);
}

#else

void p_debug_init(void){}
int fputc(int ch, FILE *f){	return (ch);}

#endif

