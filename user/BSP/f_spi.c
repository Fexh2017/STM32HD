#include "f_spi.h"

#include "f_gpio.h"
#include "f_rcc.h"

#include "public.h"


#if SPI_MODE == SPI_HW
void f_spi_init(u32 cs)
{
	SPI_InitTypeDef SPI_InitStructure;
	static u8 init = 0;
	
	if(init == 0)
	{
		init = 1;
		f_rcc_enable(e_RCC_GPIOA);
		
		f_gpio_init(SPI_MISO, GPIO_Mode_AF_PP);
		f_gpio_init(SPI_MOSI, GPIO_Mode_AF_PP);
		f_gpio_init(SPI_CLK,  GPIO_Mode_AF_PP);
		
		f_rcc_enable(e_RCC_SPI1);
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BAUD;
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStructure.SPI_CRCPolynomial = 7;
		
		SPI_Init(SPI1, &SPI_InitStructure);
		SPI_Cmd(SPI1, ENABLE);
	}
	
	f_gpio_init(cs, GPIO_Mode_Out_PP);
	f_gpio_set(cs);
}

u8 f_spi_rw(u8 data)
{
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI1, data);
	retry=0;
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					
}

#elif SPI_MODE == SPI_SW
void f_spi_init(u32 cs)
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
	
	f_gpio_init(cs, GPIO_Mode_Out_PP);
	f_gpio_set(cs);
}

u8 f_spi_rw(u8 data)
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
#else
void f_spi_init(u32 cs){}
u8 f_spi_rw(u8 data){return 0xFF;}

#endif



void f_spi_cs(u32 pin, u8 state)
{
	if(state)
	{
		f_gpio_reset(pin);
	}else
	{
		f_gpio_set(pin);
	}
}

void f_spi_send(u8 addr, u8 data)
{
	f_spi_rw(addr);
	delay(100);
	f_spi_rw(data);
}

u8 f_spi_read(u8 addr)
{
	u8 data;
	f_spi_rw(addr);
	delay(100);
	data = f_spi_rw(0xff);
	return data;
}

u32 f_spi_sendbuf(u8 addr, u8* send_buf, u32 len)
{
	int i;
	f_spi_rw(addr | 0x80);
	for(i = 0; i < len; i++)
	{
		f_spi_rw(send_buf[i]);
	}
	return i;
}

u32 f_spi_readbuf(u8 addr, u8* recv_buf, u32 len)
{
	int i;
	f_spi_rw(addr);
	for(i = 0; i < len; i++)
	{
		recv_buf[i] = f_spi_rw(0xff);
	}
	return i;
}

