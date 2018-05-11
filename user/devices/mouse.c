#include "config.h"
#include "mouse.h"

#if CONFIG_MOUSE_EN

#include "public.h"
#include "f_rcc.h"
#include "f_gpio.h"
#include "f_timer.h"
#include "f_spi.h"

#include "led.h"
#include "flash.h"

#if (MOUSE_DEBUG_EN >= CONFIG_DEBUG_LV_H)
#define DEV_DBG(...)					DEBUG("[DEV DBG] ",__VA_ARGS__)
#define DEV_DBGN(...)					DEBUGN("",__VA_ARGS__)
#else
#define DEV_DBG(...) 
#define DEV_DBGN(...) 
#endif
#if (MOUSE_DEBUG_EN >= CONFIG_DEBUG_LV_M)
#define DEV_LOG(...)					DEBUG("[DEV]     ",__VA_ARGS__)
#else
#define DEV_LOG(...)  
#endif
#if (MOUSE_DEBUG_EN >= CONFIG_DEBUG_LV_L)
#define DEV_ERR(...)					DEBUG("[DEV_ERR] ",__VA_ARGS__)
#else
#define DEV_ERR(...)  
#endif



u8 mouse_state = e_MOUSE_IDLE;
u8 mouse_cpi   = 3;


/*  µ¼º½Ä£¿é  */
#define MOUSE_NAVI_CSN		GPIO_PB_10
#define MOUSE_NAVI_MOTION	GPIO_PB_11

#define MOUSE_NAVI_CPI_0400	0x00
#define MOUSE_NAVI_CPI_0800	0x20
#define MOUSE_NAVI_CPI_1200	0x40
#define MOUSE_NAVI_CPI_1600	0x60

u8 mouse_navi_cpi_buf[] = 
{
	MOUSE_NAVI_CPI_0400,
	MOUSE_NAVI_CPI_0800,
	MOUSE_NAVI_CPI_1200,
	MOUSE_NAVI_CPI_1600,
};

u8 mouse_navi_init_buf[] = 
{
	0x1A, 0xC0,
	0x1F, 0x00,
	0x1C, 0x80,
	0x1D, 0x7F,
	0x13, 0x04,	//downshift_time = x * 8 * run_rate
//	0x36, 0x04,	//800 cpi
	
};

void mouse_navi_writereg(u8 addr, u8 data)
{
	if(f_spi_cs(MOUSE_NAVI_CSN, 1) != 0)return;
	delay(10);
	f_spi_send(addr | 0x80, data);
	delay(10);
	f_spi_cs(MOUSE_NAVI_CSN, 0);
}

u8 mouse_navi_readreg(u8 addr)
{
	u8 data;
	if(f_spi_cs(MOUSE_NAVI_CSN, 1) != 0)return 0;
	delay(10);
	data = f_spi_read(addr);
	delay(10);
	f_spi_cs(MOUSE_NAVI_CSN, 0);
	delay(10);
	return data;
}

u32 mouse_navi_readregbuf(u8 addr, u8* buf, u32 len)
{
	int i;
	if(f_spi_cs(MOUSE_NAVI_CSN, 1) != 0)return 0;
	delay(10);
	f_spi_readbuf(addr, buf, len);
	delay(10);
	f_spi_cs(MOUSE_NAVI_CSN, 0);
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
		DEV_LOG("mouse check id %02x%02x", id, rev);
		return 0;
	}
	DEV_ERR("MOUSE CHECK ID ERR %02x%02x", id, rev);
	return (u8)-1;
}

void mouse_navi_set_cpi(u8 cpi)
{
	u8 buf[4];
	if(cpi == MOUSE_NAVI_CPI_0400 || cpi == MOUSE_NAVI_CPI_0800 || cpi == MOUSE_NAVI_CPI_1200 || cpi == MOUSE_NAVI_CPI_1600)
	{
		DEV_LOG("mouse set cpi %d",(((cpi >> 5) + 1) * 400));
		mouse_navi_writereg(0x12,cpi);
		mouse_navi_writereg(0x2E, 0x00);
		mouse_navi_readregbuf(0x42, buf, 4);
	}else
	{
		DEV_ERR("MOUSE SET CPI ERR %d",cpi);
	}
}

