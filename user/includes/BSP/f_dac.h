#ifndef __F_DAC_H__
#define __F_DAC_H__

#include "stm32f10x.h"                  // Device header

void f_dac_init(u8 channel);
void f_dac_setvalue(u8 channel,u16 value);


#endif
