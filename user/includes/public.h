#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include "stm32f10x.h"                  // Device header
#include "config.h"

#define SYSTICK_US 		100 //us

//函数声明
extern void system_init(void);									//系统初始化
extern u32 Get_Systick(void);									//系统计时
extern u32 Get_Syshour(void);									//系统计时（小时）
extern void delayms(u32 tim);									//延时-毫秒
extern void delay(u16 tim);										//延时-自定义
extern s32 limiter(s32 num,s32 min,s32 max);					//限幅-整形
extern double flimiter(double num,double min,double max);		//限幅-浮点
extern u16 ToWord(u8 bytehigh, u8 bytelow);
extern u16 ByteSwap(u16 wSwW);
#endif
