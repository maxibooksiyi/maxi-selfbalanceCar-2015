#include "encoder.h"
// t4
//PB6------------t4ch1
//PB7------------t4ch2

 float speed;
 float position;
void encoder_init(void)
{
	RCC->APB2ENR|=1<<3;    //PBʱ��ʹ��	   
	RCC->APB2ENR|=1<<0;    //��������ʱ��							  

	GPIOB->CRL &= 0x00FFFFFF;// PB6 PB7
	GPIOB->CRL |= 0x44000000;// ��������

// enable the timer4 power
	RCC->APB1ENR |= (1<<2);
// timer4
	
	// reset the control register
	TIM4->CR1 = 0;
	TIM4->CR2 = 0;
	TIM4->SMCR = 0;
	TIM4->DIER = 0;
	TIM4->SR = 0;
	TIM4->EGR = 0;
	TIM4->CCMR2 = 0;
	TIM4->DCR = 0;	// DMA control register
	TIM4->DMAR = 0;


	// set the control register
	TIM4->SMCR = 1;//encode mode

	TIM4->CNT = 0;
	TIM4->PSC = 0;
//	TIM4->ARR = 0xffff;
	TIM4->ARR = 0xffff;
// start the timer
	TIM4->CR1 = 1;
}




int encoder_read(void)
{
	return (int)((s16)(TIM4->CNT));
}

void encoder_write(int data)
{
	TIM4->CNT = data;
}

void Psn_Calcu(void)	 
{

	speed = speed*0.7+encoder_read()*0.3;;		            
	encoder_write(0);							// ������ֵ������Ϊ0
	position += speed;	              
	position += speed_need;
	if(position<-6000) position = -6000;//6000
	if(position> 6000) position =  6000;//6000

	 
}



