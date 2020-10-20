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
	//u8 IPRADDR=NVIC_Channel/4;  //ÿ��ֻ�ܴ�4��,�õ����ַ 
	//u8 IPROFFSET=NVIC_Channel%4;//�����ڵ�ƫ��
	//IPROFFSET=IPROFFSET*8+4;    //�õ�ƫ�Ƶ�ȷ��λ��
	//MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
	//temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	//temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	//temp&=0xf;//ȡ����λ

	//if(NVIC_Channel<32)NVIC->ISER[0]|=1<<NVIC_Channel;//ʹ���ж�λ(Ҫ����Ļ�,�෴������OK)
	//else NVIC->ISER[1]|=1<<(NVIC_Channel-32);    
	//NVIC->IPR[IPRADDR]|=temp<<IPROFFSET;//������Ӧ���ȼ����������ȼ�   	    	  				   
//}

//��ʱ��3�жϷ������ 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
		
		Angle_Calculate();
		Kalman_Filter(Angle_ax,Gyro_y);
		//Average();
		Psn_Calcu();	
		PWM_Calcu();

	}				 
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}

void PWM_INT(void)
{
	RCC->APB2ENR|=1<<2;      //ʹ��PORTAʱ��
	 RCC->APB2ENR|=1<<4;      //ʹ��PORTCʱ��
   GPIOA->CRH&=0XFFF00FFF;  //PA11��PA12���ó��������
   GPIOA->CRH|=0X00033000; 	
   GPIOA->ODR|=3<<11;		//PA11��PA12����	
	
	 GPIOC->CRL&=0XFF00FFFF;  //Pc4��Pc5���ó��������
   GPIOC->CRL|=0X00330000; 	
   GPIOC->ODR|=3<<4;		//PC4��PC5����	
}
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APBΪ36M
//arr:�Զ���װֵ
//psc:ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3
void TIM3_Int_Init(u16 arr,u16 psc)//
{
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ�� 

 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ1ms  
	TIM3->PSC=psc;  	//Ԥ��Ƶ��7200�õ�10khz�ļ���ʱ��	  
	TIM3->DIER|=1<<0;   //��������ж� 
    TIM3->DIER|=1<<6;   //�������ж�
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
    MY_NVIC_Init(2,3,TIM3_IRQn,2);//��ռ1�������ȼ�3����2
}

void TIM2_PWM_Init(u16 arr,u16 psc)//Ŀ����PA0,PA2 ch1,ch3
{
		
		RCC->APB2ENR|=1<<2;      
   RCC->APB1ENR|=1<<0;      
		GPIOA->CRL &= 0xFFFFF0F0;
	

	
		GPIOA->CRL |= 0x00000A0A;	// �ٶ�2m����������
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

   


