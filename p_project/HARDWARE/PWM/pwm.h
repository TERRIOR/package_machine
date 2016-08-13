
#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
static u32 step=0;
double GetMotorstep(void);
void SetMotorstep(double i);
void TIM14_TIME_Init(u32 psc);
void TIM13_TIME_Init(u32 psc);
void TIM9_TIME_Init(u32 psc);
void TIM14_Configuration(u32 period);
void TIM13_Configuration(u32 period);
void TIM9_Configuration(u32 period);
#endif

