#include "config.h"

#if CONFIG_LED_EN
#include "p_led.h"

#include "f_gpio.h"
#include "f_rcc.h"


void p_led_init(u8 id)
{
	if(id == e_LED_0 || id == e_LED_ALL)
	{
		f_rcc_enable(LED_RCC_0);
		f_gpio_init(LED_PORT_0, LED_PIN_0, GPIO_Mode_Out_PP);
	}
	if(id == e_LED_1 || id == e_LED_ALL)
	{
		f_rcc_enable(LED_RCC_1);
		f_gpio_init(LED_PORT_1, LED_PIN_1, GPIO_Mode_Out_PP);
	}
}

void p_led_on(u8 id)
{
	if(id == e_LED_0 || id == e_LED_ALL)
	{
		f_gpio_reset(LED_PORT_0,LED_PIN_0);
	}
	if(id == e_LED_1 || id == e_LED_ALL)
	{
		f_gpio_reset(LED_PORT_1,LED_PIN_1);
	}
}

void p_led_off(u8 id)
{
	if(id == e_LED_0 || id == e_LED_ALL)
	{
		f_gpio_set(LED_PORT_0,LED_PIN_0);
	}
	if(id == e_LED_1 || id == e_LED_ALL)
	{
		f_gpio_set(LED_PORT_1,LED_PIN_1);
	}
}

void p_led_toggle(u8 id)
{
	if(id == e_LED_0 || id == e_LED_ALL)
	{
		f_gpio_toggle(LED_PORT_0,LED_PIN_0);
	}
	if(id == e_LED_1 || id == e_LED_ALL)
	{
		f_gpio_toggle(LED_PORT_1,LED_PIN_1);
	}
}

#else
void p_led_init(u8 id){}
void p_led_on(u8 id){}
void p_led_off(u8 id){}
void p_led_toggle(u8 id){}
#endif

