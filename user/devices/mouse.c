#include "config.h"

#include "mouse.h"
#include "public.h"

#include "f_rcc.h"
#include "f_gpio.h"

#include "p_spi.h"

#define MOUSE_CEN			GPIO_Pin_3
#define MOUSE_MOTION	GPIO_Pin_4


u8 mouse_init_buf[] = 
{
	0x1A, 0x4A,
	0x1F, 0x80,
	0x1C, 0xFF,
	0x1D, 0x00,
//	0x12, 0x26,	//800 cpi, 8ms run_rate
//	0x13, 0x04,	//downshift_time = x * 8 * run_rate
//	0x36, 0x04,	//800 cpi
	
};

void mouse_write(u8 addr, u8 data)
{
	Spi_dev.cs(MOUSE_CEN, 1);
	delay(10);
	Spi_dev.send(addr | 0x80, data);
	delay(10);
	Spi_dev.cs(MOUSE_CEN, 0);
}

u8 mouse_read(u8 addr)
{
	u8 data;
	Spi_dev.cs(MOUSE_CEN, 1);
	delay(10);
	data = Spi_dev.read(addr);
	delay(10);
	Spi_dev.cs(MOUSE_CEN, 0);
	delay(10);
	return data;
}

u32 mouse_readbuf(u8 addr, u8* buf, u32 len)
{
	int i;
	Spi_dev.cs(MOUSE_CEN, 1);
	delay(10);
	Spi_dev.rw(addr);
	delay(10);
	for(i = 0; i <len; i++)
	{
		buf[i] = Spi_dev.rw(0);
		//buf[i] = mouse_read(addr + i);
	}
	delay(10);
	Spi_dev.cs(MOUSE_CEN, 0);
	return i;
}

u8 mouse_check(void)
{
	u8 id = 0xff;
	u8 rev = 0xff;
	id = mouse_read(0x00);
	rev = mouse_read(0x01);
	if(id == 0x31 && rev == 0x03)
	{
		DEV_LOG("mouse id %02x %02x", id, rev);
		return 0;
	}
	DEV_ERR("mouse id err %02x %02x", id, rev);
	return 0xff;
}

u8 mouse_init(void)
{
	u8 buf[4];
	Spi_dev.init();
	f_gpio_init(GPIOA, MOUSE_CEN, GPIO_Mode_Out_PP);
	f_gpio_init(GPIOA, MOUSE_MOTION, GPIO_Mode_IPU);
	Spi_dev.cs(MOUSE_CEN, 0);
	delayms(100);
	Spi_dev.cs(MOUSE_CEN, 1);
	delayms(100);
	Spi_dev.cs(MOUSE_CEN, 0);
	delayms(100);
	Spi_dev.cs(MOUSE_CEN, 1);
	delayms(100);
	
	if(mouse_check() == 0)
	{
		DEV_LOG("mouse init");
		mouse_write(0x3A, 0x5A);
		delayms(10);
		
		mouse_readbuf(0x42, buf, 4);
//		
		mouse_write(0x3C, 0x27);
		mouse_write(0x22, 0x0A);
		mouse_write(0x21, 0x01);
		mouse_write(0x3C, 0x32);
		mouse_write(0x23, 0x20);
		mouse_write(0x3C, 0x05);
		mouse_write(0x37, 0xB9);
		delayms(10);
		
		//self test
//		mouse_write(0x10,0x01);
//		delayms(500);
//		for(int i = 0x0c; i <= 0x0f; i++)
//		{
//			DEV_DBG("%02X %02X", i, mouse_read(i));
//		}

		for(int i = 0; i < sizeof(mouse_init_buf); i += 2)
		{
			mouse_write(mouse_init_buf[i], mouse_init_buf[i + 1]);
		}
		
		mouse_readbuf(0x42, buf, 4);
		
		mouse_write(0x2E, 0x00);
		delayms(1);
		buf[0] = mouse_read(0x2E);
		
		return 0;
	}
	return 0xff;
}

void mouse_state(s16 *x, s16 *y)
{
	u8 buf[4];
	if(!f_gpio_read(GPIOA, MOUSE_MOTION))
	{
		mouse_readbuf(0x42, buf, 4);
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
//		DEV_DBG("%02x %4d %4d",buf[0], *x, *y);
	}
}





