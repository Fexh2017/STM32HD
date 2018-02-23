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
#define CONFIG_USB_EN					1								//USB






/*DEBUG��������=============================================================*/
#if CONFIG_DEBUG_UART_EN
#include "stdio.h"
#define DEBUG(fmt,...) do{printf("%s ",fmt);printf(__VA_ARGS__);}while(0)
#else
#define DEBUG(fmt,...)
#endif //CONFIG_DEBUG_UART_EN

#if CONFIG_DEBUG_UART_EN
//SYS_DEBUG����
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

/*USB����===================================================================*/
#if CONFIG_USB_EN
#define CONFIG_USB_DEBUG_EN				CONFIG_DEBUG_LV_H			//USB_DEBUGʹ��
#define CONFIG_USB_PWR_EN				1								//USB ʹ�ܽ�
#define CONFIG_USB_HID_EN				0								//USB_HIDʹ��
#define CONFIG_USB_MSD_EN				0								//USB_MSDʹ��

//USB��������
#define CONFIG_USB_CNTR					CNTR_SUSPM | CNTR_RESETM | CNTR_WKUPM
#define CONFIG_USB_EP_NUM				1								//EndPoint����
#define CONFIG_USB_MAX_PACKET_SIZE 		64								//��������
#define CONFIG_USB_EP0_RXADDR        	(0x18)							//EP0 rx buffer base address
#define CONFIG_USB_EP0_TXADDR        	(0x58)							//EP0 tx buffer base address
#define CONFIG_USB_EP1_TXADDR			(0x98)							//EP1 tx buffer base address

#if CONFIG_USB_DEBUG_EN
//USB_DEBUG����
#if (CONFIG_USB_DEBUG_EN >= CONFIG_DEBUG_LV_H)
#define USB_DBG(...)					DEBUG("",__VA_ARGS__)
#else
#define USB_DBG(...)  
#endif
#if (CONFIG_USB_DEBUG_EN >= CONFIG_DEBUG_LV_M)
#define USB_LOG(...)					DEBUG("\n[USB]     ",__VA_ARGS__)
#else
#define USB_LOG(...)  
#endif
#if (CONFIG_USB_DEBUG_EN >= CONFIG_DEBUG_LV_L)
#define USB_ERR(...)					DEBUG("\n[USB_ERR] ",__VA_ARGS__)
#else
#define USB_ERR(...)  
#endif
#endif //CONFIG_USB_DEBUG_EN

#if CONFIG_USB_PWR_EN
//USB ʹ�ܽ�����
#define CONFIG_USB_PWR_CLK				e_RCC_GPIOC
#define CONFIG_USB_PWR_PORT				GPIOC
#define CONFIG_USB_PWR_PIN				GPIO_Pin_15
#endif //CONFIG_USB_PWR_EN

#if CONFIG_USB_HID_EN
//USB HID����
#endif //CONFIG_USB_HID_EN

#if CONFIG_USB_MSD_EN
//USB MSD����
#endif //CONFIG_USB_MSD_EN

#endif //CONFIG_USB_EN

#endif //__CONFIG_H__
