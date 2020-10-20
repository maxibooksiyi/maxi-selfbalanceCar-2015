#include "timer.h" 
#include "pid.h"
#include "mpu6050.h"
#include "sys.h"
u16 time1,time2;

void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group); 

void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);
//void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
//{ 
	//u32 temp;	
	//u8 IPRADDR=NVIC_Channel/4;  //每组只能存4个,得到组地址 
	//u8 IPROFFSET=NVIC_Channel%4;//在组内的偏移
	//IPROFFSET=IPROFFSET*8+4;    //得到偏移的确切位置
	//MY_NVIC_PriorityGroupConfig(NVIC_Group);//设置分组
	//temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	//temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	//temp&=0xf;//取低四位

	//if(NVIC_Channel<32)NVIC->ISER[0]|=1<<NVIC_Channel;//使能中断位(要清除的话,相反操作就OK)
	//else NVIC->ISER[1]|=1<<(NVIC_Channel-32);    
	//NVIC->IPR[IPRADDR]|=temp<<IPROFFSET;//设置响应优先级和抢断优先级   	    	  				   
//}

//定时器3中断服务程序 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		
		Angle_Calculate();
		Kalman_Filter(Angle_ax,Gyro_y);
		//Average();
		Psn_Calcu();	
		PWM_Calcu();

	}				 
	TIM3->SR&=~(1<<0);//清楚中断标志位 	    
}

void PWM_INT(void)
{
	RCC->APB2ENR|=1<<2;      //使能PORTA时钟
	 RCC->APB2ENR|=1<<4;      //使能PORTC时钟
   GPIOA->CRH&=0XFFF00FFF;  //PA11，PA12设置成推挽输出
   GPIOA->CRH|=0X00033000; 	
   GPIOA->ODR|=3<<11;		//PA11，PA12上拉	
	
	 GPIOC->CRL&=0XFF00FFFF;  //Pc4，Pc5设置成推挽输出
   GPIOC->CRL|=0X00330000; 	
   GPIOC->ODR|=3<<4;		//PC4，PC5上拉	
}
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB为36M
//arr:自动重装值
//psc:时钟预分频数
//这里使用的是定时器3
void TIM3_Int_Init(u16 arr,u16 psc)//
{
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能 

 	TIM3->ARR=arr;  	//设定计数器自动重装值1ms  
	TIM3->PSC=psc;  	//预分频器7200得到10khz的计数时钟	  
	TIM3->DIER|=1<<0;   //允许更新中断 
    TIM3->DIER|=1<<6;   //允许触发中断
	TIM3->CR1|=0x01;    //使能定时器3
    MY_NVIC_Init(2,3,TIM3_IRQn,2);//抢占1，子优先级3，组2
}

void TIM2_PWM_Init(u16 arr,u16 psc)//目的是PA0,PA2 ch1,ch3
{
		
		RCC->APB2ENR|=1<<2;      
   RCC->APB1ENR|=1<<0;      
		GPIOA->CRL &= 0xFFFFF0F0;
	

	
		GPIOA->CRL |= 0x00000A0A;	// 速度2m，复用推挽
  	    TIM2->CR1 = 0;
		TIM2->CR2 = 0;
		TIM2->SMCR = 0;
		TIM2->DIER = 0;
		TIM2->SR = 0;
		TIM2->EGR = 0;
		
		TIM2->DCR = 0;
		TIM2->DMAR = 0;
		TIM2->CCMR1 =6<<4 ; // set ch1 
		TIM2->CCMR2 =6<<4 ; // set ch3 mode.
		TIM2->CCER = ( 1 | 1<<8 );	// enable ch1 & ch3 output.
		TIM2->CNT = 0;
		TIM2->ARR=arr;		
		TIM2->PSC=psc;		
		PWM_CL=0;    
		PWM_CR=0;	
   TIM2->CR1=1;   
       
}

   


