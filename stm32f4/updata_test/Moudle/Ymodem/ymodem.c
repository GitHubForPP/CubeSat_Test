/* *****************************************************
*		�ļ�����		ymodem.c
*		���ã�			Ymodem�����ļ�Э��
*		��������:		2019-09-30
*		����:				��Сƽ
* ***************************************************** */

/******************* ͷ�ļ� ****************/
#include "ymodem.h"

/******************* �������� ****************/


/******************* �������� **************/

/**
	* ������:		void vYmodem_Init(void)
	* ����:			Ymodem��ʼ��
	* ����:			��
	*	���:			��
	*/
void vYmodem_Init(void) {
	
}

/**
	* ������:		int8_t prv_cYmodem_TimeOut(uint32_t t)
	* ����:			Ymodem�������ݳ�ʱ����
	* ����:			t->��ʱ��ֵ��׼
	*	���:			0->�������������
	*						-1->��ʱ��������
	*/
int8_t prv_cYmodem_TimeOut(uint32_t t) {
	uint16_t old_cnt, new_cnt, in_cnt;
	uint32_t time_out = t;
	
	new_cnt = uiUpdata_Usart_Get_Num();
	in_cnt = new_cnt;
	old_cnt = new_cnt;
	
	while (time_out > 0) {
		new_cnt = uiUpdata_Usart_Get_Num();
		if (new_cnt != old_cnt) {
			time_out = NAK_TIMEOUT;
			old_cnt = new_cnt;
		}
		time_out = time_out-1;
	}
	
	if ((in_cnt == new_cnt) && (new_cnt == 0)) {	//��ʱ��������
		return (-1);
	}
	
	return 0;
}

/**
	* ������:		void prv_vYmodem_Clear_Buff(void) 
	* ����:			Ymodem������ջ���
	* ����:			��
	*	���:			��
	*/
void prv_vYmodem_Clear_Buff(void) {
	vUpdata_Usart_Clear_Buff();
}

/**
	* ������:		void prv_vYmodem_Send_Byte(uint8_t byte)
	* ����:			Ymodem����һ���ֽ�
	* ����:			byte->�������ֽ�
	*	���:			��
	*/
void prv_vYmodem_Send_Byte(uint8_t byte) {
	vUpdata_Usart_SendByte(byte);
}

/**
	* ������:		uint16_t prv_uiYmodem_Get_Buff(uint8_t *data)
	* ����:			Ymodem���ջ�������
	* ����:			*data->�����ջ��������׵�ַ
	*	���:			���ջ������ݳ���
	*/
uint16_t prv_uiYmodem_Get_Buff(uint8_t *data) {
	uint16_t packet_length = 0;
	
	packet_length = uiUpdata_Usart_Get_Buff(data);
	return (packet_length);
}

/**
	* ������:		int8_t prv_cYmodem_Get_File_Info(_YMODEM_MSG_INFO_STR *str)
	* ����:			Ymodem���ս����ļ���Ϣ����
	* ����:			str->Ymodem�ļ���Ϣ�ṹ��
	*	���:			0->�ļ���Ϣ��ȷ
	*						-1->�ļ���Ϣ����
	*/
