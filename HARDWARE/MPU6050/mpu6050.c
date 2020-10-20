#include "mpu6050.h"
#include "usart.h"
#include "myiic.h"
#include "math.h"
#include "delay.h"




float Gyro_y;        //Y�������������ݴ�
float Angle_gy;      //�ɽ��ٶȼ������б�Ƕ�
float Accel_x;	     //X����ٶ�ֵ�ݴ�
float Angle_ax;      //�ɼ��ٶȼ������б�Ƕ�
float zero=1100;


 
//IICдһ���ֽ� 
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
void MPU_Write_Byte(u8 reg_address,u8 reg_data) 				 
{ 
  IIC_Start(); //��ʼ�ź�
	IIC_Send_Byte(0XD0+0);//����������ַ+д����
	IIC_Wait_Ack();
  IIC_Send_Byte(reg_address);	//д�Ĵ�����ַ
	IIC_Wait_Ack();//�ȴ�Ӧ��
	IIC_Send_Byte(reg_data);//��������
	IIC_Wait_Ack();//�ȴ�Ӧ��
 	IIC_Stop();	
	
}
//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 MPU_Read_Byte(u8 reg_address)
{
	 u8 res_data;
   IIC_Start(); 
	 IIC_Send_Byte(0XD0+0);//����������ַ+д����	
	 IIC_Wait_Ack();		//�ȴ�Ӧ�� 
   IIC_Send_Byte(reg_address);	//д�Ĵ�����ַ
   IIC_Wait_Ack();		//�ȴ�Ӧ��
   IIC_Start();
	 IIC_Send_Byte(0XD0+1);//����������ַ+������	
   IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	 res_data=IIC_Read_Byte(0);//��ȡ����,����nACK 
   IIC_Stop();			//����һ��ֹͣ���� 
	return res_data;		
}


//**************************************
//��ʼ��MPU6050
//**************************************
void MPU6050_Init(void)
{
    IIC_Init();
	  delay_ms(10);//��ʱ10ms
    MPU_Write_Byte(PWR_MGMT_1, 0x00);        //�������״̬
    MPU_Write_Byte(SMPLRT_DIV, 0x07);
    MPU_Write_Byte(CONFIG, 0x06);
    MPU_Write_Byte(GYRO_CONFIG, 0x18);
    MPU_Write_Byte(ACCEL_CONFIG, 0x00);
		
  
 }
 

short  GetData(u8 REG_Address)//��ȡ6050����
{
    u8 H_data,L_data;
    H_data=MPU_Read_Byte(REG_Address);
    L_data=MPU_Read_Byte(REG_Address+1);
    return (H_data<<8)+L_data;   //�ϳ�����
}
void Angle_Calculate()
{

//------���ٶ�--------------------------

	//��ΧΪ2gʱ�������ϵ��16384 LSB/g
	//�ǶȽ�Сʱ��x=sinx�õ��Ƕȣ����ȣ�, deg = rad*180/3.14
	//��Ϊx>=sinx,�ʳ���1.3�ʵ��Ŵ�

	Accel_x  = GetData(ACCEL_XOUT_H);	  //��ȡX����ٶ�
	Angle_ax = (Accel_x-zero) /16384;   //ȥ�����ƫ��,����õ��Ƕȣ����ȣ�	 //798.5	   //16384
	//Angle_ax = Angle_ax*1.2*180/3.14;     //����ת��Ϊ��,
	Angle_ax = Angle_ax*1.2*180/3.14;     //����ת��Ϊ��,


    //-------���ٶ�-------------------------

	//��ΧΪ2000deg/sʱ�������ϵ��16.4 LSB/(deg/s)

	Gyro_y = GetData(GYRO_YOUT_H)-50;	      //��ֹʱ���ٶ�Y�����Ϊ-30����
	Gyro_y = -(Gyro_y)/44;         //ȥ�����ƫ�ƣ�������ٶ�ֵ,����Ϊ������ 
}
