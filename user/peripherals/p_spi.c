#include "p_spi.h"

#include "f_gpio.h"
#include "f_rcc.h"

#include "public.h"


#define SPI_MISO GPIO_Pin_6
#define SPI_MOSI GPIO_Pin_7
#define SPI_CLK	 GPIO_Pin_5

#define SPI_DELAY 1

void p_spi_init(void)
{
	static u8 init = 0;
	if(init == 0)
	{
		init = 1;
		f_rcc_enable(e_RCC_GPIOA);
		
		f_gpio_init(GPIOA, SPI_MISO, GPIO_Mode_IN_FLOATING);
		f_gpio_init(GPIOA, SPI_MOSI, GPIO_Mode_Out_PP);
		f_gpio_init(GPIOA, SPI_CLK,  GPIO_Mode_Out_PP);
		
		f_gpio_set(GPIOA, SPI_CLK);
	}
}

void p_spi_cs(u16 pin, u8 state)
{
	if(state)
	{
		f_gpio_reset(GPIOA,pin);
	}else
	{
		f_gpio_set(GPIOA,pin);
	}
}

u8 p_spi_rw(u8 data)
{
	int i;
	u8 tmp = 0;
	for(i = 0; i < 8; i++)
	{
		f_gpio_reset(GPIOA, SPI_CLK);
		delay(SPI_DELAY);
		if(data & (1 << i))
		{
			f_gpio_set(GPIOA, SPI_MOSI);
		}else
		{
			f_gpio_reset(GPIOA, SPI_MOSI);
		}
		delay(SPI_DELAY);
		f_gpio_set(GPIOA, SPI_CLK);
		delay(SPI_DELAY);
		tmp <<= 1;
		tmp |= (!!f_gpio_read(GPIOA, SPI_MISO));
		delay(SPI_DELAY);
	}
	return tmp;
}

void p_spi_send(u8 addr, u8 data)
{
	p_spi_rw(addr);
	delay(100);
	p_spi_rw(data);
}

u8 p_spi_read(u8 addr)
{
	u8 data;
	p_spi_rw(addr);
	delay(100);
	data = p_spi_rw(0xff);
	return data;
}

u32 p_spi_sendbuf(u8* send_buf, u32 len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		p_spi_rw(send_buf[i]);
	}
	return i;
}

u32 p_spi_readbuf(u8* recv_buf, u32 len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		recv_buf[i] = p_spi_rw(0xff);
	}
	return i;
}

const SPI_DEV Spi_dev = 
{
	.init = p_spi_init,
	.cs 	= p_spi_cs,
	.rw		= p_spi_rw,
	.send = p_spi_send,
	.read = p_spi_read,
	.sendbuf = p_spi_sendbuf,
	.readbuf = p_spi_readbuf,
};

