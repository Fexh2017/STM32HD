#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f10x.h"
#include "config.h"

#define SPI_HW	1	//Ó²¼þSPI
#define SPI_SW	2	//Èí¼þSPI

#define SPI_MODE	SPI_HW

#define SPI_MISO 	GPIO_PA_06
#define SPI_MOSI 	GPIO_PA_07
#define SPI_CLK	 	GPIO_PA_05
#define SPI_DELAY 	0
#define SPI_BAUD	SPI_BaudRatePrescaler_128	// 72M/x

void f_spi_init(u32 cs);
void f_spi_cs(u32 pin, u8 state);
u8 f_spi_rw(u8 data);
void f_spi_send(u8 addr, u8 data);
u8 f_spi_read(u8 addr);
u32 f_spi_sendbuf(u8 addr, u8* send_buf, u32 len);
u32 f_spi_readbuf(u8 addr, u8* recv_buf, u32 len);

#endif
