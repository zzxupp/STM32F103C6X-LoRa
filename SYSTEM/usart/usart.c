//=============================================================================
//	Project			:	sht30
//	Version			:	V1.0
//	File				:	usart.c
//	Author			:	eonegh
//	Date				:	2023-04-20
//	Controller	:	STM32F103C6T6
//	IDE					:	Keil uVision5 V5.36.0.0
//	Compiler		:	eonegh
//	Brief				:	����debug��printf������API
//=============================================================================

/******************************************************************************/
/* ���õ�ͷ�ļ���'#include'��                                                  */
/******************************************************************************/
#include "usart.h"	//����debug��printf������API



/******************************************************************************/
/* ���ر����Ķ��� ('static')                                                   */
/******************************************************************************/



/******************************************************************************/
/* ȫ�ֱ����Ķ��� ('extern'��'static'��'volatile'��'const')                    */
/******************************************************************************/
u8	USART1_RX_BUF[USART1_REC_LEN] = {0};	//���ջ��壬���USART1_REC_LEN���ֽڣ�������������ʶ��0x0D��0x0A
u16	USART1_RX_STA = 0;										//����״̬��ǡ�bit15��������ɱ�־��bit14�����յ�0x0d��bit13~0�����յ�����Ч�ֽ���Ŀ



/******************************************************************************/
/* ����ԭ�ͣ�ȫ�� ('extern') ������ ('static')                                 */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief  ֧��printf����,������Ҫѡ��use MicroLIB
 **
 ** \param  ��
 **
 ** \retval ��
 **
 ******************************************************************************
**/
#if 1
#pragma import(__use_no_semihosting)

struct __FILE 											//��׼����Ҫ��֧�ֺ���
{ 
	int handle;
};

FILE __stdout;


void _sys_exit(int x)								//����_sys_exit()�Ա���ʹ�ð�����ģʽ
{ 
	x = x;
}


int fputc(int ch, FILE *f)					//�ض���fputc����
{
	while(0 == (USART1->SR&0X40)){;}	//ѭ������,ֱ���������
		USART1->DR = (u8)ch;
	return ch;
}
#endif


/**
 ******************************************************************************
 ** \brief  ����1��ȡֵ1��2�ȣ����������á�
 **					���ں�x1��Ӧ���ض����ţ�����APBx2_peripheral(RCC_APB2Periph_GPIOA)Ҳ��
 **					�ض���Ӧ��ϵ�������stm32f10x_rcc.h�ļ�L492��L559��
 **
 ** \param  ��
 **
 ** \retval ��
 **
 ******************************************************************************
**/
void USART1_PinConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;										//����һ��GPIO������ṹ�壬���ڶ��������������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//GPIO�˿�ʱ�ӣ�����/ʹ��GPIOAʱ��
	
	
//USARTx1_TX
	GPIO_InitStructure.GPIO_Pin		=	USART1_TXD_PIN;				//GPIO���ţ�			ָ������1��TXD����
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;				//GPIO����ʱ�ӣ�	IO��ʱ���ٶ�Ϊ50MHz��ģ������ģʽʱ���붨������ʱ�ӣ�
	GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_AF_PP;				//GPIO���������	����ʽ���ù��ܣ���������ߵ�ƽ����������͵�ƽ��	
	GPIO_Init(USART1_PORT,&GPIO_InitStructure);						//GPIO������Ч��	�����趨������ʼ���˿ڼ�TXD����
	
	
//USARTx1_RX
	GPIO_InitStructure.GPIO_Pin = USART1_RXD_PIN;					//GPIO���ţ�			ָ������1��RXD����
//	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_50MHz;				//GPIO����ʱ�ӣ�	IO��ʱ���ٶ�Ϊ50MHz��ģ������ģʽʱ���붨������ʱ�ӣ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//GPIO���������	���븡��	
	GPIO_Init(USART1_PORT, &GPIO_InitStructure);						//GPIO������Ч��	�����趨������ʼ���˿ڼ�RXD����
}


/**
 ******************************************************************************
 ** \brief  ����1��ȡֵ1��2��3�ȣ�������������ж����á�
 **					���ں�x1��Ӧ���ض����ţ�����APBx2_peripheral(RCC_APBx2Periph_GPIOA)Ҳ��
 **					�ض���Ӧ��ϵ�������stm32f10x_rcc.h�ļ�L492��L559��
 **
** \param  bound�����ڲ����ʣ�ͨ��ȡֵ9600��115200
 **
 ** \retval ��
 **
 ******************************************************************************
**/
void USART1_Init(u32 bound)
{
	USART_InitTypeDef USART_InitStructure;										//����һ��USART������ṹ�壬���ڶ��������������
	NVIC_InitTypeDef 	NVIC_InitStructure;											//����һ��NVIC���жϿ�������������ṹ�壬���ڶ��������������

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);		//ʹ��USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//�������ڸ��ù��ܣ�Ҫ��������ʱ��
	
	
//����USART1�Ľṹ��
	USART_DeInit(USART1);																														//���USART1������ΪĬ��ֵ
	USART_InitStructure.USART_BaudRate = bound;																			//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;															//����żУ��λ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//�շ�ģʽ		
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_Init(USART1, &USART_InitStructure);																				//��ʼ��USARTx

	
//�����жϿ������Ľṹ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//�����жϿ�����������ռ���飨���main()���������У���ע�ʹ˴���
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				//USART1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	USART_ClearFlag(USART1, USART_FLAG_TXE);								//�����ж�֮ǰ�������һ���жϱ�־λ
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					//����USART1�����ж�
	USART_ITConfig(USART1, USART_IT_TC, DISABLE);						//�ر�USART1�����жϡ��мǣ���ʼ��ʱ����ʹ�ܴ��ڷ����жϣ�����ᵼ�³�������ʱһֱ�ڽ������ж�


	USART_Cmd(USART1, ENABLE);															//ʹ��USART1
}


/**
 ******************************************************************************
 ** \brief  ����1��ȡֵ1��2�ȣ����жϷ���������ν��յ���ʶ��0x0d��0x0a��ʾ����֡�������
 **
 ** \param  ��
 **
 ** \retval ��
 **
 ******************************************************************************
**/
void USART1_IRQHandler(void)
{
	u8 Res;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//�жϽ�����ʶ��0x0D��0x0A
	{
		Res = USART_ReceiveData(USART1);						//��ȡ���յ�������
		if(0 == (USART1_RX_STA & 0x8000))						//����δ���
		{
			if(USART1_RX_STA & 0x4000)								//��һ�ν��յ��˽�����ʶ��0x0D
			{
				if(Res != 0x0A)	USART1_RX_STA=0;				//���մ������¿�ʼ
				else USART1_RX_STA |= 0x8000;						//���������
			}
			else //��һ��δ�յ�0x0D
			{	
				if(0x0D == Res)	USART1_RX_STA |= 0x4000;				//��Ǳ��ν��յ���0x0D
				else
				{
					USART1_RX_BUF[USART1_RX_STA & 0X3FFF]	=	Res;								//�洢���ν��յ������ݣ�����֡���Ȳ�����0x3FFF
					USART1_RX_STA++;
					if(USART1_RX_STA > (USART1_REC_LEN-1))	USART1_RX_STA = 0;	//�ѽ������ݳ��ȳ������ƣ������������ݣ����¿�ʼ����
				}
			}
		}
  } 
}


/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
