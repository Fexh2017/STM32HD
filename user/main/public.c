#include "public.h"

#define SYSTICK_CNT 	(SystemCoreClock / 1000000 * SYSTICK_US)
#define SYSTICK_HOUR 	(3600000000 / SYSTICK_US)

volatile static u32 Systick_H = 0;	//小时
volatile static u32 Systick_L = 0;	//SYSTICK_CNT us
volatile static u32 Systick_Delay = 0; //延时

void systick_init(void)
{
	SysTick_Config(SYSTICK_CNT); //100us
}

void SysTick_Handler(void)
{
	Systick_L++;
	if(Systick_L >= SYSTICK_HOUR){
		Systick_L = 0;
		Systick_H ++;
	}
	if(Systick_Delay)
	{
		Systick_Delay --;
	}
}

u32 Get_Systick(void)
{
	return Systick_L;
}

u32 Get_Syshour(void)
{
	return Systick_H;
}

//延时毫秒
void delayms(u32 tim)
{
	Systick_Delay = tim * 1000 / SYSTICK_US;
	while(Systick_Delay);
}

//软件延时
void delay(u16 tim)
{
	u32 i;
	if(tim)
	{
		while(tim--)
		{
			i=720;
			while(i--);
		}
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

//把两个u8合成一个u16
u16 ToWord(u8 bytehigh, u8 bytelow)
{
	u16 wRet;
	wRet = (u16)bytelow | ((u16)bytehigh << 8);
	return wRet;
}

//把u16的两个字节交换位置
u16 ByteSwap(u16 wSwW)
{
	u8 bTemp;
	u16 wRet;
	bTemp = (u8)(wSwW & 0xff);
	wRet =  (wSwW >> 8) | ((u16)bTemp << 8);
	return wRet;
}

