/* *****************************************************
*		ymodem.h
*		Ymodem串口文件协议头文件
*		创建日期:	20190930
*		作者:	王小平
* ***************************************************** */
#ifndef __YMODEM_H__
#define __YMODEM_H__

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
//stm32
#include "stm32f4xx.h"
//bsp
#include "bsp_updata_usart.h"


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

//------------------函数定义----------------
void vYmodem_Init(void);
void vYmodem_Handle(void);

#endif

