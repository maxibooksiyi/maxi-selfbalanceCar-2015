#include "remote.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//����ң�ؽ������� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
int  turn_need;
int  speed_need; 

void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 ; 


//����ң�س�ʼ��
//����IO�Լ���ʱ��5�����벶��
void Remote_Init(void)    			  
{									   
	RCC->APB1ENR|=1<<3;   	//TIM5 ʱ��ʹ�� 
	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ�� 

	GPIOA->CRL&=0XFFFFFF0F;	//PA1 ����  
	GPIOA->CRL|=0X00000080;	//��������     
	GPIOA->ODR|=1<<1;		//PA1 ����
	  
 	TIM5->ARR=10000;  		//�趨�������Զ���װֵ ���10ms���  
	TIM5->PSC=71;  			//Ԥ��Ƶ��,1M�ļ���Ƶ��,1us��1.	 

	TIM5->CCMR1|=1<<8;		//CC2S=01 	ѡ������� IC2ӳ�䵽TI2��
 	TIM5->CCMR1|=3<<12;  	//IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
 	TIM5->CCMR1|=0<<10;  	//IC2PS=00 	���������Ƶ,����Ƶ 

	TIM5->CCER|=0<<5; 		//CC2P=0	�����ز���
	TIM5->CCER|=1<<4; 		//CC2E=1 	���������������ֵ������Ĵ�����
	TIM5->DIER|=1<<2;   	//����CC2IE�����ж�				
	TIM5->DIER|=1<<0;   	//���������ж�				
	TIM5->CR1|=0x01;    	//ʹ�ܶ�ʱ��5
 MY_NVIC_Init(1,3,TIM5_IRQn,2);//��ռ1�������ȼ�3����2		
 turn_need=0;
 speed_need=0;	
}

//ң��������״̬
//[7]:�յ����������־
//[6]:�õ���һ��������������Ϣ
//[5]:����	
//[4]:����������Ƿ��Ѿ�������								   
//[3:0]:�����ʱ��
u8 	RmtSta=0;	  	  
u16 Dval;		//�½���ʱ��������ֵ
u32 RmtRec=0;	//������յ�������	   		    
u8  RmtCnt=0;	//�������µĴ���	  
//��ʱ��5�жϷ������


	 
void TIM5_IRQHandler(void)
{ 		    	 
	u16 tsr;
	tsr=TIM5->SR;
	if(tsr&0X01)//���
	{
		if(RmtSta&0x80)//�ϴ������ݱ����յ���
		{	
			RmtSta&=~0X10;						//ȡ���������Ѿ���������
			if((RmtSta&0X0F)==0X00)RmtSta|=1<<6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
			if((RmtSta&0X0F)<14)RmtSta++;
			else
			{
				RmtSta&=~(1<<7);//���������ʶ
				RmtSta&=0XF0;	//��ռ�����	
			}						 	   	
		}							    
	}
	if(tsr&(1<<2))//CC2IE�ж�
	{	  
		if(RDATA)//�����ز���
		{
  			TIM5->CCER|=1<<5; 				//CC2P=1	����Ϊ�½��ز���
			TIM5->CNT=0;					//��ն�ʱ��ֵ
			RmtSta|=0X10;					//����������Ѿ�������
		}else //�½��ز���
		{
			Dval=TIM5->CCR2;				//��ȡCCR1Ҳ������CC2IF��־λ
  			TIM5->CCER&=~(1<<5);			//CC2P=0	����Ϊ�����ز���
			if(RmtSta&0X10)					//���һ�θߵ�ƽ���� 
			{
 				if(RmtSta&0X80)//���յ���������
				{
					
					if(Dval>300&&Dval<800)			//560Ϊ��׼ֵ,560us
					{
						RmtRec<<=1;	//����һλ.
						RmtRec|=0;	//���յ�0	   
					}else if(Dval>1400&&Dval<1800)	//1680Ϊ��׼ֵ,1680us
					{
						RmtRec<<=1;	//����һλ.
						RmtRec|=1;	//���յ�1
					}else if(Dval>2200&&Dval<2600)	//�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
					{
						RmtCnt++; 		//������������1��
						RmtSta&=0XF0;	//��ռ�ʱ��		
					}
 				}else if(Dval>4200&&Dval<4700)		//4500Ϊ��׼ֵ4.5ms
				{
					RmtSta|=1<<7;	//��ǳɹ����յ���������
					RmtCnt=0;		//�����������������
				}						 
			}
			RmtSta&=~(1<<4);
		}				 		     	    					   
	}
	TIM5->SR=0;//����жϱ�־λ 	    
}

//�����������
//����ֵ:
//	 0,û���κΰ�������
//����,���µİ�����ֵ.
u8 Remote_Scan(void)
{        
	u8 sta=0;       
    u8 t1,t2;  
	if(RmtSta&(1<<6))//�õ�һ��������������Ϣ��
	{ 
	    t1=RmtRec>>24;			//�õ���ַ��
	    t2=(RmtRec>>16)&0xff;	//�õ���ַ���� 
 	    if((t1==(u8)~t2)&&t1==REMOTE_ID)//����ң��ʶ����(ID)����ַ 
	    { 
	        t1=RmtRec>>8;
	        t2=RmtRec; 	
	        if(t1==(u8)~t2)sta=t1;//��ֵ��ȷ	 
		}   
		if((sta==0)||((RmtSta&0X80)==0))//�������ݴ���/ң���Ѿ�û�а�����
		{
		 	RmtSta&=~(1<<6);//������յ���Ч������ʶ
			RmtCnt=0;		//�����������������
		}
	}  
    return sta;
}



void red_con(void)
{
		u8 key;
	key=Remote_Scan();
	switch(key)
	{
			case UP   : speed_need=100 ;  break;
			case DOWN : speed_need=-100;  break;
			case LEFT :	turn_need =3;  break;
			case RIGHT:	turn_need=-3; break; 
			case REST :turn_need=0;speed_need=0;break;
		default :break;
	}
}




