int8_t prv_cYmodem_Get_File_Info(_YMODEM_MSG_INFO_STR *str) {
	#ifdef YMODEM_RTOS_FLAG
	uint8_t *packet_data = pvPortMalloc(1314), *file_size = pvPortMalloc(FILE_SIZE_LENGTH);
	#else
	uint8_t *packet_data = (uint8_t *)(YMODEM_DATA_SAVE_SRAM_ADDR), file_size[FILE_SIZE_LENGTH];
	#endif
	uint8_t *file_ptr;
	uint16_t packet_length, i;
	
	str->ul_ymodem_file_size = 0;
	prv_vYmodem_Clear_Buff();	//��ս��ջ���
	prv_vYmodem_Send_Byte(YMODEM_C);	//���Ϳ�ʼYmodem���ݴ���
	
	//�ȴ����ݷ���
	if (prv_cYmodem_TimeOut(NAK_TIMEOUT) != 0) {		
		//��ʱ�˳� �ǵ��ͷ�������ڴ�
		//printf("no data\r\n");
		#ifdef YMODEM_RTOS_FLAG
		vPortFree(packet_data); vPortFree(file_size);
		#endif		
		return (-1);
	}
	
	//��ȡ�ļ���Ϣ֡
	packet_length = prv_uiYmodem_Get_Buff(packet_data);
	if (packet_length == (PACKET_SIZE+PACKET_OVERHEAD)) {	//��ȷ����
		if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
				#ifdef YMODEM_RTOS_FLAG
				vPortFree(packet_data); vPortFree(file_size); 
				#endif
        return (-1);
    }
		//todo ���CRCУ���ж�
		//�ļ������ݰ�
		if (packet_data[PACKET_HEADER] != 0) {
				//�ļ������ݰ���Ч��������
				for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
				{
						str->file_name[i++] = *file_ptr++;
				}
				str->file_name[i++] = '\0';
				for (i = 0, file_ptr++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
				{
						file_size[i++] = *file_ptr++;
				}
				file_size[i++] = '\0';
				sscanf((char *)file_size, "%d", &str->ul_ymodem_file_size);
				
				printf("size %d\r\n", str->ul_ymodem_file_size);
				
				prv_vYmodem_Clear_Buff();
				
				prv_vYmodem_Send_Byte(ACK);
				prv_vYmodem_Send_Byte(YMODEM_C);
		} else {	//�ļ������ݰ��գ���������
				prv_vYmodem_Send_Byte(ACK);
				printf("error name\r\n");
		}
	}else {
		printf("error first\r\n");
		#ifdef YMODEM_RTOS_FLAG
		vPortFree(packet_data); vPortFree(file_size); 
		#endif
		return (-1);
	}
	
	return 0;
}

/**
	* ������:		int8_t vYmodem_Handle(_YMODEM_MSG_INFO_STR *str)
	* ����:			Ymodem�����ļ�Э�鴦����
	* ����:			str->Ymodem�ļ���Ϣ�ṹ��
	*	���:			0->�����ļ��ɹ�
	*						-1->�����ļ�ʧ��
	*/
int8_t vYmodem_Handle(_YMODEM_MSG_INFO_STR *str) {
	#ifdef YMODEM_RTOS_FLAG
	uint8_t *packet_data = pvPortMalloc(1314);
	#else
	uint8_t packet_data[1314] = {0};
	#endif
	uint8_t stop_flag = 0;
	uint16_t packet_length, i, write_size;
	uint32_t ul_ymodem_write_cnt = 0;
	
	//�ж��ļ���Ϣ�Ƿ���ս����ɹ�
	if (prv_cYmodem_Get_File_Info(str) != 0) {
		#ifdef YMODEM_RTOS_FLAG
		vPortFree(packet_data); vPortFree(file_size); 
		#endif
		return (-1);
	}
	
	ul_ymodem_write_cnt = 0;
	
	//ѭ�������ļ�����
	while (prv_cYmodem_TimeOut(NAK_TIMEOUT) == 0) {
		packet_length = prv_uiYmodem_Get_Buff(packet_data);
		prv_vYmodem_Clear_Buff();
		//printf("%x\r\n", packet_data[0]);
		switch (packet_data[0]) {
			case SOH: 
			case STX: {
					if (stop_flag) {
						//����
						stop_flag = 0;
						prv_vYmodem_Send_Byte(ACK); 
						break;
					}
					//����֡У��
					if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
							#ifdef YMODEM_RTOS_FLAG
							vPortFree(packet_data); vPortFree(file_size); 
							#endif
							return (-1);
					}
					//�ж�����֡����
					write_size = (packet_data[0] == SOH)?PACKET_SIZE:PACKET_1K_SIZE;
					
					//��������
					for (i = 0; i < write_size; ++i) {
						if (ul_ymodem_write_cnt > str->ul_ymodem_file_size) break;
						*(uint8_t *)(YMODEM_DATA_SAVE_SRAM_ADDR+ul_ymodem_write_cnt) = packet_data[PACKET_HEADER+i];
						ul_ymodem_write_cnt++;
					}
					prv_vYmodem_Send_Byte(ACK);
				}break;
			case EOT: {
					if (stop_flag == 0) {
						stop_flag = 1;
						//��һ��EOT����NACK
						prv_vYmodem_Send_Byte(NAK);
					}else {
						//�ڶ���EOT����ACK
						prv_vYmodem_Send_Byte(ACK);
						prv_vYmodem_Send_Byte(YMODEM_C);
						str->crc_value = uiCrc_16((uint8_t *)(YMODEM_DATA_SAVE_SRAM_ADDR), str->ul_ymodem_file_size);
					}
					
				}break;
			default: break;
		}
	}
	
	#ifdef YMODEM_RTOS_FLAG
	vPortFree(packet_data); vPortFree(file_size); 
	#endif
	
	return 0;
}
