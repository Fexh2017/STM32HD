#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stm32f10x.h"                  // Device header
#include "public.h"

/*�û��궨��================================================================*/
#define CONFIG_HW_VERSION				0x0100	//Ӳ���汾��
#define CONFIG_SW_VERSION				0x0100	//����汾��



/*�̶��궨�壨�벻Ҫ�޸ı��������ݣ�========================================*/
//��ӡ�ȼ�
#define CONFIG_DEBUG_LV_H				3		//��ӡDEBUG + LOG + ERR
#define CONFIG_DEBUG_LV_M				2		//��ӡLOG + ERR
#define CONFIG_DEBUG_LV_L				1		//��ӡERR
#define CONFIG_DEBUG_LV_N				0		//�رմ�ӡ



/*��������==================================================================*/
#define CONFIG_DEBUG_UART_EN 			CONFIG_DEBUG_LV_H				//DEBUG����
#define CONFIG_LED_EN					1								//LED��
#define CONFIG_USB_EN					1								//USB
#define CONFIG_MOUSE_EN					1								//MOUSE
#define CONFIG_FLASH_EN					1								//SPI_FLASH



/*DEBUG��������=============================================================*/
#if CONFIG_DEBUG_UART_EN
#include "stdio.h"
//#include "stdarg.h"
#define DEBUG_UART 				1		//���ں�
#define DEBUG_UART_BAUDRATE		115200	//���ڲ�����

#define CONFIG_DEBUG_DMA 		1		//ʹ��DMA����
#define CONFIG_DEBUG_TIMESTAMP	1		//ʹ��ʱ���

#define DEBUGN(fmt,...) 	do{printf("%s ",fmt);printf(__VA_ARGS__);}while(0)
#if CONFIG_DEBUG_TIMESTAMP
#define DEBUG(fmt,...) 		do{u32 __systime_h = sys_get_hour(); \
							   u32 __systime_m = sys_get_ms(); \
							   printf("\n%02d:%02d:%02d.%03d %s ", __systime_h, (__systime_m / 60000), (__systime_m % 60000 / 1000), (__systime_m % 1000) ,fmt);printf(__VA_ARGS__);}while(0)
#else
#define DEBUG(fmt,...) 		do{printf("\n%s ",fmt);printf(__VA_ARGS__);}while(0)
#endif //CONFIG_DEBUG_TIMESTAMP
#define _debug(...) 		printf(__VA_ARGS__)
#else
#define DEBUGN(fmt,...)
#define DEBUG(fmt,...)
#define _debug
#endif //CONFIG_DEBUG_UART_EN

//SYS_DEBUG����
#if (CONFIG_DEBUG_UART_EN >= CONFIG_DEBUG_LV_H)
#define SYS_DBG(...)					DEBUG("",__VA_ARGS__)
#define SYS_DBGN(...)					DEBUGN("",__VA_ARGS__)
#else
#define SYS_DBG(...)  
#define SYS_DBGN(...)	
#endif
#if (CONFIG_DEBUG_UART_EN >= CONFIG_DEBUG_LV_M)
#define SYS_LOG(...)					DEBUG("[SYS]     ",__VA_ARGS__)
#else
#define SYS_LOG(...)  
#endif
#if (CONFIG_DEBUG_UART_EN >= CONFIG_DEBUG_LV_L)
#define SYS_ERR(...)					DEBUG("[SYS_ERR] ",__VA_ARGS__)
#else
#define SYS_ERR(...)  
#endif


/*�ж�����==================================================================*/
#if CONFIG_USB_EN
#define CONFIG_NVIC_USB_WakeUp			0
#define CONFIG_NVIC_USB_LP				1
#endif //CONFIG_USB_EN
#if CONFIG_DEBUG_DMA
#define CONFIG_NVIC_DEBUG_DMA			2
#endif

#endif //__CONFIG_H__
