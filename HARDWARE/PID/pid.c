#include "pid.h"
#include "remote.h"
	
float Kp=58.5;//平衡时2.8	210			 250/249.7								160有点感觉了。
float Kd=2.6  ;//平衡时0.31	 10			 20/19.65								10  
float Ki=0.1;//平衡时0.09
float Kn=0.009;//平衡时0.0015
float PWM_R;     
float PWM_L;     
float PWM; 
//int  turn_need=0;
//int  speed_need;         

u8 ii;
 
void PWM_Calcu(void)
{	


	PWM = Angle*Kp+Gyro_y*Kd;
	PWM+=speed*Ki+position*Kn;
	
    PWM_R = PWM+turn_need;
	PWM_L = PWM-turn_need;							 
	//PWM_R = PWM;
	//PWM_L = PWM;
	motor_con(PWM_L,PWM_R); 
	//PWM_CL=PWM;
	//	PWM_CR=PWM;
	
		
}
void motor_con(int pwm_l,int pwm_r)
{
		if(pwm_l<0)
		{
				LWC_1=1;
				LWC_2=0;
			pwm_l=-pwm_l;
				//RWC_1=1;
			//	RWC_2=0;


		   //pwm_r=-pwm_r;
		}
		else
		{
				LWC_1=0;
				LWC_2=1;
			//	RWC_1=0;
			//	RWC_2=1;
				}
		
		if(pwm_r<0)
		{
				RWC_1=1;
				RWC_2=0;
			pwm_r=-pwm_r;
			//pwm_l=-pwm_l+20;
		}
	  
		else
		{
				RWC_1=0;
				RWC_2=1;
		}
		if(pwm_l>255)pwm_l=255;
		if(pwm_r>255)pwm_r=255;
		PWM_CL=0+pwm_l;
		PWM_CR=0+pwm_r;


}









