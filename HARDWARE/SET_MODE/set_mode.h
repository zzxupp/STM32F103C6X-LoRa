#ifndef __MODE_H
#define __MODE_H	
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//����
#define MD0 PAout(4)// PA4
#define MD1 PAout(5)// PA5

////�ҵ�
//#define MD0 PAout(5)// PA5
//#define MD1 PAout(4)// PA4


void SET_MODE_Init(void);//��ʼ��
void Config_Mode(void);
void Transmit_Mode(void);	 				    
#endif

