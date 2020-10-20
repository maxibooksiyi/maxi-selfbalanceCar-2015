#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "sys.h"
//#include "remote.h"
extern float speed_l;
extern float speed_r;
extern float speed_r_l;
extern float speed;
extern float position;
void encoder_init(void);
int  encoder_read(void);
void encoder_write(int data);
void Psn_Calcu(void);	
#endif
