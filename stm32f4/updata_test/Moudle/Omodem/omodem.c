/* *****************************************************
*		�ļ�����		omodem.c
*		���ã�			�ҵ�omodem�ڹ��ļ�Э��
*		��������:		2019-11-08
*		����:				��Сƽ
* ***************************************************** */

/******************* ͷ�ļ� ****************/
#include "omodem.h"

/******************* �������� ****************/


/******************* �������� **************/

/**
	* ������:		void vOmodem_Init(void)
	* ����:			Omodem��ʼ��
	* ����:			��
	*	���:			��
	*/
void vOmodem_Init(void) {
	
}

/**
	* ������:		int8_t prv_cOmodem_TimeOut(uint32_t t)
	* ����:			Omodem�������ݳ�ʱ����
	* ����:			t->��ʱ��ֵ��׼
	*	���:			0->�������������
	*						-1->��ʱ��������
	*/
int8_t prv_cOmodem_TimeOut(uint32_t t) {
	uint16_t old_cnt, new_cnt, in_cnt;
	uint32_t time_out = t;
	
	new_cnt = uiUpdata_Usart_Get_Num();
	in_cnt = new_cnt;
	old_cnt = new_cnt;
	
	while (time_out-- > 0) {
		new_cnt = uiUpdata_Usart_Get_Num();
		if (new_cnt != old_cnt) {
			time_out = t;
			old_cnt = new_cnt;
		}
	}
	
	if ((in_cnt == new_cnt) && (new_cnt == 0)) {	//��ʱ��������
		return (-1);
	}
	
	return 0;
}

/**
	* ������:		void prv_vOmodem_Clear_Buff(void) 
	* ����:			Omodem������ջ���
	* ����:			��
	*	���:			��
	*/
void prv_vOmodem_Clear_Buff(void) {
	vUpdata_Usart_Clear_Buff();
}

/**
	* ������:		void prv_vOmodem_Send_Byte(uint8_t byte)
	* ����:			Omodem����һ���ֽ�
	* ����:			byte->�������ֽ�
	*	���:			��
	*/
void prv_vOmodem_Send_Byte(uint8_t byte) {
	vUpdata_Usart_SendByte(byte);
}

/**
	* ������:		uint16_t prv_uiOmodem_Get_Buff(uint8_t *data)
	* ����:			Omodem���ջ�������
	* ����:			*data->�����ջ��������׵�ַ
	*	���:			���ջ������ݳ���
	*/
uint16_t prv_uiOmodem_Get_Buff(uint8_t *data) {
	uint16_t packet_length = 0;
	
	packet_length = uiUpdata_Usart_Get_Buff(data);
	return (packet_length);
}

/**
	* ������:		int8_t vOmodem_Handle(uint8_t *data)
	* ����:			Omodem�ڹ�Э�鴦����
	*						*data->���������׵�ַ
	*	���:			0->�����ļ��ɹ�
	*						-1->�����ļ�ʧ��
	*/
int8_t vOmodem_Handle(uint8_t *data) {
	uint8_t packet_data[1314] = {0}, cnt = 0, sync_num = 0, len = 0;
	uint8_t i = 0, j = 0;
	uint16_t packet_length;
	int8_t get_data_ok_flag = (-1);
	uint32_t now_idx = 0, all_idx = 0, start_addr = 0;
	
	//ѭ�������ļ�����
	while (prv_cOmodem_TimeOut(OMODEM_NAK_TIMEOUT) == 0) {
		packet_length = prv_uiOmodem_Get_Buff(packet_data);
		prv_vOmodem_Clear_Buff();
		
		printf("packet_length %d\r\n", packet_length);
		
		/* �жϳ��� */
		if (packet_length == 100) {
			memcpy(&now_idx, &packet_data[cnt], 4); cnt+=4;
			memcpy(&all_idx, &packet_data[cnt], 4); cnt+=4;
			sync_num = packet_data[cnt++];
			
			printf("now_idx %x all_idx %x sync_num %d\r\n", now_idx, all_idx, sync_num);
			
			for (i = 0; i < sync_num; ++i) {
				memcpy(&start_addr, &packet_data[cnt], 4); cnt+=4;
				len = packet_data[cnt++];
				printf("start_addr %x len %d\r\n", start_addr, len);
				for (j = 0; j < len; ++j) {
					printf("%x ", packet_data[cnt+j]);
					if ((j%16 == 0) && (j>0)) {
						printf("\r\n");
					}
				}
				printf("\r\n");
				memcpy(&data[start_addr], &packet_data[cnt], len);
				cnt += len;
			}
			
			if (now_idx == all_idx) {
				get_data_ok_flag = 0;
			}
		}
		
	}
	
	return get_data_ok_flag;
}
