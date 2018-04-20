#include "config.h"

#include "mouse.h"
#include "public.h"

#include "f_rcc.h"
#include "f_gpio.h"
#include "f_timer.h"

#include "p_spi.h"

/*  ����ģ��  */
#define MOUSE_NAVI_CEN		GPIO_PA_03
#define MOUSE_NAVI_MOTION	GPIO_PA_04
u8 mouse_navi_init_buf[] = 
{
	0x1A, 0xC0,
	0x1F, 0x00,
	0x1C, 0x80,
	0x1D, 0x7F,
//	0x12, 0x26,	//800 cpi, 8ms run_rate
//	0x13, 0x04,	//downshift_time = x * 8 * run_rate
//	0x36, 0x04,	//800 cpi
	
};

void mouse_navi_writereg(u8 addr, u8 data)
{
	Spi_dev.cs(MOUSE_NAVI_CEN, 1);
	delay(10);
	Spi_dev.send(addr | 0x80, data);
	delay(10);
	Spi_dev.cs(MOUSE_NAVI_CEN, 0);
}

u8 mouse_navi_readreg(u8 addr)
{
	u8 data;
	Spi_dev.cs(MOUSE_NAVI_CEN, 1);
	delay(10);
	data = Spi_dev.read(addr);
	delay(10);
	Spi_dev.cs(MOUSE_NAVI_CEN, 0);
	delay(10);
	return data;
}

u32 mouse_navi_readregbuf(u8 addr, u8* buf, u32 len)
{
	int i;
	Spi_dev.cs(MOUSE_NAVI_CEN, 1);
	delay(10);
	Spi_dev.rw(addr);
	delay(10);
	for(i = 0; i <len; i++)
	{
		buf[i] = Spi_dev.rw(0);
		//buf[i] = mouse_read(addr + i);
	}
	delay(10);
	Spi_dev.cs(MOUSE_NAVI_CEN, 0);
	return i;
}

u8 mouse_navi_check(void)
{
	u8 id = 0xff;
	u8 rev = 0xff;
	id = mouse_navi_readreg(0x00);
	rev = mouse_navi_readreg(0x01);
	if(id == 0x31 && rev == 0x03)
	{
		DEV_LOG("mouse id %02x %02x", id, rev);
		return 0;
	}
	DEV_ERR("mouse id err %02x %02x", id, rev);
	return 0xff;
}

u8 mouse_navi_init(void)
{
	u8 buf[4];
	Spi_dev.init();
	f_gpio_init(MOUSE_NAVI_CEN, GPIO_Mode_Out_PP);
	f_gpio_init(MOUSE_NAVI_MOTION, GPIO_Mode_IPU);
	Spi_dev.cs(MOUSE_NAVI_CEN, 0);
	delayms(100);
	Spi_dev.cs(MOUSE_NAVI_CEN, 1);
	delayms(100);
	Spi_dev.cs(MOUSE_NAVI_CEN, 0);
	delayms(100);
	Spi_dev.cs(MOUSE_NAVI_CEN, 1);
	delayms(100);
	
	if(mouse_navi_check() == 0)
	{
		DEV_LOG("mouse init");
		mouse_navi_writereg(0x3A, 0x5A);
		delayms(10);
		
		mouse_navi_readregbuf(0x42, buf, 4);
//		
		mouse_navi_writereg(0x3C, 0x27);
		mouse_navi_writereg(0x22, 0x0A);
		mouse_navi_writereg(0x21, 0x01);
		mouse_navi_writereg(0x3C, 0x32);
		mouse_navi_writereg(0x23, 0x20);
		mouse_navi_writereg(0x3C, 0x05);
		mouse_navi_writereg(0x37, 0xB9);
		delayms(10);
		
		//self test
//		mouse_write(0x10,0x01);
//		delayms(500);
//		for(int i = 0x0c; i <= 0x0f; i++)
//		{
//			DEV_DBG("%02X %02X", i, mouse_read(i));
//		}

		for(int i = 0; i < sizeof(mouse_navi_init_buf); i += 2)
		{
			mouse_navi_writereg(mouse_navi_init_buf[i], mouse_navi_init_buf[i + 1]);
		}
		
		mouse_navi_readregbuf(0x42, buf, 4);
		
		mouse_navi_writereg(0x2E, 0x00);
		delayms(1);
		
		return 0;
	}
	return 0xff;
}

