#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include "stm32f10x.h"                  // Device header
#include "config.h"

#define SYSTICK_US 		1000 //(1~233000) us
#define SYSTIMER_COUNT	5

//函数声明
extern void system_init(void);											//系统初始化
extern u32 sys_timer_add(void (*fun)(void* arg), void* arg, u32 ms);	//新增系统定时器事件
extern u32 sys_timer_del(u32 id);										//删除系统定时器事件
extern void sys_timer_loop(void);										//系统定时器
extern u32 sys_get_tick(void);											//系统计时（72M）
extern u32 sys_get_ms(void);											//系统计时（毫秒）
extern u32 sys_get_hour(void);											//系统计时（小时）
extern u32 sys_get_time_after(u32 ms);									//系统计时（过去多少毫秒）
extern void delayms(u32 tim);											//延时-毫秒
extern void delay(u16 tim);												//延时-自定义
extern s32 limiter(s32 num,s32 min,s32 max);							//限幅-整形
extern double flimiter(double num,double min,double max);				//限幅-浮点
extern u16 ToWord(u8 bytehigh, u8 bytelow);
extern u16 ByteSwap(u16 wSwW);
#endif
