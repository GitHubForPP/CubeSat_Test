/* *****************************************************
*	�ļ���: 				bsp_updata_usart.h
*	����: 					�����������ڲ����ļ�
*	�����ļ�ʱ��:		2019/10/21
*	����:						��Сƽ
* ***************************************************** */
#ifndef __BSP_UPDATA_USART_H__
#define __BSP_UPDATA_USART_H__

/******************* ͷ�ļ� ****************/
/* STMͷ�ļ� */
#include "stm32f4xx.h"
#include "string.h"

/******************* �������� ****************/
void vUpdata_USART_Config(void);
void vUpdata_Usart_SendByte(uint8_t ch);
uint16_t uiUpdata_Usart_Get_Buff(uint8_t *data);
uint16_t uiUpdata_Usart_Get_Num(void);
void vUpdata_Usart_Clear_Buff(void);

#endif

