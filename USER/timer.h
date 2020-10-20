#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define LWC_1 PAout(11) //控制左轮端口
#define LWC_2 PAout(12) //控制左轮端口
#define RWC_1 PCout(4) //控制右轮端口
#define RWC_2 PCout(5) //控制右轮端口
//#define RWCE PAout(0) //控制右轮端口使能
//#define LWCE PAout(1) //控制右轮端口使能
#define PWM_CL TIM2->CCR1 //左轮PWM占空比(PA0)
#define PWM_CR TIM2->CCR3 //右轮PWM占空比(PA2)
extern u16 time1,time2;
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void PWM_INT(void);
#endif




