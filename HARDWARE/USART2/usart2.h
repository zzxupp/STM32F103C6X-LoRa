/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   UART配置H文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#ifndef __DRV_UART_H__
#define __DRV_UART_H__


#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

//串口硬件定义
#define UART_PORT					USART2
#define UART_PORT_CLK				RCC_APB1Periph_USART2
#define UART_PORT_AF				GPIO_Remap_USART2

//串口引脚定义
#define UART_TX_GPIO_PORT			GPIOA
#define UART_TX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define UART_TX_GPIO_PIN			GPIO_Pin_2

#define UART_RX_GPIO_PORT			GPIOA
#define UART_RX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define UART_RX_GPIO_PIN			GPIO_Pin_3


#define USART2_REC_LEN  			200  	//定义最大接收字节数 200

extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	

void drv_uart_init( uint32_t UartBaudRate );
void drv_uart_tx_bytes( uint8_t* TxBuffer, uint8_t Length );
uint8_t drv_uart_rx_bytes( uint8_t* RxBuffer );
void ASxx_tx_packet( uint8_t Addr_H, uint8_t Addr_L, uint8_t Channel, uint8_t *pTxBuff, uint8_t Length );
#endif



