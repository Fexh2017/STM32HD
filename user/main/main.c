#include "config.h"
#include "public.h"

#include "debug.h"
#include "usb.h"
#include "usb_property.h"
#include "mouse.h"
#include "led.h"



void sys_led_rainbow(void *p)
{
	Led.write(e_LED_FUN, NULL, e_LED_FUN_RAINBOW);
}





void sys_init(void)
{
	system_init();
	
	Debug.init();
	SYS_LOG("system init");
	Led.init();
	Led.write(e_LED_R,(void*)LED_ON,0);
	Mouse.init();
	Usb.init();
	
	sys_timer_add(sys_led_rainbow, NULL, 2);
}

void sys_mainloop(void)
{
	MOUSE_DATA mouse_data;
	
	if(Mouse.read(0,&mouse_data, USB_EP1_MAX_PACKET_SIZE) == 1)
	{
		Usb.write(1, &mouse_data, USB_EP1_MAX_PACKET_SIZE);
	}
}

int main(void)
{	
    sys_init();

    while(1)
    {
		sys_timer_loop();
        sys_mainloop();
    }
}

