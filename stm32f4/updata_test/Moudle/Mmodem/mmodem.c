/* *****************************************************
*		�ļ�����		mmodem.c
*		���ã�			�ҵ�modem�����ļ�Э��
*		��������:		2019-11-06
*		����:				��Сƽ
* ***************************************************** */

/******************* ͷ�ļ� ****************/
#include "mmodem.h"

/******************* �������� ****************/


/******************* �������� **************/

/**
	* ������:		void vMmodem_Init(void)
	* ����:			Mmodem��ʼ��
	* ����:			��
	*	���:			��
	*/
void vMmodem_Init(void) {
	
}

/**
	* ������:		int8_t prv_cMmodem_TimeOut(uint32_t t)
	* ����:			Mmodem�������ݳ�ʱ����
	* ����:			t->��ʱ��ֵ��׼
	*	���:			0->�������������
	*						-1->��ʱ��������
	*/
int8_t prv_cMmodem_TimeOut(uint32_t t) {
	uint16_t old_cnt, new_cnt, in_cnt;
	uint32_t time_out = t;
	
	new_cnt = uiUpdata_Usart_Get_Num();
	in_cnt = new_cnt;
	old_cnt = new_cnt;
	
	while (time_out-- > 0) {
		new_cnt = uiUpdata_Usart_Get_Num();
		if (new_cnt != old_cnt) {
			time_out = NAK_TIMEOUT;
			old_cnt = new_cnt;
		}
	}
	
	//�жϳ�ʱ��������
	if ((in_cnt == new_cnt) && (new_cnt == 0)) {	
		return (-1);
	}
	
	return 0;
}

/**
	* ������:		void prv_vMmodem_Clear_Buff(void) 
	* ����:			Mmodem������ջ���
	* ����:			��
	*	���:			��
	*/
void prv_vMmodem_Clear_Buff(void) {
	vUpdata_Usart_Clear_Buff();
}

/**
	* ������:		void prv_vMmodem_Send_Byte(uint8_t byte)
	* ����:			Mmodem����һ���ֽ�
	* ����:			byte->�������ֽ�
	*	���:			��
	*/
void prv_vMmodem_Send_Byte(uint8_t byte) {
	vUpdata_Usart_SendByte(byte);
}

/**
	* ������:		uint16_t prv_uiMmodem_Get_Buff(uint8_t *data)
	* ����:			Mmodem���ջ�������
	* ����:			*data->�����ջ��������׵�ַ
	*	���:			���ջ������ݳ���
	*/
uint16_t prv_uiMmodem_Get_Buff(uint8_t *data) {
	uint16_t packet_length = 0;
	
	packet_length = uiUpdata_Usart_Get_Buff(data);
	return (packet_length);
}

/**
	* ������:		int8_t prv_cMmodem_Get_File_Info(_YMODEM_MSG_INFO_STR *str)
	* ����:			Mmodem���ս����ļ���Ϣ����
	* ����:			str->Ymodem�ļ���Ϣ�ṹ��
	*	���:			0->�ļ���Ϣ��ȷ
	*						-1->�ļ���Ϣ����
	*/
