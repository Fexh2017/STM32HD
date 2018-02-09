#include "public.h"

//延时微秒
void delayus(u32 tim)
{
	SysTick->LOAD=9*tim;
	SysTick->VAL=0x00;
	SysTick->CTRL=0x01;
	while(!((SysTick->CTRL)&(1<<16)));
	SysTick->CTRL=0;
	SysTick->VAL=0x00;
}

//延时毫秒
void delayms(u32 tim)
{
	SysTick->LOAD=9000*tim;
	SysTick->VAL=0x00;
	SysTick->CTRL=0x01;
	while(!((SysTick->CTRL)&(1<<16)));
	SysTick->CTRL=0x00;
	SysTick->VAL=0x00;
}

//软件延时
void delay(u16 tim)
{
	u32 i;
	while(tim--)
	{
		i=7200;
		while(i--);
	}
}

//限幅
s32 limiter(s32 num,s32 min,s32 max)
{
	if(num<min)num=min;
	if(num>max)num=max;
	return num;
}

//限幅（浮点）
double limiterf(double num,double min,double max)
{
	if(num<min)num=min;
	if(num>max)num=max;
	return num;
}
