#include "sth_send.h"

void LoRa_SendData(u8 a)
{
	u16 len;	
	float* temhum;                                   //��������tmhum������ʱ������ʪ������
	float data_frame[5];                             //��������data_frame���ڴ洢ÿ�η��͵�����ֵ
	temhum = SHT30_read_result(SHT30_DEVADDR);       //��ʪ�ȴ������ɼ����ݲ�������ֵ��������temhum
		data_frame[0] = 0x00;                          //��һ������Ϊ���
		data_frame[1] = 0x04;                          //�ڶ�������Ϊ��ַ
		data_frame[2] = 0x17;                          //����������ΪУ��λ
		data_frame[3] = temhum[0];                     //���ĸ�����Ϊ�¶�
		data_frame[4] = temhum[1];                     //���������Ϊʪ��
		len = sizeof(data_frame);                      //lenΪ���ݳ���
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	  USART_Send(data_frame,len);
}