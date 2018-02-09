#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "stm32f10x.h"                  // Device header

/*宏定义====================================================================*/
#define CONFIG_HW_VERSION				0x0100	//硬件版本号
#define CONFIG_SW_VERSION				0x0100	//软件版本号


/*宏定义（请不要修改本区域内容）============================================*/
//打印等级
#define CONFIG_DEBUG_LV_H				3		//打印DEBUG + LOG + ERR
#define CONFIG_DEBUG_LV_M				2		//打印LOG + ERR
#define CONFIG_DEBUG_LV_L				1		//打印ERR
#define CONFIG_DEBUG_LV_N				0		//关闭打印






/*功能配置==================================================================*/
#define CONFIG_DEBUG_UART_EN 			CONFIG_DEBUG_LV_H				//DEBUG串口
#define CONFIG_LED_EN					1								//LED灯






/*DEBUG串口配置=============================================================*/
#if CONFIG_DEBUG_UART_EN
#include "stdio.h"
#define DEBUG(fmt,...) do{printf("%s ",fmt);printf(__VA_ARGS__);}while(0)
#else
#define DEBUG(fmt,...)
#endif //CONFIG_DEBUG_UART_EN

#if CONFIG_DEBUG_UART_EN
//USB_DEBUG配置
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
#define DEBUG_UART 						1								//串口号
#define DEBUG_UART_BAUDRATE				460800							//串口波特率
#endif //CONFIG_DEBUG_UART_EN

/*LED配置===================================================================*/
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
