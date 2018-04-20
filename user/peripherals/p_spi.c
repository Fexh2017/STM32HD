#include "p_spi.h"

#include "f_gpio.h"
#include "f_rcc.h"

#include "public.h"


#define SPI_MISO GPIO_PA_06
#define SPI_MOSI GPIO_PA_07
#define SPI_CLK	 GPIO_PA_05

#define SPI_DELAY 0

void p_spi_init(void)
{
	static u8 init = 0;
	if(init == 0)
	{
		init = 1;
		f_rcc_enable(e_RCC_GPIOA);
		
		f_gpio_init(SPI_MISO, GPIO_Mode_IPU);
		f_gpio_init(SPI_MOSI, GPIO_Mode_Out_PP);
		f_gpio_init(SPI_CLK,  GPIO_Mode_Out_PP);
		
		f_gpio_set(SPI_CLK);
	}
}

void p_spi_cs(u32 pin, u8 state)
{
	if(state)
	{
		f_gpio_reset(pin);
	}else
	{
		f_gpio_set(pin);
	}
}

u8 p_spi_rw(u8 data)
{
	int i;
	u8 tmp = data;
	for(i = 0; i < 8; i++)
	{
		f_gpio_reset(SPI_CLK);
		delay(SPI_DELAY);
		if(tmp & 0x80)
		{
			f_gpio_set(SPI_MOSI);
		}else
		{
			f_gpio_reset(SPI_MOSI);
		}
		tmp <<= 1;
		delay(SPI_DELAY);
		f_gpio_set(SPI_CLK);
		delay(SPI_DELAY);
		tmp |= (!!f_gpio_read(SPI_MISO));
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

