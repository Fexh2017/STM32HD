#ifndef __F_ADC_H__
#define __F_ADC_H__

#include "stm32f10x.h"                  // Device header

void f_adc_init(void);
u16 f_adc_getvalue(u8 adc_id,u8 channel);

#endif

