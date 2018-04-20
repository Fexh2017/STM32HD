#include "config.h"
#include "public.h"

#include "p_debug.h"
#include "p_led.h"
#include "p_usb.h"

#include "usb_property.h"

#include "mouse.h"

void sys_init(void)
{
	systick_init();
	
	p_debug_init();
	SYS_LOG("system init");
	p_led_init(e_LED_ALL);
	
	Mouse.init();
	SYS_LOG("USB init");
	
	p_usb_init();
}

void sys_mainloop(void)
{
	MOUSE_DATA mouse_data;
	
	if(Mouse.read(&mouse_data) == 1)
	{
		DEV_DBG("%02x %d %d %d",mouse_data.keys,mouse_data.x,mouse_data.y,mouse_data.wheel);
		Usb_hid_property.send_event((u8*)&mouse_data, sizeof(mouse_data));
		//delayms(2);
	}
}

int main(void)
{	
    sys_init();

    while(1)
    {
        sys_mainloop();
    }
}

