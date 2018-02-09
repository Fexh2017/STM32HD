#include "config.h"
#include "public.h"

#include "p_debug.h"
#include "p_led.h"

#include "usb_lib.h"

void sys_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	p_debug_init();
	SYS_LOG("system init");
	p_led_init(e_LED_0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_15);
	delayms(1000);
	delayms(1000);
	delayms(1000);
	GPIO_SetBits(GPIOC,GPIO_Pin_15);
	
	SYS_LOG("USB init");
	USB_Interrupts_Config();
	Set_USBClock();
	USB_Init();
}

void sys_mainloop(void)
{
	u8 send_buf[4]; //key, x, y, 0
	//key[0]:0,左键松开;1,左键按下;
	//key[1]:0,右键松开;1,右键按下
	//key[2]:0,中键松开;1,中键按下   
	send_buf[0] = !send_buf[0];
	send_buf[1] = 0;
	send_buf[2] = 0;
	send_buf[3] = 0;
	Joystick_Send(send_buf[0],send_buf[1],send_buf[2],0);
	
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

