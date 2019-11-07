/* *****************************************************
*	文件名: 				bsp_updata_usart.h
*	作用: 					地面升级串口操作文件
*	创建文件时间:		2019/10/21
*	作者:						王小平
* ***************************************************** */
#ifndef __BSP_UPDATA_USART_H__
#define __BSP_UPDATA_USART_H__

/******************* 头文件 ****************/
/* STM头文件 */
#include "stm32f4xx.h"
#include "string.h"

/******************* 函数声明 ****************/
void vUpdata_USART_Config(void);
void vUpdata_Usart_SendByte(uint8_t ch);
uint16_t uiUpdata_Usart_Get_Buff(uint8_t *data);
uint16_t uiUpdata_Usart_Get_Num(void);
void vUpdata_Usart_Clear_Buff(void);

#endif

