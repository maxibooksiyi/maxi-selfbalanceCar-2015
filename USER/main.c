#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "pid.h"
#include "math.h"	 
//#include "adxl345.h"
#include "mpu6050.h" 
#include "filter.h"
#include "remote.h"
//float zero=200; 	 
//ALIENTEKս��STM32������ʵ��29
//���ٶȴ����� ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 	 
//x,y:��ʼ��ʾ������λ��
//num:Ҫ��ʾ������
//mode:0,��ʾ���ٶ�ֵ;1,��ʾ�Ƕ�ֵ;
//x,y:��ʼ��ʾ������λ��
//num:Ҫ��ʾ������
//mode:0,��ʾ���ٶ�ֵ;1,��ʾ�Ƕ�ֵ;
void Adxl_Show_Num(u16 x,u16 y,short num,u8 mode)
{
	if(mode==0)	//��ʾ���ٶ�ֵ
	{
		if(num<0)
		{
			LCD_ShowChar(x,y,'-',16,0);		//��ʾ����
			num=-num;						//תΪ����
		}else LCD_ShowChar(x,y,' ',16,0);	//ȥ������
		LCD_ShowNum(x+8,y,num,4,16);		//��ʾֵ		    
 	}else 		//��ʾ�Ƕ�ֵ
	{
		if(num<0)
		{
			LCD_ShowChar(x,y,'-',16,0);		//��ʾ����
			num=-num;						//תΪ����
		}else LCD_ShowChar(x,y,' ',16,0);	//ȥ������
 		LCD_ShowNum(x+8,y,num/10,2,16);		//��ʾ��������		    
		LCD_ShowChar(x+24,y,'.',16,0);		//��ʾС����
		LCD_ShowNum(x+32,y,num%10,1,16); 	//��ʾС������		    
	}
}


 	
 int main(void)
 {	 
	//u8 key;
	u8 t=0;	
	//short x,y,z;  	    
	//short angx,angy,angz;
	float Accel_x,Accel_y,Accel_z,Angle_ay,Angle_az,Gyro_y;
	float Angle_ax;//Angle_gy ;
    float PWM;
	float speed;
		//unsigned char TxBuf1[100] ={"�����ַ�������!\r\n"};
     //int  i;
	//float Kp=2.5;//ƽ��ʱ2.8
    //float Kd=0.31;//ƽ��ʱ0.31
	u8 *str;
	u8 key;	   
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
 	LED_Init();			     //LED�˿ڳ�ʼ��	��������ؼ������Ǽ���IO�˿ڵĳ�ʼ��������ʼ��ʱ���еģ�
	LCD_Init();	
	KEY_Init();	
	PWM_INT(); 	
	Remote_Init();

	//�����Ǵ��ڵķ��͵���

     //SystemInit();
     //usart_debug_config();
	//IO�ڳ�ʼ������
  // GPIO_InitTypeDef  GPIO_InitStructure;
 	
 //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //LED0-->PB.5 �˿�����
 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 //GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 //GPIO_SetBits(GPIOB,GPIO_Pin_12);						 //PB.5 �����

 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PE.5 �˿�����, �������
 //GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 //GPIO_SetBits(GPIOE,GPIO_Pin_4); 						 //PE.5 ����� 

	//PAout(11)=1 ;//��������
    //PAout(12)=0; //�������ֶ˿�
    //PCout(4)=1; //�������ֶ˿�
    //oPCout(5)=0; //�������ֶ˿�M 
	TIM3_Int_Init(99,7199);	//10ms���붨ʱ
	//TIM2_PWM_Init(255,0);//����Ƶ72M������Ϊ255
	TIM2_PWM_Init(255,0);		 
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,10,200,16,16,"MAXI");	
	//LCD_ShowString(60,70,200,16,16,"3D TEST");	
	LCD_ShowString(60,30,200,16,16,"201212268031");
	LCD_ShowString(60,50,200,16,16,"2015/5/23");
 	//LCD_ShowString(60,130,200,16,16,"KEY0:Auto Adjust");
	MPU6050_Init();
	
 	while(1)	//3D���ٶȴ�������ʼ��	
	{
		//LCD_ShowString(60,150,200,16,16,"ADXL345 Error");
		//delay_ms(200);
		//LCD_Fill(60,150,239,150+16,WHITE);
 		//delay_ms(200);
	//}	
	LCD_ShowString(60,70,200,16,16,"PWM:");							   
	LCD_ShowString(60,90,200,16,16,"MPU6050");
 	LCD_ShowString(60,110,200,16,16,"X VAL:");
	LCD_ShowString(60,130,200,16,16,"Y VAL:");
	LCD_ShowString(60,150,200,16,16,"Z VAL:");
  	LCD_ShowString(60,170,200,16,16,"X ANG:");
	LCD_ShowString(60,190,200,16,16,"Y ANG:");
	LCD_ShowString(60,210,200,16,16,"Z ANG:");
    LCD_ShowString(60,250,200,16,16,"SPEED:");
	LCD_ShowString(35,270,200,16,16,"POSITION:");
	LCD_ShowString(60,290,200,16,16,"KEY V:");
	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	while(1)
	{		  	    
 		//if(t%10==0)//ÿ100ms��ȡһ��
		if(t%1==0)
		{
			//�õ�X,Y,Z��ļ��ٶ�ֵ(ԭʼֵ)
			//ADXL345_Read_Average(&x,&y,&z,10);	//��ȡX,Y,Z��������ļ��ٶ�ֵ 
			//Adxl_Show_Num(60+48,170,x,0);		//��ʾ���ٶ�ԭʼֵ
			//Adxl_Show_Num(60+48,190,y,0);
			//Adxl_Show_Num(60+48,210,z,0);
 			////�õ��Ƕ�ֵ,����ʾ
			//angx=ADXL345_Get_Angle(x,y,z,1);    
			//angy=ADXL345_Get_Angle(x,y,z,2);   
			//angz=ADXL345_Get_Angle(x,y,z,0); 
			//Adxl_Show_Num(60+48,230,angx,1);		//��ʾ�Ƕ�ֵ
			//Adxl_Show_Num(60+48,250,angy,1);
			//Adxl_Show_Num(60+48,270,angz,1);
			
			key=Remote_Scan();
			switch(key)
			{
				case 0:str="ERROR";break;			   
				case 162:str="POWER";break;	    
				case 98:str="UP";break;	    
				case 2:str="PLAY";break;		 												
				case 226:str="ALIENTEK";break;		  
				case 194:str="RIGHT";break;	   
				case 34:str="LEFT";break;		  
				case 224:str="VOL-";break;		  
				case 168:str="DOWN";break;		   
			    case 144:str="VOL+";break;		    
				case 104:str="1";break;		  
				case 152:str="2";break;	   
				case 176:str="3";break;	    
				case 48:str="4";break;		    
				case 24:str="5";break;		    
				case 122:str="6";break;		  
				case 16:str="7";break;			   					
				case 56:str="8";break;	 
				case 90:str="9";break;
				case 66:str="0";break;
				case 82:str="DELETE";break;		 
			}
			
		   	Accel_x  = GetData(ACCEL_XOUT_H);	  //��ȡX����ٶ�
	        Angle_ax = (Accel_x-zero) /16384;   //ȥ�����ƫ��,����õ��Ƕȣ����ȣ�	 //798.5		   //16384
	        //Angle_ax = Angle_ax*1.2*180/3.14;     //����ת��Ϊ��,
			Angle_ax = Angle_ax*1.2*180/3.14;     //����ת��Ϊ��,

			Adxl_Show_Num(48,230,Angle_ax,0);
			Accel_y  = GetData(ACCEL_YOUT_H);	  //��ȡX����ٶ�
	        //Angle_ay = (Accel_y-zero) /16384;   //ȥ�����ƫ��,����õ��Ƕȣ����ȣ�	 //798.5
	        //Angle_ay = Angle_ay*1.2*180/3.14;     //����ת��Ϊ��,
			Accel_z  = GetData(ACCEL_ZOUT_H);	  //��ȡX����ٶ�
	        //Angle_az = (Accel_z-zero) /16384;   //ȥ�����ƫ��,����õ��Ƕȣ����ȣ�	 //798.5
	        //Angle_az = Angle_az*1.2*180/3.14;     //����ת��Ϊ��,
	        Gyro_y= GetData(GYRO_YOUT_H)-50;	      //��ֹʱ���ٶ�Y�����Ϊ-30����
	        Gyro_y = -(Gyro_y)/44;         //ȥ�����ƫ�ƣ�������ٶ�ֵ,����Ϊ������ 
			//Angle_gy+=Gyro_y*0.1;
			Adxl_Show_Num(60+48,230,Gyro_y,0);
			//Adxl_Show_Num(60+48,110,GetData(ACCEL_XOUT_H),0);		//��ʾ���ٶ�ԭʼֵ
		    Adxl_Show_Num(60+48,110,GetData(ACCEL_XOUT_H),0);
			Adxl_Show_Num(60+48,130,Accel_y,0);
			Adxl_Show_Num(60+48,150,GetData(ACCEL_ZOUT_H),0);
			Adxl_Show_Num(60+48,170,GetData(GYRO_XOUT_H),1);		//��ʾ�Ƕ�ֵ
			//Adxl_Show_Num(60+48,190,GetData(GYRO_YOUT_H),1);
			Adxl_Show_Num(60+48,190,Gyro_y,1);
			Adxl_Show_Num(60+48,210,GetData(GYRO_ZOUT_H),1);
			Kalman_Filter(Angle_ax,Gyro_y);
			//Average();

			Adxl_Show_Num(60+108,230,Angle,1);
			//Psn_Calcu();
			//speed = speed*0.7+encoder_read()*0.3;;		            
	        //encoder_write(0);
			//Adxl_Show_Num(60+48,250,,1);
			//Adxl_Show_Num(60+48,290,sta,1);
		    LCD_ShowString(60+48,290,200,16,16,str);


			//for( i = 0; TxBuf1[i] != '\0'; i++)
//{
//while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);//���������fputcʱ��һ����
//USART_SendData(USART1 , TxBuf1[i]);//�����ַ�������ĵ����ַ�
//while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
//}
			//USART_SendData(USART1,0x1111);
			USART_SendData(USART1, 0x03);                                 //C
   while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_SendData(USART1, 0xFC);                                 //C
   while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_SendData(USART1,(int)Angle_ax);                                 //A
   while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);   //B
   USART_SendData(USART1, Gyro_y);                                 //C
   while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
   USART_SendData(USART1,(int)Angle);                                 //C
   while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
   USART_SendData(USART1, 0xFC);                                 //C
   while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
   USART_SendData(USART1, 0x03);                                 //C
   while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		   //Psn_Calcu();	
		   //PWM_Calcu();
		   //PWM_Calcu();
		   ////***********************
		   
		   //PWM = Angle*Kp+Gyro_y*Kd;
		   //Adxl_Show_Num(60+48,70,PWM,0);
		   //PAout(0)=PWM;
		   //PAout(2)=PWM;
		   //motor_con(PWM,PWM);
		}
		//key=KEY_Scan(0);
		//if(key==KEY_UP)
		//{
			//LED1=0;//�̵���,��ʾУ׼��
			//ADXL345_AUTO_Adjust((char*)&x,(char*)&y,(char*)&z);//�Զ�У׼
			//LED1=1;//�̵���,��ʾУ׼���
		//}
	    //red_con();
	 	//delay_ms(10);
		//t++;
		//if(t==20)
		//{
		//	t=0;
		//	LED0=!LED0;
		//}
		//while(1)
		//{
		}
	}

}


