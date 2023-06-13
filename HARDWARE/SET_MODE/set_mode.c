/**
  ***************************************************************************************************
  * @author  lilin
  * @version V1.0
  * @date    2021.10.16
  * @brief   ͨ��IO�ڵ�ƽ�ı仯�����ı�MD0��MD1��ֵ���Ӷ��ı�LoRa�Ĺ���ģʽ
  ***************************************************************************************************
  * @attention
  *һ������˵��
	*	MD0==1,MD1==0----------------------��ģʽΪ����ģʽ
	*   MD0==0,MD1==0----------------------��ģʽΪ����ģʽ
	*   ͨ�����Ĳ�����˸���жϳ����Ƿ��������С�
  ***************************************************************************************************
  */
#include "set_mode.h"

void SET_MODE_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB�˿�ʱ��
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //MOD0-->PA5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.5
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);						 //PA.5 �����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //MOD1-->PA4 �˿�����
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.4
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);						 //PA.4 �����
}
void Config_Mode(){
	MD0 = 0;
	MD1 = 0;
}
void Transmit_Mode(){
	MD0 = 1;
	MD1 = 0;
}
