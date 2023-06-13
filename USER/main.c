//=============================================================================
//	Project			:	sht30
//	Version			:	V1.0
//	File				:	main.c
//	Author			:	eonegh
//	Date				:	2023-04-20
//	Controller	:	STM32F103C6T6
//	IDE					:	Keil uVision5 V5.36.0.0
//	Compiler		:	eonegh
//	Brief				:	������
//=============================================================================
#include "sht30.h"	//��ʪ�ȴ�����sht30�ɼ�����
#include "usart.h"	//����debug��printf������API
#include "led.h"		//LED����API������debug

//ʵ���LoRa�����ߴ��书�ܵ�ʵ����Ҫ���������ļ�����
#include "usart2.h" //����2ͨ������,��LoRaģ�����ӣ��������߷��ͺͽ��պ���
#include "set_mode.h"//LoRa����ģʽ���ã�����ģʽ�ʹ���ģʽ

//���ݽڵ㲻ͬ���ͣ�ѡ��ͬ�Ķ��壬���統ǰ�Ǹ�֪�ڵ㣬��ע�͵��������д���
#define PERCEPTION 1//���嵱ǰ�ڵ�Ϊ��֪�ڵ�
//#define ROUTER 1//���嵱ǰ�ڵ�Ϊ·�ɽڵ�
//#define GATEWAY 1//���嵱ǰ�ڵ�Ϊ���ؽڵ�

#define SEND_ADDR 0x08//���͸���һ���ڵ�ĵ�ַ����ǰ����һ����ַ��00 0x08 0x17 �ɸ���ʵ���������

#define NUM 4//�������ݰ��ĳ��ȣ��ɸ���ʵ���������
u8 DATA_BUF[NUM] = {0x00};
u8 REC_DATA_BUF[64] = {0x00};//·�ɽڵ���յ���֪�ڵ�����ݴ洢�������������
u8 GPRS_BUF[64] = {0x00};//���ؽڵ���յ���֪�ڵ�����ݴ洢�������������
void buildDATA(void);

int main(void)
{ 
	u8 len = 0;//�������ݵĳ���
	u8 i = 0;
	
	delay_init();				//��ʱ������ʼ��
	
	//ϵͳ�ж���ж���Ӧʱ��Ҫ�������ȼ���������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	
//����1������Ϊ115200������ʹ��printf��������debug
	USART1_PinConfig();
	USART1_Init(115200);
	
//����2��ʼ�����˿�����	
	drv_uart_init( 9600 ); //���ڳ�ʼ��  ����ģ�鲨���ʱ仯
	SET_MODE_Init();//��ʼ��MD0��MD1����
	Transmit_Mode();//����Ϊ����ģʽ
	
	IIC_PinConfig();		//��ʼ��IIC������MCU��SHT30ͨ�ţ�����ʹ�����ģ�ⷽʽʵ��
	LED_PinConfig();		//��ʼ��LED�����ڵ���
  

	
	while(1)
	{
		//��֪�ڵ�ɼ���ʪ�ȣ���ͨ��LoRa�����·�ɽڵ�
		#if PERCEPTION
		if(SHT30_ReadTempHum(SHT30_DEVADDR)){
			printf("Temperature = %5.2fC, Humidity = %5.2f%%RH \r\n",Temperature,Humidity);
			//�ɼ�������ʪ��ת����浽DATA_BUF���棬Ȼ��ͨ��LoRa���ͳ�ȥ
			buildDATA();
			ASxx_tx_packet(0x00,SEND_ADDR,0x17,DATA_BUF,NUM);
		}
		else{
			printf("Temperature or Humidity illegal! \r\n");	//�ɼ�����ʪ�����ݷǷ�������
		} 
		#endif
		
		//·�ɽڵ㣬���ո�֪�ڵ�����ݣ���ת�������ؽڵ�
		#if ROUTER
			if(USART2_RX_STA&0x8000){//������յ���ȷ�����ݣ�USART2_RX_STA&0x8000�����1
				len=USART2_RX_STA&0x3f; //�õ��˴ν��յ������ݳ���
				//��ӡ��ǰ���յ�����
				for(i = 0; i < len; i++){
					printf("USART2_RX_BUF[%0d] == %0d\r\n",i,USART2_RX_BUF[i]);
					REC_DATA_BUF[i] = USART2_RX_BUF[i];//�����յ������ݴ浽REC_DATA_BUF����
				}
				ASxx_tx_packet(00,SEND_ADDR,17,REC_DATA_BUF,len);//Ȼ������ת�������ؽڵ�
				USART2_RX_STA = 0;
			}	
		#endif
		
		//���ؽڵ㣬����·�ɽڵ�����ݣ����ϴ���������
		#if GATEWAY
			if(USART2_RX_STA&0x8000){//������յ���ȷ�����ݣ�USART2_RX_STA&0x8000�����1
				len=USART2_RX_STA&0x3f; //�õ��˴ν��յ������ݳ���
				//��ӡ��ǰ���յ�����
				for(i = 0; i < len; i++){
					printf("USART2_RX_BUF[%0d] == %0d\r\n",i,USART2_RX_BUF[i]);
					GPRS_BUF[i] = USART2_RX_BUF[i];//�����յ������ݴ浽GPRS_BUF����
				}
				
				//������Ҫ�Լ�����һ����룬���ǽ�GPRS_BUF�����ݴ��䵽��������ʵ����������
				
				//
				
				USART2_RX_STA = 0;
			}	
			
		#endif
		LED0 = !LED0;		//LEDÿ����˸һ��,����debug
		
		delay_ms(1000);
	}
	
 return 0;
}

//���ɼ�������ʪ�ȹ�����һ������������֡�������ݴ���
void buildDATA(void){
	u16 Tem_4 = 0;
	u16 Hum_4 = 0;
	
	//Temperature = 28.40C, Humidity = 36.85%RH 
	//1���ɼ�������ʪ����С�������Ǵ��ڴ������u8���͵�����������Ҫ��С��ת����
	Tem_4 = (int) (Temperature*100);
	Hum_4 = (int) (Humidity*100);
	//2��u8���͵���ֵ��С��ΧΪ0-255��2840��Ȼ������Χ�ˣ�����Ҫ���в��
	//�����˼·��������u8���洢���ֱ�洢�߰�λ�͵Ͱ�λ
	//��ֳ��ĸ�8λ����
	
	DATA_BUF[0] = (Tem_4 >> 8) & (0xFF);//�¶ȸ߰�λ
	DATA_BUF[1] = (Tem_4 & 0xFF);//�¶ȵͰ�λ
		
	DATA_BUF[2] = (Hum_4 >> 8) & (0xFF);//ʪ�ȸ߰�λ
	DATA_BUF[3] = (Hum_4 & 0xFF);//ʪ�ȵͰ�λ
	
	//���Ҫ������������ݣ���������������Ӿ���
	//DATA_BUF[4] = xx;
	//
}


/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
