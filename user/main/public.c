#include "public.h"
#include "f_rcc.h"

#define SYSTICK_CNT 	(SystemCoreClock / 1000000 * SYSTICK_US)
#define SYSTICK_HOUR 	(3600000000 / SYSTICK_US)

volatile static u32 Systick_H = 0;	//小时
volatile static u32 Systick_L = 0;	//SYSTICK_CNT us

typedef struct _SYS_TIMER{
	u32 id;
	void (*fun)(void* arg);
	void *arg;
	u32 ms;
	u32 start_time;
}SYS_TIMER;

static SYS_TIMER sys_timer[SYSTIMER_COUNT];
void sys_timer_init(void)
{
	for(int i = 0; i < SYSTIMER_COUNT; i++)
	{
		sys_timer[i].id = 0;
	}
}

u32 sys_timer_add(void (*fun)(void* arg), void* arg, u32 ms)
{
	if(fun && ms)
	{
		for(int i = 0; i < SYSTIMER_COUNT; i++)
		{
			if(sys_timer[i].id == 0)
			{
				sys_timer[i].id  = i + 1;
				sys_timer[i].fun = fun;
				sys_timer[i].arg = arg;
				sys_timer[i].ms  = ms;
				sys_timer[i].start_time = sys_get_ms();
				return sys_timer[i].id;
			}
		}
	}
	SYS_ERR("SYS TIMER ADD FAILED");
	return (u32)-1;
}

u32 sys_timer_del(u32 id)
{
	if(id)
	{
		for(int i = 0; i < SYSTIMER_COUNT; i++)
		{
			if(sys_timer[i].id == id)
			{
				sys_timer[i].id = 0;
				return 0;
			}
		}
	}
	SYS_ERR("SYS TIMER DEL FAILED");
	return (u32)-1;
}

void sys_timer_loop(void)
{
	u8 overflow = 0;
	u32 sys_time = sys_get_ms();
	for(int i = 0; i < SYSTIMER_COUNT; i++)
	{
		if(sys_timer[i].id != 0)
		{
			overflow = 0;
			if(sys_time < sys_timer[i].start_time)
			{
				overflow = 1;
			}
			if((sys_time + 3600000 * overflow - sys_timer[i].start_time) >= sys_timer[i].ms)
			{
				sys_timer[i].start_time = sys_time;
				if(sys_timer[i].fun)
				{
					sys_timer[i].fun(sys_timer[i].arg);
				}
			}
		}
	}
}

void SysTick_Handler(void)
{
	Systick_L++;
	if(Systick_L >= SYSTICK_HOUR){
		Systick_L = 0;
		Systick_H ++;
	}
}

u32 sys_get_tick(void)
{
	return SysTick->VAL;
}

u32 sys_get_ms(void)
{
#if (SYSTICK_US > 1000)
	return ((Systick_L * SYSTICK_US / 1000) + (sys_get_tick() * 1000 / SystemCoreClock));
#else
	return (Systick_L * SYSTICK_US / 1000);
#endif
}

u32 sys_get_hour(void)
{
	return Systick_H;
}

//延时毫秒
void delayms(u32 tim)
{
	u8 overflow = 0;
	u32 sys_tim = (sys_get_ms() * 10) + (sys_get_tick() * 10000 / SystemCoreClock);
	u32 delay_over = sys_tim + tim * 10;
	u32 sys_hour = sys_get_hour();
	
	if(tim > 1800000)
	{
		SYS_ERR("DELAY OVERFLOW");
		return;
	}
	
	if(delay_over >= 36000000)
	{
		overflow = 1;
	}
		
	while((delay_over > sys_tim) || (sys_get_hour() != sys_hour + overflow))
	{
		sys_tim = (sys_get_ms() * 10) + (sys_get_tick() * 10000 / SystemCoreClock);
	}
}

//软件延时
void delay(u16 tim)
{
	if(tim)
	{
		while(tim--);
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








void system_init(void)
{
	//关闭jtag
	f_rcc_enable(e_RCC_AFIO);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	//系统时钟
	SysTick_Config(SYSTICK_CNT); 
	//系统定时器
	sys_timer_init();
}
