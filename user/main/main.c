#include "config.h"
#include "public.h"

#include "p_debug.h"
#include "p_led.h"
#include "p_usb.h"

#include "p_spi.h"

#include "usb_property.h"

#include "f_gpio.h"
#include "f_rcc.h"

void sys_init(void)
{
	systick_init();
	
	p_debug_init();
	SYS_LOG("system init");
	p_led_init(e_LED_ALL);
	
	extern void mouse_init(void);
	mouse_init();
	
	f_rcc_enable(e_RCC_GPIOC);
	f_gpio_init(GPIOC,GPIO_Pin_1,GPIO_Mode_IPU);
	f_gpio_init(GPIOC,GPIO_Pin_13,GPIO_Mode_IPU);
	
	SYS_LOG("USB init");
	
	p_usb_init();
}

void sys_mainloop(void)
{
	s16 x,y;
	static u8 last_key = 0;
	//u8 send_buf[4] = {0,0,0,0}; //key, x, y, 0
	struct {
		u8 keys;
		s8 x;
		s8 y;
		s8 wheel;
	}send_buf = {0,0,0,0};
	
	if(Usb_status.State != E_USB_STATE_CONFIGURED)
	{
		delayms(2);
		return;
	}

	//key[0]:0,����ɿ�;1,�������;
	//key[1]:0,�Ҽ��ɿ�;1,�Ҽ�����
	//key[2]:0,�м��ɿ�;1,�м�����   
	extern void mouse_state(s16 *x, s16 *y);
	mouse_state(&x, &y);
	
	send_buf.x = (s8)x;
	send_buf.y = (s8)-y;
	
	if(f_gpio_read(GPIOC,GPIO_Pin_1) == 0)
	{
		p_led_on(e_LED_0);
		send_buf.keys |= 0x01;
//		send_buf[0] |= 0x01;
	}else
	{
		p_led_off(e_LED_0);
	}
	if(f_gpio_read(GPIOC,GPIO_Pin_13) == 0)
	{
		p_led_on(e_LED_1);
		send_buf.keys |= 0x02;
//		send_buf[0] |= 0x02;
	}else
	{
		p_led_off(e_LED_1);
	}
	
	if(last_key != send_buf.keys || send_buf.x != 0 || send_buf.y != 0 || last_key == 0)
//	if(last_key != send_buf[0] || send_buf[1] != 0 || send_buf[2] != 0)
	{
		last_key = send_buf.keys;
		Usb_hid_property.send_event((u8*)&send_buf, sizeof(send_buf));
//		last_key = send_buf[0];
//		Usb_hid_property.send_event(send_buf, 4);
	}
	
	//delayms(2);
	
}

int main(void)
{	
    sys_init();

    while(1)
    {
        sys_mainloop();
    }
}

