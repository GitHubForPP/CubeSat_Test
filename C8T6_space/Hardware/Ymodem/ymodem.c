/* *****************************************************
*		ymodem.c
*		Ymodem串口文件协议
*		创建日期:	20190930
*		作者:	baymaxPP
* ***************************************************** */
#include "ymodem.h"

uint8_t file_name[FILE_NAME_LENGTH];

int8_t Ymodem_TimeOut(uint32_t t) {
	uint16_t old_cnt, new_cnt, in_cnt;
	uint32_t time_out = t;
	
	new_cnt = Uart1_Get_Receive_Len();
	in_cnt = new_cnt;
	old_cnt = new_cnt;
	
	while (time_out-- > 0) {
		new_cnt = Uart1_Get_Receive_Len();
		if (new_cnt != old_cnt) {
			time_out = NAK_TIMEOUT;
			old_cnt = new_cnt;
		}
	}
	
	if (in_cnt == new_cnt && new_cnt == 0) {	//超时无新数据
		return (-1);
	}
	
	return 0;
}

/* *********************************
*	void Ymodem_Handle(void)
*	show:	Ymodem串口文件协议处理函数
*	input:	not
*	output:	not
* ********************************* */
void Ymodem_Handle(void) {
	uint8_t packet_data[USART_1_REC_LEN_MAX], file_size[FILE_SIZE_LENGTH], *file_ptr, ymodem_over = 0;	//PACKET_1K_SIZE + PACKET_OVERHEAD , *put_file = (uint8_t *)(0x20002800)
	uint16_t packet_length, i, write_size;
	static uint32_t s_ymodem_write_cnt = 0, s_ymodem_file_size = 0;
	
	Uart1_Clean_Receive_Buff();
	
	printf(YMODEM_C);	//start
	
	if (Ymodem_TimeOut(NAK_TIMEOUT) != 0) {
		printf("no data1\r\n");
		return ;
	}
	
	packet_length = Uart1_Get_Receive_Buff(packet_data);
	if (packet_length == (PACKET_SIZE+PACKET_OVERHEAD)) {	//正确数据
		if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
        return;
    }
		//todo 添加CRC校验判断
		//文件名数据包
		if (packet_data[PACKET_HEADER] != 0) {
				//文件名数据包有效数据区域
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
				
				Uart1_Clean_Receive_Buff();
				s_ymodem_write_cnt = 0;
				
				printf("%c", ACK);
				printf(YMODEM_C);
		} else {	//文件名数据包空，结束传输
				printf("%c", ACK);
		}
	}else {
		printf("error first\r\n");
		return ;
	}
	
	while (Ymodem_TimeOut(NAK_TIMEOUT) == 0) {
		packet_length = Uart1_Get_Receive_Buff(packet_data);
		Uart1_Clean_Receive_Buff();
		
		switch (packet_data[0]) {
			case SOH: 
			case STX: {
					if (ymodem_over) {
						printf("%c", ACK); break;
					}
					if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
							return;
					}
					write_size = (packet_data[0] == SOH)?PACKET_SIZE:PACKET_1K_SIZE;
					for (i = 0; i < write_size; ++i) {
						if (s_ymodem_write_cnt > s_ymodem_file_size) break;
						*(uint8_t *)(0x20002800+s_ymodem_write_cnt) = packet_data[PACKET_HEADER+i];
						s_ymodem_write_cnt++;
					}
					printf("%c", ACK);
				}break;
			case EOT: {
					printf("%c", ACK);
					printf(YMODEM_C);
					ymodem_over = 1;
				}break;
			default: break;
		}
	}
	
}
