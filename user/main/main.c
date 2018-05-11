#include "config.h"
#include "public.h"

#include "debug.h"
#include "usb.h"
#include "usb_property.h"
#include "mouse.h"
#include "led.h"
#include "flash.h"


void sys_timer_mouse_read(void *p)
{
	MOUSE_DATA mouse_data;
	
	if(Mouse.read(0,&mouse_data, USB_EP1_MAX_PACKET_SIZE) == 1)
	{
		Usb.write(1, &mouse_data, USB_EP1_MAX_PACKET_SIZE);
	}
}



void sys_init(void)
{
	system_init();
	
	Debug.init();
	SYS_LOG("system init");
	Led.init();
	Led.write(e_LED_FUN, NULL, e_LED_FUN_RED);
	Flash.init();
	Mouse.init();
	Usb.init();
	
//	Flash.write(0 , 0, 0);
//	delayms(2000);
	
	Led.write(e_LED_FUN, NULL, e_LED_FUN_RAINBOW);
	sys_timer_add(sys_timer_mouse_read, NULL, 1);
}

void sys_mainloop(void)
{
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

