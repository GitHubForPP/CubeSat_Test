/* *****************************************************
*		�ļ�����		omodem.h
*		���ã�			�ҵ�omodem�ڹ��ļ�Э��
*		��������:		2019-11-08
*		����:				��Сƽ
* ***************************************************** */
#ifndef __OMODEM_H__
#define __OMODEM_H__

//stm32
#include "stm32f4xx.h"
//bsp
#include "bsp_updata_usart.h"
#include "bsp_sdram.h" 
//moudle
#include "crc.h"

/******************* �궨�� ****************/
#define OMODEM_NAK_TIMEOUT             (0x10000)	//0x10000

/******************* �ṹ�嶨�� ****************/


//------------------��������----------------
void vOmodem_Init(void);
int8_t vOmodem_Handle(uint8_t *data);

#endif

