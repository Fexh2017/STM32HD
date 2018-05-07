#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include "stm32f10x.h"                  // Device header
#include "config.h"

#define SYSTICK_US 		100 //us

//��������
extern void system_init(void);									//ϵͳ��ʼ��
extern u32 Get_Systick(void);									//ϵͳ��ʱ
extern u32 Get_Syshour(void);									//ϵͳ��ʱ��Сʱ��
extern void delayms(u32 tim);									//��ʱ-����
extern void delay(u16 tim);										//��ʱ-�Զ���
extern s32 limiter(s32 num,s32 min,s32 max);					//�޷�-����
extern double flimiter(double num,double min,double max);		//�޷�-����
extern u16 ToWord(u8 bytehigh, u8 bytelow);
extern u16 ByteSwap(u16 wSwW);
#endif
