#ifndef __USART_H__
#define __USART_H__

#include "stdio.h"	
#include "sys.h" 
#include "string.h"

//////////////////////////////////////////////////////////////////////////////////	 
//------------------�궨��--------------------------
#define USART_1_REC_LEN_MAX  			1248  	//�����������ֽ��� 200

//---------------��������----------------------
/* *********************************
*	void uart_init(u32 bound)
*	show:	���ô��ڳ�ʼ��
*	input:	bound:������
*	output:	not
* ********************************* */
void uart_init(u32 bound);

/* *********************************
*	void Uart1_Clean_Receive_Buff(void)
*	show:	���UART1���ջ�������
*	input:	not
*	output:	not
* ********************************* */
void Uart1_Clean_Receive_Buff(void);

/* *********************************
*	uint16_t Uart1_Get_Receive_Buff(uint8_t *buff)
*	show:	��ȡUART1���ջ�������
*	input:	*buff->��ȡ�������������׵�ַ
*	output:	UART1�������ݳ���
* ********************************* */
uint16_t Uart1_Get_Receive_Buff(uint8_t *buff);

/* *********************************
*	uint16_t Uart1_Get_Receive_Len(void)
*	show:	��ȡUART1���ջ��泤��
*	input:	not
*	output:	UART1�������ݳ���
* ********************************* */
uint16_t Uart1_Get_Receive_Len(void);

#endif


