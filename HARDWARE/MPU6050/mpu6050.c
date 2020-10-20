#include "mpu6050.h"
#include "usart.h"
#include "myiic.h"
#include "math.h"
#include "delay.h"




float Gyro_y;        //Y轴陀螺仪数据暂存
float Angle_gy;      //由角速度计算的倾斜角度
float Accel_x;	     //X轴加速度值暂存
float Angle_ax;      //由加速度计算的倾斜角度
float zero=1100;


 
//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
void MPU_Write_Byte(u8 reg_address,u8 reg_data) 				 
{ 
  IIC_Start(); //开始信号
	IIC_Send_Byte(0XD0+0);//发送器件地址+写命令
	IIC_Wait_Ack();
  IIC_Send_Byte(reg_address);	//写寄存器地址
	IIC_Wait_Ack();//等待应答
	IIC_Send_Byte(reg_data);//发送数据
	IIC_Wait_Ack();//等待应答
 	IIC_Stop();	
	
}
//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
u8 MPU_Read_Byte(u8 reg_address)
{
	 u8 res_data;
   IIC_Start(); 
	 IIC_Send_Byte(0XD0+0);//发送器件地址+写命令	
	 IIC_Wait_Ack();		//等待应答 
   IIC_Send_Byte(reg_address);	//写寄存器地址
   IIC_Wait_Ack();		//等待应答
   IIC_Start();
	 IIC_Send_Byte(0XD0+1);//发送器件地址+读命令	
   IIC_Wait_Ack();		//等待应答 
	 res_data=IIC_Read_Byte(0);//读取数据,发送nACK 
   IIC_Stop();			//产生一个停止条件 
	return res_data;		
}


//**************************************
//初始化MPU6050
//**************************************
void MPU6050_Init(void)
{
    IIC_Init();
	  delay_ms(10);//延时10ms
    MPU_Write_Byte(PWR_MGMT_1, 0x00);        //解除休眠状态
    MPU_Write_Byte(SMPLRT_DIV, 0x07);
    MPU_Write_Byte(CONFIG, 0x06);
    MPU_Write_Byte(GYRO_CONFIG, 0x18);
    MPU_Write_Byte(ACCEL_CONFIG, 0x00);
		
  
 }
 

short  GetData(u8 REG_Address)//读取6050数据
{
    u8 H_data,L_data;
    H_data=MPU_Read_Byte(REG_Address);
    L_data=MPU_Read_Byte(REG_Address+1);
    return (H_data<<8)+L_data;   //合成数据
}
void Angle_Calculate()
{

//------加速度--------------------------

	//范围为2g时，换算关系：16384 LSB/g
	//角度较小时，x=sinx得到角度（弧度）, deg = rad*180/3.14
	//因为x>=sinx,故乘以1.3适当放大

	Accel_x  = GetData(ACCEL_XOUT_H);	  //读取X轴加速度
	Angle_ax = (Accel_x-zero) /16384;   //去除零点偏移,计算得到角度（弧度）	 //798.5	   //16384
	//Angle_ax = Angle_ax*1.2*180/3.14;     //弧度转换为度,
	Angle_ax = Angle_ax*1.2*180/3.14;     //弧度转换为度,


    //-------角速度-------------------------

	//范围为2000deg/s时，换算关系：16.4 LSB/(deg/s)

	Gyro_y = GetData(GYRO_YOUT_H)-50;	      //静止时角速度Y轴输出为-30左右
	Gyro_y = -(Gyro_y)/44;         //去除零点偏移，计算角速度值,负号为方向处理 
}
