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
//ALIENTEK战舰STM32开发板实验29
//加速度传感器 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 	 
//x,y:开始显示的坐标位置
//num:要显示的数据
//mode:0,显示加速度值;1,显示角度值;
//x,y:开始显示的坐标位置
//num:要显示的数据
//mode:0,显示加速度值;1,显示角度值;
void Adxl_Show_Num(u16 x,u16 y,short num,u8 mode)
{
	if(mode==0)	//显示加速度值
	{
		if(num<0)
		{
			LCD_ShowChar(x,y,'-',16,0);		//显示负号
			num=-num;						//转为正数
		}else LCD_ShowChar(x,y,' ',16,0);	//去掉负号
		LCD_ShowNum(x+8,y,num,4,16);		//显示值		    
 	}else 		//显示角度值
	{
		if(num<0)
		{
			LCD_ShowChar(x,y,'-',16,0);		//显示负号
			num=-num;						//转为正数
		}else LCD_ShowChar(x,y,' ',16,0);	//去掉负号
 		LCD_ShowNum(x+8,y,num/10,2,16);		//显示整数部分		    
		LCD_ShowChar(x+24,y,'.',16,0);		//显示小数点
		LCD_ShowNum(x+32,y,num%10,1,16); 	//显示小数部分		    
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
		//unsigned char TxBuf1[100] ={"发送字符串！！!\r\n"};
     //int  i;
	//float Kp=2.5;//平衡时2.8
    //float Kd=0.31;//平衡时0.31
	u8 *str;
	u8 key;	   
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600
 	LED_Init();			     //LED端口初始化	这里面最关键的事那几个IO端口的初始化，不初始化时不行的！
	LCD_Init();	
	KEY_Init();	
	PWM_INT(); 	
	Remote_Init();

	//下面是串口的发送调试

     //SystemInit();
     //usart_debug_config();
	//IO口初始化操作
  // GPIO_InitTypeDef  GPIO_InitStructure;
 	
 //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PE端口时钟
	
 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //LED0-->PB.5 端口配置
 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 //GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 //GPIO_SetBits(GPIOB,GPIO_Pin_12);						 //PB.5 输出高

 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PE.5 端口配置, 推挽输出
 //GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 //GPIO_SetBits(GPIOE,GPIO_Pin_4); 						 //PE.5 输出高 

	//PAout(11)=1 ;//控制左轮
    //PAout(12)=0; //控制左轮端口
    //PCout(4)=1; //控制右轮端口
    //oPCout(5)=0; //控制右轮端口M 
	TIM3_Int_Init(99,7199);	//10ms进入定时
	//TIM2_PWM_Init(255,0);//不分频72M，周期为255
	TIM2_PWM_Init(255,0);		 
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,10,200,16,16,"MAXI");	
	//LCD_ShowString(60,70,200,16,16,"3D TEST");	
	LCD_ShowString(60,30,200,16,16,"201212268031");
	LCD_ShowString(60,50,200,16,16,"2015/5/23");
 	//LCD_ShowString(60,130,200,16,16,"KEY0:Auto Adjust");
	MPU6050_Init();
	
 	while(1)	//3D加速度传感器初始化	
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
	POINT_COLOR=BLUE;//设置字体为红色 
	while(1)
	{		  	    
 		//if(t%10==0)//每100ms读取一次
		if(t%1==0)
		{
			//得到X,Y,Z轴的加速度值(原始值)
			//ADXL345_Read_Average(&x,&y,&z,10);	//读取X,Y,Z三个方向的加速度值 
			//Adxl_Show_Num(60+48,170,x,0);		//显示加速度原始值
			//Adxl_Show_Num(60+48,190,y,0);
			//Adxl_Show_Num(60+48,210,z,0);
 			////得到角度值,并显示
			//angx=ADXL345_Get_Angle(x,y,z,1);    
			//angy=ADXL345_Get_Angle(x,y,z,2);   
			//angz=ADXL345_Get_Angle(x,y,z,0); 
			//Adxl_Show_Num(60+48,230,angx,1);		//显示角度值
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
			
		   	Accel_x  = GetData(ACCEL_XOUT_H);	  //读取X轴加速度
	        Angle_ax = (Accel_x-zero) /16384;   //去除零点偏移,计算得到角度（弧度）	 //798.5		   //16384
	        //Angle_ax = Angle_ax*1.2*180/3.14;     //弧度转换为度,
			Angle_ax = Angle_ax*1.2*180/3.14;     //弧度转换为度,

			Adxl_Show_Num(48,230,Angle_ax,0);
			Accel_y  = GetData(ACCEL_YOUT_H);	  //读取X轴加速度
	        //Angle_ay = (Accel_y-zero) /16384;   //去除零点偏移,计算得到角度（弧度）	 //798.5
	        //Angle_ay = Angle_ay*1.2*180/3.14;     //弧度转换为度,
			Accel_z  = GetData(ACCEL_ZOUT_H);	  //读取X轴加速度
	        //Angle_az = (Accel_z-zero) /16384;   //去除零点偏移,计算得到角度（弧度）	 //798.5
	        //Angle_az = Angle_az*1.2*180/3.14;     //弧度转换为度,
	        Gyro_y= GetData(GYRO_YOUT_H)-50;	      //静止时角速度Y轴输出为-30左右
	        Gyro_y = -(Gyro_y)/44;         //去除零点偏移，计算角速度值,负号为方向处理 
			//Angle_gy+=Gyro_y*0.1;
			Adxl_Show_Num(60+48,230,Gyro_y,0);
			//Adxl_Show_Num(60+48,110,GetData(ACCEL_XOUT_H),0);		//显示加速度原始值
		    Adxl_Show_Num(60+48,110,GetData(ACCEL_XOUT_H),0);
			Adxl_Show_Num(60+48,130,Accel_y,0);
			Adxl_Show_Num(60+48,150,GetData(ACCEL_ZOUT_H),0);
			Adxl_Show_Num(60+48,170,GetData(GYRO_XOUT_H),1);		//显示角度值
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
//while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);//这里跟分析fputc时是一样的
//USART_SendData(USART1 , TxBuf1[i]);//发送字符数组里的单个字符
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
			//LED1=0;//绿灯亮,提示校准中
			//ADXL345_AUTO_Adjust((char*)&x,(char*)&y,(char*)&z);//自动校准
			//LED1=1;//绿灯灭,提示校准完成
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


