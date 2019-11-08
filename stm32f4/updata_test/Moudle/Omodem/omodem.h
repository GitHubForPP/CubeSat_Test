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
#define OMODEM_NAK_TIMEOUT             		(0x10000)	//0x10000

#define OMODEM_PACKET_SIZE								(100)
#define OMODEM_PACKET_FRAME_ID_SIZE				(8)
#define OMODEM_PACKET_FRAME_DATA_NUM			(1)
#define OMODEM_PACKET_START_ADDR_LEN			(4)
#define OMODEM_PACKET_DATA_LEN_INFO_LEN		(1)
#define OMODEM_PACKET_ONE_DATA_INFO_LEN 	(OMODEM_PACKET_START_ADDR_LEN + OMODEM_PACKET_DATA_LEN_INFO_LEN)
#define OMODEM_PACKET_DATA_SIZE						(OMODEM_PACKET_SIZE - OMODEM_PACKET_FRAME_ID_SIZE - OMODEM_PACKET_ONE_DATA_INFO_LEN - OMODEM_PACKET_FRAME_DATA_NUM)

#define OMODEM_PACKET_NOW_ID_IDX					(0)
#define OMODEM_PACKET_ALL_ID_IDX					(4)

/******************* �ṹ�嶨�� ****************/
typedef struct __OMODEM_DATA_STR{
	uint32_t now_id;
	uint32_t all_id;
	uint8_t  sync_num;
	uint32_t start_addr[16];
	uint8_t  sync_len[16];
	uint8_t  sync_buff[16][OMODEM_PACKET_DATA_SIZE];
}_OMODEM_DATA_STR;									//��Ϣ�ṹ��

//------------------��������----------------
void vOmodem_Init(void);
int8_t cOmodem_Handle(_OMODEM_DATA_STR *str);

#endif

