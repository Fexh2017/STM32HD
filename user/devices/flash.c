#include "config.h"
#include "flash.h"

#if CONFIG_FLASH_EN

#if (FLASH_DEBUG_EN >= CONFIG_DEBUG_LV_H)
#define DEV_DBG(...)					DEBUG("[DEV DBG] ",__VA_ARGS__)
#define DEV_DBGN(...)					DEBUGN("",__VA_ARGS__)
#else
#define DEV_DBG(...) 
#define DEV_DBGN(...) 
#endif
#if (FLASH_DEBUG_EN >= CONFIG_DEBUG_LV_M)
#define DEV_LOG(...)					DEBUG("[DEV]     ",__VA_ARGS__)
#else
#define DEV_LOG(...)  
#endif
#if (FLASH_DEBUG_EN >= CONFIG_DEBUG_LV_L)
#define DEV_ERR(...)					DEBUG("[DEV_ERR] ",__VA_ARGS__)
#else
#define DEV_ERR(...)  
#endif

#include "f_gpio.h"
#include "f_spi.h"


#define FLASH_CSN 		GPIO_PB_01


#define FLASH_CMD_ID			0x90	//(ID7-0)(ID15-8)
#define FLASH_CMD_READ_STATUS	0x05	//(STATUS7-0)						//SRP0 SEC TB BP2 BP1 BP0 (WEL) (BUSY)
#define FLASH_CMD_READ_STATUS_1	0x35	//(STATUS15-8)						//(SUS) CMP LB3 LB2 LB1 (R) QE SRP1
#define FLASH_CMD_WRITE_STATUS	0x01	//STATUS7-0							//(SRP0) SEC TB BP2 BP1 BP0 (WEL) (BUSY)
#define FLASH_CMD_WRITE_ENABLE	0x06	//X
#define FLASH_CMD_WRITE_DISABLE	0x04	//X
#define FLASH_CMD_PROGRAM		0x02	//A23-16 A15-8 A7-0 D7-0 ...
#define FLASH_CMD_READ			0x03	//A23-16 A15-8 A7-0 (D7-0) ...
#define FLASH_CMD_ERASE_4K		0x20	//A23-16 A15-8 A7-0 
#define FLASH_CMD_ERASE_32K		0x52	//A23-16 A15-8 A7-0 
#define FLASH_CMD_ERASE_64K		0xD8	//A23-16 A15-8 A7-0 
#define FLASH_CMD_ERASE_CHIP	0xC7	//X

#define FLASH_ID_W25Q80 0xEF13


FLASH_INFO flash_info;

void flash_wcmd(u8 cmd)
{
	f_spi_rw(cmd);
}

void flash_waddr(u32 addr)
{
	f_spi_rw((addr>>16) & 0x000000FF);
	f_spi_rw((addr>>8)& 0x000000FF);
	f_spi_rw((addr)& 0x000000FF);
}

void flash_wdata(u8* data, u32 len)
{
	for(int i = 0; i < len; i++)
	{
		f_spi_rw(data[i]);
	}
}

u32 flash_rcmd(u8 len)
{
	u32 tmp = 0;
	for(int i = len; i > 0; i--)
	{
		tmp <<= 8;
		tmp |=	f_spi_rw(0xFF);
	}
	return tmp;
}

void flash_rdata(u8* buf, u32 len)
{
	for(int i = 0; i < len; i++)
	{
		buf[i] = f_spi_rw(0xFF);
	}
}

u16 flash_status(void)
{
	u16 status = 0;
	if(flash_info.id == 0)return 0xFFFF;
	if(f_spi_cs(FLASH_CSN, 1) != 0 )return 0xFFFF;
	flash_wcmd(FLASH_CMD_READ_STATUS);
	status = flash_rcmd(1);
	f_spi_cs(FLASH_CSN, 0);
	if(f_spi_cs(FLASH_CSN, 1) != 0 )return 0xFFFF;
	flash_wcmd(FLASH_CMD_READ_STATUS_1);
	status |= (flash_rcmd(1) << 8);
	f_spi_cs(FLASH_CSN, 0);
	return status;
}

u8 flash_write_enable(void)
{
	if(flash_info.id == 0)return (u8)-1;
	if(f_spi_cs(FLASH_CSN, 1) != 0 )return (u8)-1;
	flash_wcmd(FLASH_CMD_WRITE_ENABLE);
	f_spi_cs(FLASH_CSN,0);
	return 0;
}

u8 flash_write_disable(void)
{
	if(flash_info.id == 0)return (u8)-1;
	if(f_spi_cs(FLASH_CSN, 1) != 0 )return (u8)-1;
	flash_wcmd(FLASH_CMD_WRITE_DISABLE);
	f_spi_cs(FLASH_CSN,0);
	return 0;
}

u8 flash_is_busy(void)
{
	u16 status = flash_status();
	if(status == 0xFFFF)return (u8)-1;
	else if(status & 0x0001)return 1;
	else return 0;
}

u8 flash_program(u32 addr, u8* buf, u32 len)
{
	if(buf == NULL || len == 0) return (u8)-1;
	if(flash_info.id == 0)return (u8)-1;
	if(flash_is_busy())return (u8)1;
	if(len <= flash_info.page_size - (addr % flash_info.page_size))
	{
		if(flash_write_enable())return (u8)-1;
		if(f_spi_cs(FLASH_CSN, 1) != 0 )return (u8)-1;
		flash_wcmd(FLASH_CMD_PROGRAM);
		flash_waddr(addr);
		flash_wdata(buf, len);
		f_spi_cs(FLASH_CSN,0);
		DEV_LOG("flash program %08X %d", addr, len);
		return 0;
	}
	else
	{
		DEV_ERR("FLASH PEOGRAM ERR %08X %d", addr, len);
		return (u8)-1;
	}
}

