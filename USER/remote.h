#ifndef __REMOTE_H
#define __REMOTE_H 
#include "sys.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32������
//����ң�ؽ������� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
  
#define RDATA 	PAin(1)	 	//�������������

//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0      		   
#define UP    98
#define DOWN  168
#define LEFT  34
#define RIGHT 194
#define REST  2
extern u8 RmtCnt;			//�������µĴ���
extern int  turn_need;
extern int speed_need;

void Remote_Init(void);    	//���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);	  
void red_con(void);
#endif















