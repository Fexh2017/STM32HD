#include "config.h"
#include "public.h"

#include "debug.h"
#include "usb.h"
#include "mouse.h"

void sys_init(void)
{
	systick_init();
	
	Debug.init();
	SYS_LOG("system init");
	
	Mouse.init();
	Usb.init();
}

void sys_mainloop(void)
{
	MOUSE_DATA mouse_data;
	
	if(Mouse.read(0,&mouse_data) == 1)
	{
		Usb.write(1, &mouse_data);
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

