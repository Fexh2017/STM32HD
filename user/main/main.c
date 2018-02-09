#include "config.h"
#include "public.h"

#include "p_debug.h"
#include "p_led.h"

void sys_init(void)
{
	p_debug_init();
	SYS_LOG("system init");
	p_led_init(e_LED_0);
}

void sys_mainloop(void)
{
	p_led_toggle(e_LED_0);
	delayms(500);
}

int main(void)
{	
    sys_init();

    while(1)
    {
        sys_mainloop();
    }
}