void mouse_navi_read(s16 *x, s16 *y)
{
	u8 buf[4];
	if(!f_gpio_read(MOUSE_NAVI_MOTION))
	{
		mouse_navi_readregbuf(0x42, buf, 4);
		*x = (buf[3] & 0xF0) << 4 | buf[1];
		*y = (buf[3] & 0x0F) << 8 | buf[2];
		if(*x > 2048)
		{
			*x |= 0xF000;
		}
		if(*y > 2048)
		{
			*y |= 0xF000;
		}
		//DEV_DBG("%02x %4d %4d",buf[0], *x, *y);
	}else
	{
		*x = 0;
		*y = 0;
	}
}



/*   ����   */
void mouse_wheel_init(void)
{
	TIM_ICInitTypeDef Tim_ICInitStructure;
	
	f_rcc_enable(e_RCC_GPIOB);
	f_rcc_enable(e_RCC_TIM4);
	f_gpio_init(GPIO_PB_06 | GPIO_PB_07, GPIO_Mode_IPU);
	
	f_timer_init(TIM4, 0xFF, 0);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
	Tim_ICInitStructure.TIM_Channel = TIM_Channel_1;
	Tim_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	Tim_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	Tim_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	Tim_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &Tim_ICInitStructure);
	
	//TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM4->CNT = 0;
	f_timer_enable(TIM4);
}

s8 mouse_wheel_read(void)
{
	static s8 last_wheel = 0;
	u8 tmp = TIM4->CNT;
	s8 value = (s8)tmp - last_wheel;
	last_wheel = tmp;
	
	//DEV_DBG("%5d  %d", tmp, value);
	return value;
}




/*   ����   */
#define MOUSE_KEYS_1 GPIO_PC_01
#define MOUSE_KEYS_2 GPIO_PC_13
#define MOUSE_KEYS_3 GPIO_PC_01
#define MOUSE_KEYS_4 GPIO_PC_01
#define MOUSE_KEYS_5 GPIO_PC_01
#define MOUSE_KEYS_6 GPIO_PC_01

void mouse_keys_init(void)
{
	f_rcc_enable(e_RCC_GPIOC);
	f_gpio_init(MOUSE_KEYS_1, GPIO_Mode_IPU);
	f_gpio_init(MOUSE_KEYS_2, GPIO_Mode_IPU);
}

u8 mouse_keys_read(void)
{
	u8 keys = 0;
	if(f_gpio_read(MOUSE_KEYS_1) == 0)
	{
		keys |= 0x01;
	}
	if(f_gpio_read(MOUSE_KEYS_2) == 0)
	{
		keys |= 0x02;
	}
	return keys;
}












u8 mouse_state = E_MOUSE_IDLE;

u8 mouse_init(void)
{
	u8 err = 0;
	err = mouse_navi_init();
	if(err)
	{
		mouse_state = E_MOUSE_ERR;
		return err;
	}
	
	mouse_wheel_init();
	mouse_keys_init();
	
	mouse_state |= E_MOUSE_INIT;
	return 0;
}

u8 mouse_read(MOUSE_DATA *data)
{
	s16 x,y;
	static u8 last_state = 0;
	u8 state = 0;
	
	if(mouse_state & E_MOUSE_INIT)
	{
		mouse_navi_read(&x, &y);
		data->x = (s8)x;
		data->y = (s8)y;
		
		data->wheel = mouse_wheel_read();
		
		data->keys = mouse_keys_read();
		
		state = data->keys;
		
		if((state != last_state) || data->x != 0 || data->y != 0 || data->wheel != 0)
		{
			last_state = state;
			return 1;
		}else
		{
			last_state = state;
			return 0;
		}
	}else
	{
		return 0xFF;
	}
}

const MOUSE_DEV Mouse = 
{
	.init = mouse_init,
	.read = mouse_read,
	
};

