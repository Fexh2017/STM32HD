#include "config.h"
#include "public.h"

#include "debug.h"
#include "usb.h"
#include "usb_property.h"
#include "mouse.h"

void sys_init(void)
{
	system_init();
	
	Debug.init();
	SYS_LOG("system init");
	
	Mouse.init();
	Usb.init();
}

void sys_mainloop(void)
{
	MOUSE_DATA mouse_data = {
		.keys = 0,
		.x = 0,
		.y = 0,
		.wheel = 0,
	};
	
	if(Mouse.read(0,&mouse_data, USB_EP1_MAX_PACKET_SIZE) == 1)
	{
		Usb.write(1, &mouse_data, USB_EP1_MAX_PACKET_SIZE);
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

