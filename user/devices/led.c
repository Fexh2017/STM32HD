#include "config.h"
#include "led.h"

#if CONFIG_LED_EN
#include "f_pwm.h"

#if (LED_DEBUG_EN >= CONFIG_DEBUG_LV_H)
#define DEV_DBG(...)					DEBUG("[DEV DBG] ",__VA_ARGS__)
#define DEV_DBGN(...)					DEBUGN("",__VA_ARGS__)
#else
#define DEV_DBG(...) 
#define DEV_DBGN(...) 
#endif
#if (LED_DEBUG_EN >= CONFIG_DEBUG_LV_M)
#define DEV_LOG(...)					DEBUG("[DEV]     ",__VA_ARGS__)
#else
#define DEV_LOG(...)  
#endif
#if (LED_DEBUG_EN >= CONFIG_DEBUG_LV_L)
#define DEV_ERR(...)					DEBUG("[DEV_ERR] ",__VA_ARGS__)
#else
#define DEV_ERR(...)  
#endif

static u32 led_funs = e_LED_FUN_CONSUMER;
static u32 led_timer_id = 0;

void led_rainbow(void *p)
{
	static LED_VALUE value = {LED_ON,LED_OFF,LED_OFF};
	if((value.R != 0) && (value.B == 0))
	{
		value.R--;
		value.G++;
	}else if((value.G != 0) && (value.R == 0))
	{
		value.G--;
		value.B++;
	}else if((value.B != 0) && (value.G == 0))
	{
		value.B--;
		value.R++;
	}else
	{
		DEV_LOG("led fun rainbow reset");
		value.R = LED_ON;
		value.G = LED_OFF;
		value.B = LED_OFF;
	}
	f_pwm_set(0, value.R);
	f_pwm_set(1, value.G);
	f_pwm_set(2, value.B);
	led_funs = e_LED_FUN_RAINBOW;
	
}

u8 led_init(void)
{
	DEV_LOG("led init");
	f_pwm_init();
	return 0;
}

u8 led_read(u32 addr, void *data, u32 arg)
{
	LED_VALUE* v = (LED_VALUE*)data;
	if(addr == e_LED_ALL)
	{
		v->R = f_pwm_get(0);
		v->G = f_pwm_get(1);
		v->B = f_pwm_get(2);
	}else if(addr == e_LED_R)
	{
		return f_pwm_get(0);
	}else if(addr == e_LED_R)
	{
		return f_pwm_get(1);
	}else if(addr == e_LED_R)
	{
		return f_pwm_get(2);
	}else if(addr == e_LED_FUN)
	{
		return led_funs;
	}else{
		DEV_ERR("READ LED ERR %d",addr);
		return 0xFF;
	}
	return 0;
}

u8 led_write(u32 addr, void *data, u32 arg)
{
	LED_VALUE* v = (LED_VALUE*)data;
	u16 value = arg;
	
	if(addr == e_LED_ALL)
	{
		if(led_timer_id)
		{
			sys_timer_del(led_timer_id);
			led_timer_id = 0;
		}
		f_pwm_set(0, v->R);
		f_pwm_set(1, v->G);
		f_pwm_set(2, v->B);
		led_funs = e_LED_FUN_CONSUMER;
	}
	else if(addr == e_LED_R)
	{
		if(led_timer_id)
		{
			sys_timer_del(led_timer_id);
			led_timer_id = 0;
		}
		f_pwm_set(0, value);
		led_funs = e_LED_FUN_CONSUMER;
	}
	else if(addr == e_LED_G)
	{
		if(led_timer_id)
		{
			sys_timer_del(led_timer_id);
			led_timer_id = 0;
		}
		f_pwm_set(1, value);
		led_funs = e_LED_FUN_CONSUMER;
	}
	else if(addr == e_LED_B)
	{
		if(led_timer_id)
		{
			sys_timer_del(led_timer_id);
			led_timer_id = 0;
		}
		f_pwm_set(2, value);
		led_funs = e_LED_FUN_CONSUMER;
	}else if(addr == e_LED_FUN)
	{
		switch(value)
		{
			case e_LED_FUN_RED:
				if(led_timer_id)
				{
					sys_timer_del(led_timer_id);
					led_timer_id = 0;
				}
				f_pwm_set(0, LED_ON);
				f_pwm_set(1, LED_OFF);
				f_pwm_set(2, LED_OFF);
				led_funs = e_LED_FUN_RED;
				break;
			case e_LED_FUN_GREEN:
				if(led_timer_id)
				{
					sys_timer_del(led_timer_id);
					led_timer_id = 0;
				}
				f_pwm_set(0, LED_OFF);
				f_pwm_set(1, LED_ON);
				f_pwm_set(2, LED_OFF);
				led_funs = e_LED_FUN_GREEN;
				break;
			case e_LED_FUN_BLUE:
				if(led_timer_id)
				{
					sys_timer_del(led_timer_id);
					led_timer_id = 0;
				}
				f_pwm_set(0, LED_OFF);
				f_pwm_set(1, LED_OFF);
				f_pwm_set(2, LED_ON);
				led_funs = e_LED_FUN_BLUE;
				break;
			case e_LED_FUN_RAINBOW:
				if(led_timer_id)
				{
					sys_timer_del(led_timer_id);
					led_timer_id = 0;
				}
				led_timer_id = sys_timer_add(led_rainbow, NULL, 2);
				break;
			default:
				DEV_ERR("LED FUNS ERR %d",value);
				break;
		}
	}else
	{
		DEV_ERR("WRITE LED ERR %d",addr);
		return 0xFF;
	}
	return 0;
}

#else
u8 led_init(void){return 0xFF;}
u8 led_read(u32 addr, void *data, u32 arg){return 0xFF;}
u8 led_write(u32 addr, void *data, u32 arg){return 0xFF;}
#endif

const DEVICE Led = 
{
	.name = "led",
	.init = led_init,
	.read = led_read,
	.write = led_write,
};
