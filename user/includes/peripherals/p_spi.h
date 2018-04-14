#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f10x.h"
#include "config.h"


typedef struct _SPI_DEV{
	void 	(*init)(void);
	void	(*cs)(u16 pin, u8 state);
	u8 		(*rw)(u8 data);
	void	(*send)(u8 addr, u8 data);
	u8		(*read)(u8 addr);
	u32 	(*sendbuf)(u8* send_buf, u32 len);
	u32 	(*readbuf)(u8* recv_buf, u32 len);
}SPI_DEV;

extern const SPI_DEV Spi_dev;

#endif
