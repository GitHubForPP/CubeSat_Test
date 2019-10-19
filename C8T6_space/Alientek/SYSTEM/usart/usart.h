#ifndef __USART_H__
#define __USART_H__

#include "stdio.h"	
#include "sys.h" 
#include "string.h"

//////////////////////////////////////////////////////////////////////////////////	 
//------------------宏定义--------------------------
#define USART_1_REC_LEN_MAX  			1248  	//定义最大接收字节数 200

//---------------函数定义----------------------
/* *********************************
*	void uart_init(u32 bound)
*	show:	设置串口初始化
*	input:	bound:波特率
*	output:	not
* ********************************* */
void uart_init(u32 bound);

/* *********************************
*	void Uart1_Clean_Receive_Buff(void)
*	show:	清空UART1接收缓存数组
*	input:	not
*	output:	not
* ********************************* */
void Uart1_Clean_Receive_Buff(void);

/* *********************************
*	uint16_t Uart1_Get_Receive_Buff(uint8_t *buff)
*	show:	读取UART1接收缓存数组
*	input:	*buff->获取缓存数据数组首地址
*	output:	UART1接收数据长度
* ********************************* */
uint16_t Uart1_Get_Receive_Buff(uint8_t *buff);

/* *********************************
*	uint16_t Uart1_Get_Receive_Len(void)
*	show:	读取UART1接收缓存长度
*	input:	not
*	output:	UART1接收数据长度
* ********************************* */
uint16_t Uart1_Get_Receive_Len(void);

#endif


