/* *****************************************************
*		�ļ�����		mmodem.h
*		���ã�			mmodem�����ļ�Э��
*		��������:		2019-11-06
*		����:				��Сƽ
* ***************************************************** */
#ifndef __MMODEM_H__
#define __MMODEM_H__

//stm32
#include "stm32f4xx.h"
//bsp
#include "bsp_updata_usart.h"
#include "bsp_sdram.h" 
//moudle
#include "crc.h"

/*
�ο���YmodemЭ��

INFO: SOH 0x00 0xFF file_name file_size start_addr [NULL] CRCH CRCL (128+5Byte)
DATA: STX 0x01 0xFE 							[data(1024)] 						CRCH CRCL (1024+5Byte)
ACK->0x06 NACK->0x15	EOT->0x04	SOH->0x01	STX->0x02

		PC							 Device
				<<<<<<<<<<< 	"C"
	 INFO >>>>>>>>>>>
				<<<<<<<<<<<	  ACK
	 DATA >>>>>>>>>>>
				<<<<<<<<<<<	  ACK
				...........
	 EOT  >>>>>>>>>>>			
*/

//-----------�궨��-------------
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define FILE_NAME_LENGTH        (50)
#define FILE_SIZE_LENGTH        (16)
#define START_ADDR_LENGTH       (11)

#define NAK_TIMEOUT             (0x10000)	//0x10000

#define MMODEM_C                ('C') 
#define SOH                     (0x01)  //128�ֽ����ݰ���ʼ
#define STX                     (0x02)  //1024�ֽڵ����ݰ���ʼ
#define EOT                     (0x04)  //��������
#define ACK                     (0x06)  //��Ӧ
#define NAK                     (0x15)  //û��Ӧ

/******************* �ṹ�嶨�� ****************/
typedef struct __MMODEM_MSG_INFO_STR{
	uint8_t file_name[FILE_NAME_LENGTH];
	uint32_t ul_file_size;
	uint32_t ul_start_addr;
	uint16_t crc_value;
}_MMODEM_MSG_INFO_STR;									//Ymodem�ļ���Ϣ�ṹ��

//------------------��������----------------
void vMmodem_Init(void);
int8_t vMmodem_Handle(_MMODEM_MSG_INFO_STR *str, uint8_t *data);

#endif

