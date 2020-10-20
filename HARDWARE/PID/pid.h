#ifndef __PID_H
#define __PID_H
#include "sys.h"
#include "timer.h"
#include "mpu6050.h"
#include "filter.h"
#include "lcd.h"
#include "encoder.h"
void PWM_Calcu(void);

void motor_con(int pwm_l,int pwm_r);

#endif




