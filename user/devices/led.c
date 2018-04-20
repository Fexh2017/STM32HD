#include "config.h"
#include "led.h"

#if CONFIG_LED_EN
#include "f_gpio.h"
#include "f_rcc.h"

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


u8 led_init(void)
{
	DEV_LOG("led init");
	return 0;
}

u8 led_read(u32 addr, void *data)
{
	return 0;
}

u8 led_write(u32 addr, void *data)
{
	return 0;
}

#else
u8 led_init(void){return 0xFF;}
u8 led_read(u32 addr, void *data){return 0xFF;}
u8 led_write(u32 addr, void *data){return 0xFF;}
#endif

const DEVICE Led = 
{
	.name = "led",
	.init = led_init,
	.read = led_read,
	.write = led_write,
};
