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
#define DEBUG(fmt,...) 		do{u32 __systime_h = Get_Syshour(); \
							   u32 __systime_l = Get_Systick(); \
							   printf("\n%02d:%02d:%02d.%03d %s ", __systime_h, (__systime_l * SYSTICK_US / 60000000), (__systime_l * SYSTICK_US % 60000000 / 1000000), (__systime_l * SYSTICK_US % 1000000 / 1000) ,fmt);printf(__VA_ARGS__);}while(0)
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
#define DEV_DBG(...)					DEBUG("",__VA_ARGS__)
#define DEV_DBGN(...)					DEBUGN("",__VA_ARGS__)
#else
#define SYS_DBG(...)  
#endif
#if (CONFIG_DEBUG_UART_EN >= CONFIG_DEBUG_LV_M)
#define SYS_LOG(...)					DEBUG("[SYS]     ",__VA_ARGS__)
#define DEV_LOG(...)					DEBUG("[DEV]     ",__VA_ARGS__)
#else
#define SYS_LOG(...)  
#endif
#if (CONFIG_DEBUG_UART_EN >= CONFIG_DEBUG_LV_L)
#define SYS_ERR(...)					DEBUG("[SYS_ERR] ",__VA_ARGS__)
#define DEV_ERR(...)					DEBUG("[DEV_ERR] ",__VA_ARGS__)
#else
#define SYS_ERR(...)  
#endif






/*LED����===================================================================*/
#if CONFIG_LED_EN
//LED0
#define LED_RCC_0						e_RCC_GPIOA
#define LED_PIN_0						GPIO_PA_08
//LED1
#define LED_RCC_1						e_RCC_GPIOD
#define LED_PIN_1						GPIO_PA_02
#endif //CONFIG_LED_EN


/*USB����===================================================================*/
#if CONFIG_USB_EN
#define CONFIG_USB_DEBUG_EN				CONFIG_DEBUG_LV_M			//USB_DEBUGʹ��
#define CONFIG_USB_PWR_EN				1								//USB ʹ�ܽ�

#if CONFIG_USB_DEBUG_EN
//USB_DEBUG����
#if (CONFIG_USB_DEBUG_EN >= CONFIG_DEBUG_LV_H)
#define USB_DBG(...)					DEBUG("[USB DBG] ",__VA_ARGS__)
#define USB_DBGN(...)					DEBUGN("",__VA_ARGS__)
#else
#define USB_DBGN(...)
#define USB_DBG(...)  
#endif
#if (CONFIG_USB_DEBUG_EN >= CONFIG_DEBUG_LV_M)
#define USB_LOG(...)					DEBUG("[USB]     ",__VA_ARGS__)
#else
#define USB_LOG(...)  
#endif
#if (CONFIG_USB_DEBUG_EN >= CONFIG_DEBUG_LV_L)
#define USB_ERR(...)					DEBUG("[USB_ERR] ",__VA_ARGS__)
#else
#define USB_ERR(...)  
#endif
#endif //CONFIG_USB_DEBUG_EN

#if CONFIG_USB_PWR_EN
//USB ʹ�ܽ�����
#define CONFIG_USB_PWR_CLK				e_RCC_GPIOC
#define CONFIG_USB_PWR_PIN				GPIO_PC_15
#endif //CONFIG_USB_PWR_EN

#endif //CONFIG_USB_EN


/*�ж�����==================================================================*/
#if CONFIG_USB_EN
#define CONFIG_NVIC_USB_WakeUp			0
#define CONFIG_NVIC_USB_LP				1
#endif //CONFIG_USB_EN
#if CONFIG_DEBUG_DMA
#define CONFIG_NVIC_DEBUG_DMA			2
#endif

#endif //__CONFIG_H__