u8 flash_read_data(u32 addr, u8* buf, u32 len)
{
	if(buf == NULL || len == 0) return (u8)-1;
	if(flash_info.id == 0)return (u8)-1;
	if(flash_is_busy())return (u8)1;
	if(len <= flash_info.size - addr)
	{
		if(f_spi_cs(FLASH_CSN, 1) != 0 )return (u8)-1;
		flash_wcmd(FLASH_CMD_READ);
		flash_waddr(addr);
		flash_rdata(buf, len);
		f_spi_cs(FLASH_CSN,0);
		DEV_LOG("flash read data %08X %d", addr, len);
		return 0;
	}
	else
	{
		DEV_ERR("FLASH READ DATA ERR %08X %d", addr, len);
		return (u8)-1;
	}
}

u8 flash_erase(u32 addr, u32 len)
{
	if(flash_info.id == 0)return (u8)-1;
	if(flash_is_busy())return (u8)1;
	if(len == 0 && addr == 0)
	{
		if(flash_write_enable())return (u8)-1;
		if(f_spi_cs(FLASH_CSN, 1) != 0 )return (u8)-1;
		flash_wcmd(FLASH_CMD_ERASE_CHIP);
		f_spi_cs(FLASH_CSN,0);
		DEV_LOG("flash erase chip");
		return 0;
	}else
	{
		if(addr % len == 0)
		{
			if(len == flash_info.sector_size)
			{
				if(flash_write_enable())return (u8)-1;
				if(f_spi_cs(FLASH_CSN, 1) != 0 )return (u8)-1;
				flash_wcmd(FLASH_CMD_ERASE_4K);
				flash_waddr(addr);
				f_spi_cs(FLASH_CSN,0);
				DEV_LOG("flash erase sector %08X", addr);
				return 0;
			}else if(len == (flash_info.block_size / 2))
			{
				if(flash_write_enable())return (u8)-1;
				if(f_spi_cs(FLASH_CSN, 1) != 0 )return (u8)-1;
				flash_wcmd(FLASH_CMD_ERASE_32K);
				flash_waddr(addr);
				f_spi_cs(FLASH_CSN,0);
				DEV_LOG("flash erase half block %08X", addr);
				return 0;
			}else if(len == flash_info.block_size)
			{
				if(flash_write_enable())return (u8)-1;
				if(f_spi_cs(FLASH_CSN, 1) != 0 )return (u8)-1;
				flash_wcmd(FLASH_CMD_ERASE_64K);
				flash_waddr(addr);
				f_spi_cs(FLASH_CSN,0);
				DEV_LOG("flash erase block %08X", addr);
				return 0;
			}else
			{
				DEV_ERR("FLASH ERASE ERR %08X %d", addr, len);
				return (u8)-1;
			}
		}else
		{
			DEV_ERR("FLASH ERASE ERR %08X %d", addr, len);
			return (u8)-1;
		}
	}
}



u8 flash_check(void)
{
	u8 retry = 0;
	flash_info.id = 0;
	
	while(retry++ < 3)
	{
		if(f_spi_cs(FLASH_CSN, 1) != 0 )continue;
		flash_wcmd(FLASH_CMD_ID);
		flash_waddr(0);
		flash_info.id = flash_rcmd(2);
		f_spi_cs(FLASH_CSN, 0);
		
		if(flash_info.id == FLASH_ID_W25Q80)
		{
			flash_write_enable();
			flash_info.status = flash_status();
			flash_info.size = 0x100000;
			flash_info.page_size = 0x100;
			flash_info.sector_size = 0x1000;
			flash_info.block_size = 0x10000;
			flash_info.block_count = 16;
			DEV_LOG("flash check id %04X %04X",flash_info.id,flash_info.status);
			return 0;
		}
		else 
		{
			DEV_ERR("FLASH ID ERR %04X",flash_info.id);
		}
	}
	return (u8)-1;
}

u8 flash_init(void)
{
	f_spi_init(FLASH_CSN);
	
	if(flash_check() != 0)return (u8)-1;
	
	return 0;
}
u32 flash_get_status(void)
{
	if(flash_info.id == 0)return (u32)0;
	flash_info.status = flash_status();
	return (u32)&flash_info;
}
u8 flash_read(u32 addr, void *data, u32 arg)
{
	u32 len = arg;
	if(flash_info.id == 0)return (u8)-1;
	if(flash_is_busy())return 1;
	return flash_read_data(addr, data, len);
}
u8 flash_write(u32 addr, void *data, u32 arg)
{
	u32 len = arg;
	if(flash_info.id == 0)return (u8)-1;
	if(flash_is_busy())return 1;
	if(data == NULL)
	{
		return flash_erase(addr, len);
	}else
	{
		return flash_program(addr, data, len);
	}
}

#else
u8 flash_init(void){return 0xFF;}
u32 flash_get_status(void){return 0;}
u8 flash_read(u32 addr, void *data, u32 arg){return 0xFF;}
u8 flash_write(u32 addr, void *data, u32 arg){return 0xFF;}
#endif


const DEVICE Flash = 
{
	.name = "flash",
	.init = flash_init,
	.get_status = flash_get_status,
	.read = flash_read,
	.write = flash_write,
};

