#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include "stm32f10x.h"                  // Device header
#include "config.h"

//��������
extern void delayus(u32 tim);																//��ʱ-΢��
extern void delayms(u32 tim);																//��ʱ-����
extern void delay(u16 tim);																	//��ʱ-�Զ���
extern s32 limiter(s32 num,s32 min,s32 max);												//�޷�-����
extern double flimiter(double num,double min,double max);									//�޷�-����


#endif
