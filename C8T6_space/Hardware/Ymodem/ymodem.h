/* *****************************************************
*		ymodem.h
*		Ymodem�����ļ�Э��ͷ�ļ�
*		��������:	20190930
*		����:	baymaxPP
* ***************************************************** */
#ifndef __YMODEM_H__
#define __YMODEM_H__

//stm32
#include "stm32f10x.h"
//sys
#include "sys.h"
#include "usart.h"


//-----------�궨��-------------
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define FILE_NAME_LENGTH        (256)
#define FILE_SIZE_LENGTH        (16)

#define NAK_TIMEOUT             (0x10000)

#define YMODEM_C                ("C") 
#define SOH                     (0x01)  //128�ֽ����ݰ���ʼ
#define STX                     (0x02)  //1024�ֽڵ����ݰ���ʼ
#define EOT                     (0x04)  //��������
#define ACK                     (0x06)  //��Ӧ
#define NAK                     (0x15)  //û��Ӧ

//------------------��������----------------
/* *********************************
*	void Ymodem_Handle(void)
*	show:	Ymodem�����ļ�Э�鴦����
*	input:	not
*	output:	not
* ********************************* */
void Ymodem_Handle(void);

#endif

