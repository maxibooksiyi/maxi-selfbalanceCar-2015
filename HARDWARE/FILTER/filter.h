
#ifndef __FILTER_H
#define __FILTER_H
#include "sys.h"
#include "MPU6050.h"
void Kalman_Filter(float Accel,float Gyro);
//void QingHua_AngleCalaulate(float G_angle,float Gyro);
void Average(void);
extern float Angle;  	  
#endif












