/* 引用的头文件（'#include'）                                                  */
/******************************************************************************/
#include "delay.h"	//延时控制API
#include "usart.h"	//串口打印（printf函数）API

#define IIC_PORT			GPIOB				//指定MCU的IIC设备的端口号
#define IIC_SCL_PIN		GPIO_Pin_6	//指定IIC设备的SCL引脚号
#define IIC_SDA_PIN		GPIO_Pin_7	//指定IIC设备的SDA引脚号



/******************************************************************************/
/* 全局变量声明 ('extern'、'static'、'volatile'、'const')                      */
/******************************************************************************/



/******************************************************************************/
/* 局部变量声明 ('static'、'volatile'、'const')                                */
/******************************************************************************/



/******************************************************************************/
/* 全局函数声明 ('extern')                                                     */
/******************************************************************************/
extern void SDA_DATA(Low_High_te high_low);	//设置SDA引脚输出高/低电平
extern void SCL_OUT(Low_High_te high_low);	//设置SCL引脚输出高/低电平

//IIC所有操作函数
extern void IIC_PinConfig(void);		//初始化IIC的IO口
extern void IIC_Start(void);				//发送IIC开始信号
extern void IIC_Stop(void);					//发送IIC停止信号
extern u8 IIC_Wait_Ack(void); 			//IIC等待ACK信号
extern void IIC_Ack(void);					//IIC发送ACK信号
extern void IIC_NAck(void);					//IIC不发送ACK信号

extern void IIC_Send_Byte(u8 txd);	//IIC主设备发送一个字节数据
extern u16 IIC_Read_Byte(u8 ack);		//IIC主设备读取一个字节数据 

#endif


/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