u8 mouse_navi_init(void)
{
	u8 buf[4];
	f_spi_init(MOUSE_NAVI_CSN);
	f_gpio_init(MOUSE_NAVI_MOTION, GPIO_Mode_IPU);
	
	f_gpio_reset(MOUSE_NAVI_CSN);
	delayms(10);
	f_gpio_set(MOUSE_NAVI_CSN);
	delayms(10);
	f_gpio_reset(MOUSE_NAVI_CSN);
	delayms(10);
	f_gpio_set(MOUSE_NAVI_CSN);
	delayms(10);
	
	if(mouse_navi_check() == 0)
	{
		DEV_LOG("mouse init");
		mouse_navi_writereg(0x3A, 0x5A);
		delayms(10);
		
		mouse_navi_readregbuf(0x42, buf, 4);
		
//		mouse_navi_writereg(0x3C, 0x27);
//		mouse_navi_writereg(0x22, 0x0A);
//		mouse_navi_writereg(0x21, 0x01);
//		mouse_navi_writereg(0x3C, 0x32);
//		mouse_navi_writereg(0x23, 0x20);
//		mouse_navi_writereg(0x3C, 0x05);
//		mouse_navi_writereg(0x37, 0xB9);
		delayms(10);

		for(int i = 0; i < sizeof(mouse_navi_init_buf); i += 2)
		{
			mouse_navi_writereg(mouse_navi_init_buf[i], mouse_navi_init_buf[i + 1]);
		}
		
		mouse_navi_set_cpi(mouse_navi_cpi_buf[mouse_cpi]);
		
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
		*y = -*y;
		//DEV_DBG("%4d %4d", *x, *y);
	}else
	{
		*x = 0;
		*y = 0;
	}
}



/*   ¹öÂÖ   */
void mouse_wheel_init(void)
{
	TIM_ICInitTypeDef Tim_ICInitStructure;
	
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
	s8 value =  last_wheel - (s8)tmp;
	value /= 2;
	if(value)
	{
		last_wheel = tmp;
	}
	return value;
}




/*   °´¼ü   */
#define MOUSE_KEYS_1 GPIO_PB_08
#define MOUSE_KEYS_2 GPIO_PB_09
#define MOUSE_KEYS_3 GPIO_PB_14
#define MOUSE_KEYS_4 GPIO_PB_13
#define MOUSE_KEYS_5 GPIO_PA_14
#define MOUSE_KEYS_6 GPIO_PA_15

void mouse_keys_init(void)
{
	f_gpio_init(MOUSE_KEYS_1, GPIO_Mode_IPU);
	f_gpio_init(MOUSE_KEYS_2, GPIO_Mode_IPU);
	f_gpio_init(MOUSE_KEYS_3, GPIO_Mode_IPU);
	f_gpio_init(MOUSE_KEYS_4, GPIO_Mode_IPU);
	f_gpio_init(MOUSE_KEYS_5, GPIO_Mode_IPU);
	f_gpio_init(MOUSE_KEYS_6, GPIO_Mode_IPU);
}

u8 mouse_keys_read(void)
{
	u8 keys = 0;
	if(f_gpio_read(MOUSE_KEYS_1) == 0)
	{//×ó¼ü
		keys |= 0x01;
	}
	if(f_gpio_read(MOUSE_KEYS_2) == 0)
	{//ÓÒ¼ü
		keys |= 0x02;
	}
	if(f_gpio_read(MOUSE_KEYS_5) == 0)
	{//ÖÐ¼ä
		keys |= 0x04;
	}
	if(f_gpio_read(MOUSE_KEYS_3) == 0)
	{//²à¼ü1
		keys |= 0x08;
	}
	if(f_gpio_read(MOUSE_KEYS_4) == 0)
	{//²à¼ü2
		keys |= 0x10;
	}
	return keys;
}

