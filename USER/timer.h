#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define LWC_1 PAout(11) //�������ֶ˿�
#define LWC_2 PAout(12) //�������ֶ˿�
#define RWC_1 PCout(4) //�������ֶ˿�
#define RWC_2 PCout(5) //�������ֶ˿�
//#define RWCE PAout(0) //�������ֶ˿�ʹ��
//#define LWCE PAout(1) //�������ֶ˿�ʹ��
#define PWM_CL TIM2->CCR1 //����PWMռ�ձ�(PA0)
#define PWM_CR TIM2->CCR3 //����PWMռ�ձ�(PA2)
extern u16 time1,time2;
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void PWM_INT(void);
#endif




