/* *****************************************************
*		ymodem.c
*		Ymodem�����ļ�Э��
*		��������:	20190930
*		����:	��Сƽ
* ***************************************************** */

/******************* ͷ�ļ� ****************/
#include "ymodem.h"

/******************* �������� ****************/
uint8_t file_name[FILE_NAME_LENGTH];
uint32_t ul_ymodem_write_cnt = 0, ul_ymodem_file_size = 0;

/******************* �������� **************/

/**
	* ������:		void vYmodem_Init(void)
	* ����:			Ymodem��ʼ��
	* ����:			��
	*	���:			��
	*/
void vYmodem_Init(void) {
	vUpdata_USART_Config();	//���ô���
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
	
	while (time_out-- > 0) {
		new_cnt = uiUpdata_Usart_Get_Num();
		if (new_cnt != old_cnt) {
			time_out = NAK_TIMEOUT;
			old_cnt = new_cnt;
		}
	}
	
	if (in_cnt == new_cnt && new_cnt == 0) {	//��ʱ��������
		return (-1);
	}
	
	return 0;
}

/**
	* ������:		int8_t prv_cYmodem_Get_File_Info(void)
	* ����:			Ymodem���ս����ļ���Ϣ����
	* ����:			��
	*	���:			0->�ļ���Ϣ��ȷ
	*						-1->�ļ���Ϣ����
	*/
int8_t prv_cYmodem_Get_File_Info(void) {
	uint8_t *packet_data = pvPortMalloc(1314), *file_size = pvPortMalloc(FILE_SIZE_LENGTH);
	uint8_t *file_ptr;
	uint16_t packet_length, i;
	
	ul_ymodem_file_size = 0;
	vUpdata_Usart_Clear_Buff();	//��ս��ջ���
	vUpdata_Usart_SendByte(YMODEM_C);	//���Ϳ�ʼYmodem���ݴ���
	
	//�ȴ����ݷ���
	if (prv_cYmodem_TimeOut(NAK_TIMEOUT) != 0) {		
		//��ʱ�˳� �ǵ��ͷ�������ڴ�
		printf("no data1\r\n");
		vPortFree(packet_data); vPortFree(file_size); 
		return (-1);
	}
	
	//��ȡ�ļ���Ϣ֡
	packet_length = uiUpdata_Usart_Get_Buff(packet_data);
	if (packet_length == (PACKET_SIZE+PACKET_OVERHEAD)) {	//��ȷ����
		if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
				vPortFree(packet_data); vPortFree(file_size); 
        return (-1);
    }
		//todo ���CRCУ���ж�
		//�ļ������ݰ�
		if (packet_data[PACKET_HEADER] != 0) {
				//�ļ������ݰ���Ч��������
				for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
				{
						file_name[i++] = *file_ptr++;
				}
				file_name[i++] = '\0';
				for (i = 0, file_ptr++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
				{
						file_size[i++] = *file_ptr++;
				}
				file_size[i++] = '\0';
				sscanf((char *)file_size, "%d", &ul_ymodem_file_size);
				
				printf("size %d\r\n", ul_ymodem_file_size);
				
				vUpdata_Usart_Clear_Buff();
				ul_ymodem_write_cnt = 0;
				
				vUpdata_Usart_SendByte(ACK);
				vUpdata_Usart_SendByte(YMODEM_C);
		} else {	//�ļ������ݰ��գ���������
				vUpdata_Usart_SendByte(ACK);
		}
	}else {
		printf("error first\r\n");
		vPortFree(packet_data); vPortFree(file_size); 
		return (-1);
	}
	
	return 0;
}

/* *********************************
*	void vYmodem_Handle(void)
*	show:	Ymodem�����ļ�Э�鴦����
*	input:	not
*	output:	not
* ********************************* */
void vYmodem_Handle(void) {
	uint8_t *packet_data = pvPortMalloc(1314), *file_size = pvPortMalloc(FILE_SIZE_LENGTH);
	uint8_t *file_ptr, ymodem_over = 0;
	
	//uint8_t packet_data[USART_1_REC_LEN_MAX], file_size[FILE_SIZE_LENGTH], *file_ptr, ymodem_over = 0;	//PACKET_1K_SIZE + PACKET_OVERHEAD , *put_file = (uint8_t *)(0x20002800)
	uint16_t packet_length, i, write_size;
	static uint32_t s_ymodem_write_cnt = 0, s_ymodem_file_size = 0;
	
	if (prv_cYmodem_Get_File_Info() != 0) {
		vPortFree(packet_data); vPortFree(file_size); 
		return ;
	}
	
	vUpdata_Usart_Clear_Buff();
	vUpdata_Usart_SendByte(YMODEM_C);	//start
	
	if (prv_cYmodem_TimeOut(NAK_TIMEOUT) != 0) {
		printf("no data1\r\n");
		vPortFree(packet_data); vPortFree(file_size); 
		return ;
	}
	
	packet_length = uiUpdata_Usart_Get_Buff(packet_data);
	if (packet_length == (PACKET_SIZE+PACKET_OVERHEAD)) {	//��ȷ����
		if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
				vPortFree(packet_data); vPortFree(file_size); 
        return;
    }
		//todo ���CRCУ���ж�
		//�ļ������ݰ�
		if (packet_data[PACKET_HEADER] != 0) {
				//�ļ������ݰ���Ч��������
				for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
				{
						file_name[i++] = *file_ptr++;
				}
				file_name[i++] = '\0';
				for (i = 0, file_ptr++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
				{
						file_size[i++] = *file_ptr++;
				}
				file_size[i++] = '\0';
				sscanf((char *)file_size, "%d", &s_ymodem_file_size);
				
				//printf("size %d\r\n", s_ymodem_file_size);
				
				vUpdata_Usart_Clear_Buff();
				s_ymodem_write_cnt = 0;
				
				vUpdata_Usart_SendByte(ACK);
				vUpdata_Usart_SendByte(YMODEM_C);
		} else {	//�ļ������ݰ��գ���������
				vUpdata_Usart_SendByte(ACK);
		}
	}else {
		printf("error first\r\n");
		vPortFree(packet_data); vPortFree(file_size); 
		return ;
	}
	
	while (prv_cYmodem_TimeOut(NAK_TIMEOUT) == 0) {
		packet_length = uiUpdata_Usart_Get_Buff(packet_data);
		vUpdata_Usart_Clear_Buff();
		printf("%x\r\n", packet_data[0]);
		switch (packet_data[0]) {
			case SOH: 
			case STX: {
					if (ymodem_over) {
						vUpdata_Usart_SendByte(ACK); break;
					}
					if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
							vPortFree(packet_data); vPortFree(file_size); 
							return;
					}
					write_size = (packet_data[0] == SOH)?PACKET_SIZE:PACKET_1K_SIZE;
					for (i = 0; i < write_size; ++i) {
						if (s_ymodem_write_cnt > s_ymodem_file_size) break;
						//*(uint8_t *)((0xD0400000)+s_ymodem_write_cnt) = packet_data[PACKET_HEADER+i];
						s_ymodem_write_cnt++;
					}
					vUpdata_Usart_SendByte(ACK);
				}break;
			case EOT: {
					vUpdata_Usart_SendByte(ACK);
					vUpdata_Usart_SendByte(YMODEM_C);
					ymodem_over = 1;
				}break;
			default: break;
		}
	}
	
	vPortFree(packet_data); vPortFree(file_size); 	
}
