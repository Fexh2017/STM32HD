#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include "stm32f10x.h"                  // Device header
#include "config.h"

//函数声明
extern void delayus(u32 tim);																//延时-微秒
extern void delayms(u32 tim);																//延时-毫秒
extern void delay(u16 tim);																	//延时-自定义
extern s32 limiter(s32 num,s32 min,s32 max);												//限幅-整形
extern double flimiter(double num,double min,double max);									//限幅-浮点


#endif
