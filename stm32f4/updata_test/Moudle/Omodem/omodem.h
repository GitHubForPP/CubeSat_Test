/* *****************************************************
*		文件名：		omodem.h
*		作用：			我的omodem在轨文件协议
*		创建日期:		2019-11-08
*		作者:				王小平
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

/******************* 宏定义 ****************/
#define OMODEM_NAK_TIMEOUT             (0x10000)	//0x10000

/******************* 结构体定义 ****************/


//------------------函数定义----------------
void vOmodem_Init(void);
int8_t vOmodem_Handle(uint8_t *data);

#endif

