/* *****************************************************
*		文件名：		ymodem.h
*		作用：			Ymodem串口文件协议
*		创建日期:		2019-09-30
*		作者:				王小平
* ***************************************************** */
#ifndef __YMODEM_H__
#define __YMODEM_H__

//stm32
#include "stm32f4xx.h"
//bsp
#include "bsp_updata_usart.h"
#include "bsp_sdram.h" 

//-----------宏定义-------------
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define FILE_NAME_LENGTH        (256)
#define FILE_SIZE_LENGTH        (16)

#define NAK_TIMEOUT             (0xF0000)	//0x10000

#define YMODEM_C                ('C') 
#define SOH                     (0x01)  //128字节数据包开始
#define STX                     (0x02)  //1024字节的数据包开始
#define EOT                     (0x04)  //结束传输
#define ACK                     (0x06)  //回应
#define NAK                     (0x15)  //没回应

//#define	YMODEM_RTOS_FLAG		
#ifdef YMODEM_RTOS_FLAG
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#endif

#define	YMODEM_DATA_SAVE_SRAM_ADDR		(SDRAM_BANK_ADDR+0x00700000)


/******************* 结构体定义 ****************/
typedef struct __YMODEM_MSG_INFO_STR{
	uint8_t file_name[FILE_NAME_LENGTH];
	uint32_t ul_ymodem_file_size;
}_YMODEM_MSG_INFO_STR;									//Ymodem文件信息结构体

//------------------函数定义----------------
void vYmodem_Init(void);
int8_t vYmodem_Handle(_YMODEM_MSG_INFO_STR *str);

#endif

