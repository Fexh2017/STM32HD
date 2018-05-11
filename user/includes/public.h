#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include "stm32f10x.h"                  // Device header
#include "config.h"

#define SYSTICK_US 		1000 //(1~233000) us
#define SYSTIMER_COUNT	5

//��������
extern void system_init(void);											//ϵͳ��ʼ��
extern u32 sys_timer_add(void (*fun)(void* arg), void* arg, u32 ms);	//����ϵͳ��ʱ���¼�
extern u32 sys_timer_del(u32 id);										//ɾ��ϵͳ��ʱ���¼�
extern void sys_timer_loop(void);										//ϵͳ��ʱ��
extern u32 sys_get_tick(void);											//ϵͳ��ʱ��72M��
extern u32 sys_get_ms(void);											//ϵͳ��ʱ�����룩
extern u32 sys_get_hour(void);											//ϵͳ��ʱ��Сʱ��
extern u32 sys_get_time_after(u32 ms);									//ϵͳ��ʱ����ȥ���ٺ��룩
extern void delayms(u32 tim);											//��ʱ-����
extern void delay(u16 tim);												//��ʱ-�Զ���
extern s32 limiter(s32 num,s32 min,s32 max);							//�޷�-����
extern double flimiter(double num,double min,double max);				//�޷�-����
extern u16 ToWord(u8 bytehigh, u8 bytelow);
extern u16 ByteSwap(u16 wSwW);
#endif
