#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stm32f10x.h"                  // Device header

/*�궨��====================================================================*/
#define CONFIG_HW_VERSION				0x0100	//Ӳ���汾��
#define CONFIG_SW_VERSION				0x0100	//����汾��


/*�궨�壨�벻Ҫ�޸ı��������ݣ�============================================*/
//��ӡ�ȼ�
#define CONFIG_DEBUG_LV_H				3		//��ӡDEBUG + LOG + ERR
#define CONFIG_DEBUG_LV_M				2		//��ӡLOG + ERR
#define CONFIG_DEBUG_LV_L				1		//��ӡERR
#define CONFIG_DEBUG_LV_N				0		//�رմ�ӡ






/*��������==================================================================*/
#define CONFIG_DEBUG_UART_EN 			CONFIG_DEBUG_LV_H				//DEBUG����
#define CONFIG_LED_EN					1								//LED��






/*DEBUG��������=============================================================*/
#if CONFIG_DEBUG_UART_EN
#include "stdio.h"
#define DEBUG(fmt,...) do{printf("%s ",fmt);printf(__VA_ARGS__);}while(0)
#else
#define DEBUG(fmt,...)
#endif //CONFIG_DEBUG_UART_EN

#if CONFIG_DEBUG_UART_EN
//USB_DEBUG����
#if (CONFIG_DEBUG_UART_EN >= CONFIG_DEBUG_LV_H)
#define SYS_DBG(...)					DEBUG("",__VA_ARGS__)
#else
#define SYS_DBG(...)  
#endif
#if (CONFIG_DEBUG_UART_EN >= CONFIG_DEBUG_LV_M)
#define SYS_LOG(...)					DEBUG("\n[SYS]     ",__VA_ARGS__)
#else
#define SYS_LOG(...)  
#endif
#if (CONFIG_DEBUG_UART_EN >= CONFIG_DEBUG_LV_L)
#define SYS_ERR(...)					DEBUG("\n[SYS_ERR] ",__VA_ARGS__)
#else
#define SYS_ERR(...)  
#endif
#endif //CONFIG_DEBUG_UART_EN

#if CONFIG_DEBUG_UART_EN
#define DEBUG_UART 						1								//���ں�
#define DEBUG_UART_BAUDRATE				460800							//���ڲ�����
#endif //CONFIG_DEBUG_UART_EN

/*LED����===================================================================*/
#if CONFIG_LED_EN
//LED0
#define LED_RCC_0						e_RCC_GPIOA
#define LED_PORT_0 						GPIOA
#define LED_PIN_0						0x0100
//LED1
#define LED_RCC_1						e_RCC_GPIOD
#define LED_PORT_1						GPIOD
#define LED_PIN_1						0x0004
#endif //CONFIG_LED_EN








#endif //__CONFIG_H__