u8 mouse_funs_det(void)
{
	static u8 key_fun_state = e_MOUSE_FUN_STATE_EMPTY;
	static u8 key_fun_last = 0;
	u8 key_fun_now = 0;
	u8 ret = 0;
	
	if(mouse_state != e_MOUSE_IDLE && mouse_state != e_MOUSE_ERR)
	{
		if(f_gpio_read(MOUSE_KEYS_6) == 0)
		{
			switch(key_fun_state)
			{
				case e_MOUSE_FUN_STATE_EMPTY:
					key_fun_state = e_MOUSE_FUN_STATE_CPI;
					key_fun_last = 0;
					DEV_DBG("6d ");
					break;
				case e_MOUSE_FUN_STATE_CPI:
					key_fun_now = mouse_keys_read();
					if(key_fun_now != key_fun_last)
					{
						DEV_DBGN("%02X ",key_fun_now);
						if(key_fun_last == 0x08 && key_fun_now == 0x00)
						{
							key_fun_state = e_MOUSE_FUN_STATE_RECORD;
							key_fun_last = 0;
						}else if(key_fun_last == 0x10 && key_fun_now == 0x00)
						{
							key_fun_state = e_MOUSE_FUN_STATE_REPLAY;
							key_fun_last = 0;
						}else if(key_fun_last)
						{
							key_fun_state = e_MOUSE_FUN_STATE_ERR;
							key_fun_last = 0;
						}else
						{
							key_fun_last = key_fun_now;
						}
					}
					break;
				case e_MOUSE_FUN_STATE_RECORD:
					key_fun_now = mouse_keys_read();
					if(key_fun_now != key_fun_last)
					{
						DEV_DBGN("%02X ",key_fun_now);
						if(key_fun_last == 0x08 && key_fun_now == 0x00)
						{
							key_fun_state = e_MOUSE_FUN_STATE_MACRO;
							key_fun_last = 0;
						}else if(key_fun_last)
						{
							key_fun_state = e_MOUSE_FUN_STATE_ERR;
							key_fun_last = 0;
						}else
						{
							key_fun_last = key_fun_now;
						}
					}
					break;
				case e_MOUSE_FUN_STATE_REPLAY:
					key_fun_now = mouse_keys_read();
					if(key_fun_now != key_fun_last)
					{
						DEV_DBGN("%02X ",key_fun_now);
						if(key_fun_last == 0x10 && key_fun_now == 0x00)
						{
							key_fun_state = e_MOUSE_FUN_STATE_MACRO_EDIT;
							key_fun_last = 0;
						}else if(key_fun_last)
						{
							key_fun_state = e_MOUSE_FUN_STATE_ERR;
							key_fun_last = 0;
						}else
						{
							key_fun_last = key_fun_now;
						}
					}
					break;
				case e_MOUSE_FUN_STATE_MACRO_EDIT:
					key_fun_now = mouse_keys_read();
					if(key_fun_now != key_fun_last)
					{
						DEV_DBGN("%02X ",key_fun_now);
						if(key_fun_last)
						{
							key_fun_state = e_MOUSE_FUN_STATE_ERR;
							key_fun_last = 0;
						}else
						{
							key_fun_last = key_fun_now;
						}
					}
					break;
				case e_MOUSE_FUN_STATE_MACRO:
					key_fun_now = mouse_keys_read();
					if(key_fun_now != key_fun_last)
					{
						DEV_DBGN("%02X ",key_fun_now);
						if(key_fun_last)
						{
							key_fun_state = e_MOUSE_FUN_STATE_ERR;
							key_fun_last = 0;
						}else
						{
							key_fun_last = key_fun_now;
						}
					}
					break;
				default:
					key_fun_state = e_MOUSE_FUN_STATE_ERR;
					break;
			}
			return e_MOUSE_FUN_STATE_DET;
		}else
		{
			if(key_fun_state != e_MOUSE_FUN_STATE_EMPTY)
			{
				DEV_DBG("MOUSE FUN SET %d",key_fun_state);
				ret = key_fun_state;
				key_fun_state = e_MOUSE_FUN_STATE_EMPTY;
				return ret;
			}
			return e_MOUSE_FUN_STATE_EMPTY;
		}
	}
	return e_MOUSE_FUN_STATE_ERR;
}



