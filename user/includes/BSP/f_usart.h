#ifndef __F_USART_H__
#define __F_USART_H__

#include "stm32f10x.h"                  // Device header


void f_usart_init(u8 id, u32 baudrate);
void f_usart_send(u8 id, u8 ch);
void f_usart_send_buf(u8 id, u8* buf, u32 len);

#endif