int8_t prv_cMmodem_Get_File_Info(_MMODEM_MSG_INFO_STR *str) {
	uint8_t packet_data[1314], file_size[FILE_SIZE_LENGTH];
	uint8_t start_addr[START_ADDR_LENGTH] = {0};

	uint8_t *file_ptr;
	uint16_t packet_length = 0, i = 0;
	uint16_t cal_crc_value = 0, get_crc_value = 0;
	
	str->ul_file_size = 0;
	prv_vMmodem_Clear_Buff();	//��ս��ջ���
	prv_vMmodem_Send_Byte(MMODEM_C);	//���Ϳ�ʼYmodem���ݴ���
	
	//�ȴ����ݷ���
	if (prv_cMmodem_TimeOut(NAK_TIMEOUT) != 0) {		
		//��ʱ�˳�		
		return (-1);
	}
	
	//��ȡ�ļ���Ϣ֡
	packet_length = prv_uiMmodem_Get_Buff(packet_data);
	if (packet_length == (PACKET_SIZE+PACKET_OVERHEAD)) {	//��ȷ����
		if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
			prv_vMmodem_Send_Byte(NAK);
			printf("[mmodem] >>> head check error\r\n");
      return (-1);
    }
		//CRCУ���ж�
		cal_crc_value = uiCrc_16(packet_data, ((PACKET_SIZE+PACKET_OVERHEAD)-2));
		get_crc_value = ((uint16_t)packet_data[packet_length-2]<<8)|packet_data[packet_length-1];
		if (cal_crc_value != get_crc_value) {
			prv_vMmodem_Send_Byte(NAK);
			printf("[mmodem] >>> error crc\r\n");
			return (-1);
		}
		
		//�ļ������ݰ�
		if (packet_data[PACKET_HEADER] != 0) {
				//�ļ������ݰ���Ч��������
				for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != ' ') && (i < FILE_NAME_LENGTH);)
				{
						str->file_name[i++] = *file_ptr++;
				}
				str->file_name[i++] = '\0';
				for (i = 0, file_ptr++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
				{
						file_size[i++] = *file_ptr++;
				}
				file_size[i++] = '\0';
				for (i = 0, file_ptr++; (*file_ptr != ' ') && (i < START_ADDR_LENGTH);)
				{
						start_addr[i++] = *file_ptr++;
				}
				start_addr[i++] = '\0';
				
				sscanf((char *)file_size, "%d", &str->ul_file_size);
				sscanf((char *)start_addr, "%d", &str->ul_start_addr);
				
				printf("file name %s\r\n", str->file_name);
				printf("file size %s\r\n", file_size);
				printf("file start addr %s\r\n", start_addr);
				
				printf("size %d\r\n", str->ul_file_size);
				printf("start addr %x\r\n", str->ul_start_addr);
				
				prv_vMmodem_Clear_Buff();
				
				prv_vMmodem_Send_Byte(ACK);
		} else {	//�ļ������ݰ��գ���������
				prv_vMmodem_Send_Byte(NAK);
				printf("[mmodem] >>> error name\r\n");
		}
	}else {
		prv_vMmodem_Send_Byte(NAK);
		printf("[mmodem] >>> error size\r\n");
		return (-1);
	}
	
	return 0;
}

/**
	* ������:		int8_t vMmodem_Handle(_MMODEM_MSG_INFO_STR *str, uint8_t *data)
	* ����:			Mmodem�����ļ�Э�鴦����
	* ����:			str->Mmodem�ļ���Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			0->�����ļ��ɹ�
	*						-1->�����ļ�ʧ��
	*/
int8_t vMmodem_Handle(_MMODEM_MSG_INFO_STR *str, uint8_t *data) {
	uint8_t packet_data[1314] = {0};
	uint16_t packet_length, i, write_size;
	uint32_t ul_ymodem_write_cnt = 0;
	int8_t get_data_ok_flag = (-1);
	uint16_t cal_crc_value = 0, get_crc_value = 0;
	
	//�ж��ļ���Ϣ�Ƿ���ս����ɹ�
	if (prv_cMmodem_Get_File_Info(str) != 0) {
		return (-1);
	}
	
	ul_ymodem_write_cnt = 0;
	
	//ѭ�������ļ�����
	while (prv_cMmodem_TimeOut(NAK_TIMEOUT) == 0) {
		packet_length = prv_uiMmodem_Get_Buff(packet_data);
		prv_vMmodem_Clear_Buff();
		
		if ((packet_length != (PACKET_SIZE+PACKET_OVERHEAD))
				&& (packet_length != (PACKET_1K_SIZE+PACKET_OVERHEAD))
				&& (packet_length != 1)) {
			prv_vMmodem_Send_Byte(NAK);
			continue;
		}
		
		switch (packet_data[0]) {
			case SOH: 
			case STX: {
					//����֡У��
					if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
						prv_vMmodem_Send_Byte(NAK);
						return (-1);
					}
					//�ж�����֡����
					write_size = (packet_data[0] == SOH)?PACKET_SIZE:PACKET_1K_SIZE;
					
					//CRCУ���ж�
					cal_crc_value = uiCrc_16(packet_data, ((PACKET_SIZE+PACKET_OVERHEAD)-2));
					get_crc_value = ((uint16_t)packet_data[packet_length-2]<<8)|packet_data[packet_length-1];
					if (cal_crc_value != get_crc_value) {
						prv_vMmodem_Send_Byte(NAK);
						printf("[mmodem] >>> error crc\r\n");
						return (-1);
					}
					
					//��������
					for (i = 0; i < write_size; ++i) {
						if (ul_ymodem_write_cnt > str->ul_file_size) break;
						data[ul_ymodem_write_cnt] = packet_data[PACKET_HEADER+i];
						ul_ymodem_write_cnt++;
					}
					prv_vMmodem_Send_Byte(ACK);
				}break;
			case EOT: {
					get_data_ok_flag = 0;
					str->crc_value = uiCrc_16(data, str->ul_file_size);
				}break;
			default: break;
		}
	}
	
	return get_data_ok_flag;
}