#pragma pack(1)
typedef struct _MOUSE_MACRO_ITEM{
	u8  cnt;
	u16 time_after;
	u8  keys;
	s8  x;
	s8  y;
	s8  wheel;
	u8  sum;
}MOUSE_MACRO_ITEM;
#pragma pack()

typedef struct _MOUSE_MACRO_STATUS{
	u32 page;
	u16 cnt;
	u32 last_time;
}MOUSE_MACRO_STATUS;

static MOUSE_MACRO_ITEM   mouse_macro_data0[MOUSE_MACRO_PAGE_COUNT];
static MOUSE_MACRO_ITEM   mouse_macro_data1[MOUSE_MACRO_PAGE_COUNT];
static MOUSE_MACRO_ITEM*  mouse_macro_data = mouse_macro_data0;
static MOUSE_MACRO_STATUS mouse_macro_status;


void mouse_funs_exit(void)
{
	u8 retry = 0;
	switch(mouse_state)
	{
		case e_MOUSE_NORMAL:
			break;
		case e_MOUSE_RECORD:
			if(MOUSE_MACRO_PAGE_COUNT * mouse_macro_status.page + mouse_macro_status.cnt < MOUSE_MACRO_ITEM_COUNT - 1)
			{
				mouse_macro_status.cnt++;
				mouse_macro_data[mouse_macro_status.cnt].time_after = 0;
			}
			
			retry = 0;
			while(retry++ < 5 && Flash.write(MOUSE_MACRO_ADDR_RECORD + MOUSE_MACRO_ITEM_SIZE * MOUSE_MACRO_PAGE_COUNT * mouse_macro_status.page, (u8*)mouse_macro_data, (mouse_macro_status.cnt + 1) * MOUSE_MACRO_ITEM_SIZE))delayms(1);
			if(retry >= 5)
			{
				DEV_ERR("FLASH PROGRAM ERR");
			}
			
			mouse_macro_status.page = 0;
			mouse_macro_status.cnt = 0;
			mouse_macro_status.last_time = 0;
			mouse_macro_data = mouse_macro_data0;
			break;
		case e_MOUSE_REPLAY:
			mouse_macro_status.page = 0;
			mouse_macro_status.cnt = 0;
			mouse_macro_status.last_time = 0;
			mouse_macro_data = mouse_macro_data0;
			break;
		case e_MOUSE_MACRO_EDIT:
			break;
		case e_MOUSE_MACRO:
			break;
		default:
			DEV_ERR("MOUSE FUN ERR %d",mouse_state);
			break;
	}
	mouse_state = e_MOUSE_NORMAL;
	Led.write(e_LED_FUN, NULL, e_LED_FUN_RAINBOW);
}


u8 mouse_init(void)
{
	u8 err = 0;
	while(1)
	{
		err = mouse_navi_init();
		if(err == 0)break;
		delayms(100);
	}
	
	if(err)
	{
		mouse_state = e_MOUSE_ERR;
		return err;
	}
	
	mouse_wheel_init();
	mouse_keys_init();
	
	mouse_state = e_MOUSE_NORMAL;
	return 0;
}

