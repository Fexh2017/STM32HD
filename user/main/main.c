#include "config.h"
#include "public.h"

#include "debug.h"


void sys_init(void)
{
	system_init();
	
	Debug.init();
	SYS_LOG("system init");
	
}

void sys_mainloop(void)
{
	SYS_DBG("hello world");
	delayms(1000);
}

int main(void)
{	
    sys_init();

    while(1)
    {
        sys_mainloop();
    }
}