u8 mouse_read(u32 addr, void *data, u32 arg)
{
	static u8 last_state = 0;
	u8 state = 0;
	MOUSE_DATA tmp_data;
	MOUSE_DATA *d = data;
	u8 retry = 0;
	
	u8 mouse_fun = mouse_funs_det();
	switch(mouse_fun)
	{
		case e_MOUSE_FUN_STATE_EMPTY:
			break;
		case e_MOUSE_FUN_STATE_CPI:
			if(mouse_state == e_MOUSE_NORMAL)
			{
				mouse_cpi = mouse_cpi >= 3 ? 0 : mouse_cpi + 1;
				mouse_navi_set_cpi(mouse_navi_cpi_buf[mouse_cpi]);
			}else
			{
				mouse_funs_exit();
				DEV_LOG("mouse fun set normal");
				Led.write(e_LED_FUN, NULL, e_LED_FUN_RAINBOW);
				mouse_state = e_MOUSE_NORMAL;
			}
			return 0;
		case e_MOUSE_FUN_STATE_RECORD:
			mouse_funs_exit();
			retry = 0;
			while(retry++ < 5 && Flash.write(MOUSE_MACRO_ADDR_RECORD, 0, 0x10000))delayms(1);
			if(retry >= 5)
			{
				DEV_ERR("FLASH ERASE ERR");
				break;
			}
			delayms(150);
			retry = 0;
			while(retry++ < 5 && Flash.write(MOUSE_MACRO_ADDR_RECORD + 0x10000, 0, 0x10000))delayms(30);
			if(retry >= 5)
			{
				DEV_ERR("FLASH ERASE ERR");
				break;
			}
			delayms(150);
			Led.write(e_LED_FUN, NULL, e_LED_FUN_GREEN);
			DEV_LOG("mouse fun set record");
			mouse_state = e_MOUSE_RECORD;
			mouse_macro_status.page = 0;
			mouse_macro_status.cnt = 0;
			mouse_macro_data = mouse_macro_data0;
			mouse_macro_status.last_time = sys_get_ms();
			break;
		case e_MOUSE_FUN_STATE_REPLAY:
			mouse_funs_exit();
			retry = 0;
			while(retry++ < 5 && Flash.read(MOUSE_MACRO_ADDR_RECORD, mouse_macro_data0, MOUSE_MACRO_ITEM_SIZE * MOUSE_MACRO_PAGE_COUNT))delayms(1);
			if(retry >= 5)
			{
				DEV_ERR("FLASH READ ERR");
				break;
			}
			Led.write(e_LED_FUN, NULL, e_LED_FUN_BLUE);
			DEV_LOG("mouse set fun replay");
			mouse_state = e_MOUSE_REPLAY;
			mouse_macro_status.page = 0;
			mouse_macro_status.cnt = 0;
			mouse_macro_data = mouse_macro_data0;
			mouse_macro_status.last_time = sys_get_ms();
			break;
		case e_MOUSE_FUN_STATE_MACRO_EDIT:
			mouse_funs_exit();
			DEV_LOG("mouse fun save");
			Led.write(e_LED_FUN, NULL, e_LED_FUN_RAINBOW);
			mouse_state = e_MOUSE_NORMAL;
			break;
		case e_MOUSE_FUN_STATE_MACRO:
			mouse_funs_exit();
			DEV_LOG("mouse fun macro");
			mouse_state = e_MOUSE_MACRO;
			break;
		case e_MOUSE_FUN_STATE_DET:
			return 0;
		default:
			mouse_funs_exit();
			DEV_ERR("MOUSE FUN ERR %d", mouse_fun);
			return 0xFF;
	}
	
	
	mouse_navi_read(&tmp_data.x, &tmp_data.y);
	tmp_data.wheel = mouse_wheel_read();
	tmp_data.keys = mouse_keys_read();
	state = tmp_data.keys;
	
	switch(mouse_state)
	{
		case e_MOUSE_NORMAL:
			if((state != last_state) || tmp_data.x != 0 || tmp_data.y != 0 || tmp_data.wheel != 0)
			{
				d->keys = tmp_data.keys;
				d->x = tmp_data.x;
				d->y = tmp_data.y;
				d->wheel = tmp_data.wheel;
				DEV_DBG("mouse %02x %4d %4d %4d", tmp_data.keys, tmp_data.x, tmp_data.y, tmp_data.wheel);
				last_state = state;
				return 1;
			}else
			{
				last_state = state;
				return 0;
			}
		case e_MOUSE_FUN_STATE_RECORD:
			if((state != last_state) || tmp_data.x != 0 || tmp_data.y != 0 || tmp_data.wheel != 0)
			{
				mouse_macro_data[mouse_macro_status.cnt].time_after = sys_get_time_after(mouse_macro_status.last_time);
				mouse_macro_data[mouse_macro_status.cnt].time_after = mouse_macro_data[mouse_macro_status.cnt].time_after == 0 ? 1 : mouse_macro_data[mouse_macro_status.cnt].time_after;
				mouse_macro_data[mouse_macro_status.cnt].cnt   = mouse_macro_status.cnt;
				mouse_macro_data[mouse_macro_status.cnt].keys  = tmp_data.keys;
				mouse_macro_data[mouse_macro_status.cnt].x     = (s8)tmp_data.x;
				mouse_macro_data[mouse_macro_status.cnt].y     = (s8)tmp_data.y;
				mouse_macro_data[mouse_macro_status.cnt].wheel = (s8)tmp_data.wheel;
				mouse_macro_data[mouse_macro_status.cnt].sum   = 0;
				for(int i = 0; i< sizeof(MOUSE_MACRO_ITEM) - 2; i++)
				{
					mouse_macro_data[mouse_macro_status.cnt].sum += ((u8*)&mouse_macro_data[mouse_macro_status.cnt])[i];
				}
				DEV_DBG("mouse record %2d %8d %02X %6d %6d %6d %02X", 	mouse_macro_data[mouse_macro_status.cnt].cnt,
																		mouse_macro_data[mouse_macro_status.cnt].time_after,
																		mouse_macro_data[mouse_macro_status.cnt].keys,
																		mouse_macro_data[mouse_macro_status.cnt].x,
																		mouse_macro_data[mouse_macro_status.cnt].y,
																		mouse_macro_data[mouse_macro_status.cnt].wheel,
																		mouse_macro_data[mouse_macro_status.cnt].sum);
				mouse_macro_status.cnt ++;
				mouse_macro_status.last_time = sys_get_ms();
				
				if(mouse_macro_status.cnt >= MOUSE_MACRO_PAGE_COUNT)
				{
					retry = 0;
					while(retry++ < 5 && Flash.write(MOUSE_MACRO_ADDR_RECORD + MOUSE_MACRO_ITEM_SIZE * MOUSE_MACRO_PAGE_COUNT * mouse_macro_status.page, (u8*)mouse_macro_data, MOUSE_MACRO_ITEM_SIZE * MOUSE_MACRO_PAGE_COUNT))delayms(1);
					if(retry >= 5)
					{
						DEV_ERR("FLASH PROGRAM ERR");
					}
					
					if(mouse_macro_data == mouse_macro_data0)mouse_macro_data = mouse_macro_data1;
					else mouse_macro_data = mouse_macro_data0;
					
					mouse_macro_status.cnt = 0;
					mouse_macro_status.page ++;
				}
				
				if(mouse_macro_status.page >= (MOUSE_MACRO_ITEM_COUNT / MOUSE_MACRO_PAGE_COUNT))
				{
					DEV_ERR("MOUSE RECORD OVERFLOW");
					DEV_LOG("mouse fun set normal");
					Led.write(e_LED_FUN, NULL, e_LED_FUN_RAINBOW);
					mouse_state = e_MOUSE_NORMAL;
					mouse_macro_status.cnt = 0;
					mouse_macro_status.last_time = 0;
				}
				
				d->keys = tmp_data.keys;
				d->x = tmp_data.x;
				d->y = tmp_data.y;
				d->wheel = tmp_data.wheel;
				last_state = state;
				return 1;
			}else
			{
				last_state = state;
				return 0;
			}
		case e_MOUSE_FUN_STATE_REPLAY:
			if(mouse_macro_status.cnt < MOUSE_MACRO_ITEM_COUNT && mouse_macro_data[mouse_macro_status.cnt].time_after != 0)
			{
				u8 sum = 0;
				for(int i = 0; i< sizeof(MOUSE_MACRO_ITEM) - 1; i++)
				{
					sum += ((u8*)&mouse_macro_data[mouse_macro_status.cnt])[i];
				}
				if(sum != mouse_macro_data[mouse_macro_status.cnt].sum)
				{
					DEV_ERR("MOUSE REPLAY ITEM CHECK ERR %2d %8d %02X %6d %6d %6d %02X", 	
							mouse_macro_data[mouse_macro_status.cnt].cnt,
							mouse_macro_data[mouse_macro_status.cnt].time_after,
							mouse_macro_data[mouse_macro_status.cnt].keys,
							mouse_macro_data[mouse_macro_status.cnt].x,
							mouse_macro_data[mouse_macro_status.cnt].y,
							mouse_macro_data[mouse_macro_status.cnt].wheel,
							mouse_macro_data[mouse_macro_status.cnt].sum);
					mouse_funs_exit();
					return 0;
				}
				if(sys_get_time_after(mouse_macro_status.last_time) >= mouse_macro_data[mouse_macro_status.cnt].time_after)
				{
					DEV_DBG("mouse replay %2d %8d %02X %6d %6d %6d %02X", 	mouse_macro_status.cnt,
																			mouse_macro_data[mouse_macro_status.cnt].time_after,
																			mouse_macro_data[mouse_macro_status.cnt].keys,
																			mouse_macro_data[mouse_macro_status.cnt].x,
																			mouse_macro_data[mouse_macro_status.cnt].y,
																			mouse_macro_data[mouse_macro_status.cnt].wheel,
																			mouse_macro_data[mouse_macro_status.cnt].sum);
					d->keys  = mouse_macro_data[mouse_macro_status.cnt].keys;
					d->x     = mouse_macro_data[mouse_macro_status.cnt].x;
					d->y     = mouse_macro_data[mouse_macro_status.cnt].y;
					d->wheel = mouse_macro_data[mouse_macro_status.cnt].wheel;
					mouse_macro_status.cnt++;
					mouse_macro_status.last_time = sys_get_ms();
					
					if(mouse_macro_status.cnt >= MOUSE_MACRO_PAGE_COUNT)
					{
						mouse_macro_status.cnt = 0;
						mouse_macro_status.page ++;
						
						retry = 0;
						while(retry++ < 5 && Flash.read(MOUSE_MACRO_ADDR_RECORD + MOUSE_MACRO_ITEM_SIZE * MOUSE_MACRO_PAGE_COUNT * mouse_macro_status.page, (u8*)mouse_macro_data, MOUSE_MACRO_ITEM_SIZE * MOUSE_MACRO_PAGE_COUNT))delayms(1);
						if(retry >= 5)
						{
							DEV_ERR("FLASH READ ERR");
						}
					}
					return 1;
				}
				return 0;
			}else
			{
				DEV_LOG("mouse fun replay end %d", mouse_macro_status.cnt);
				DEV_LOG("mouse fun set normal");
				Led.write(e_LED_FUN, NULL, e_LED_FUN_RAINBOW);
				mouse_state = e_MOUSE_NORMAL;
				mouse_macro_status.cnt = 0;
				mouse_macro_status.last_time = 0;
			}
			return 0;
		default:
			DEV_ERR("MOUSE STATE ERR");
			return 0xFF;
	}
}

u8 mouse_write(u32 addr, void *data, u32 arg)
{
	return 0;
}

u32 mouse_get_status(void)
{
	return mouse_state;
}

#else
u8 mouse_init(void){return 0xFF;}
u32 mouse_get_status(void){return 0xFF;}
u8 mouse_read(u32 addr, void *data, u32 arg){return 0xFF;}
u8 mouse_write(u32 addr, void *data, u32 arg){return 0xFF;}
#endif


const DEVICE Mouse = 
{
	.name = "mouse",
	.init = mouse_init,
	.get_status = mouse_get_status,
	.read = mouse_read,
	.write = mouse_write,
};

