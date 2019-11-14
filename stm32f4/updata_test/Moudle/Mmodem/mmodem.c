/* *****************************************************
*		文件名：		mmodem.c
*		作用：			我的modem串口文件协议
*		创建日期:		2019-11-06
*		作者:				王小平
* ***************************************************** */

/******************* 头文件 ****************/
#include "mmodem.h"

/******************* 变量定义 ****************/


/******************* 函数定义 **************/

/**
	* 函数名:		void vMmodem_Init(void)
	* 作用:			Mmodem初始化
	* 参数:			无
	*	输出:			无
	*/
void vMmodem_Init(void) {
	
}

/**
	* 函数名:		int8_t prv_cMmodem_TimeOut(uint32_t t)
	* 作用:			Mmodem接收数据超时函数
	* 参数:			t->超时数值标准
	*	输出:			0->接收新数据完成
	*						-1->超时无新数据
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
	
	//判断超时无新数据
	if ((in_cnt == new_cnt) && (new_cnt == 0)) {	
		return (-1);
	}
	
	return 0;
}

/**
	* 函数名:		void prv_vMmodem_Clear_Buff(void) 
	* 作用:			Mmodem清除接收缓存
	* 参数:			无
	*	输出:			无
	*/
void prv_vMmodem_Clear_Buff(void) {
	vUpdata_Usart_Clear_Buff();
}

/**
	* 函数名:		void prv_vMmodem_Send_Byte(uint8_t byte)
	* 作用:			Mmodem发送一个字节
	* 参数:			byte->待发送字节
	*	输出:			无
	*/
void prv_vMmodem_Send_Byte(uint8_t byte) {
	vUpdata_Usart_SendByte(byte);
}

/**
	* 函数名:		uint16_t prv_uiMmodem_Get_Buff(uint8_t *data)
	* 作用:			Mmodem接收缓存数据
	* 参数:			*data->待接收缓存数据首地址
	*	输出:			接收缓存数据长度
	*/
uint16_t prv_uiMmodem_Get_Buff(uint8_t *data) {
	uint16_t packet_length = 0;
	
	packet_length = uiUpdata_Usart_Get_Buff(data);
	return (packet_length);
}

/**
	* 函数名:		int8_t prv_cMmodem_Get_File_Info(_YMODEM_MSG_INFO_STR *str)
	* 作用:			Mmodem接收解析文件信息函数
	* 参数:			str->Ymodem文件信息结构体
	*	输出:			0->文件信息正确
	*						-1->文件信息错误
	*/
int8_t prv_cMmodem_Get_File_Info(_MMODEM_MSG_INFO_STR *str) {
	uint8_t packet_data[1314], file_size[FILE_SIZE_LENGTH];
	uint8_t start_addr[START_ADDR_LENGTH] = {0};

	uint8_t *file_ptr;
	uint16_t packet_length = 0, i = 0;
	uint16_t cal_crc_value = 0, get_crc_value = 0;
	
	str->ul_file_size = 0;
	prv_vMmodem_Clear_Buff();	//清空接收缓存
	prv_vMmodem_Send_Byte(MMODEM_C);	//发送开始Ymodem数据传输
	
	//等待数据返回
	if (prv_cMmodem_TimeOut(NAK_TIMEOUT) != 0) {		
		//超时退出		
		return (-1);
	}
	
	//读取文件信息帧
	packet_length = prv_uiMmodem_Get_Buff(packet_data);
	if (packet_length == (PACKET_SIZE+PACKET_OVERHEAD)) {	//正确数据
		if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
			prv_vMmodem_Send_Byte(NAK);
			printf("[mmodem] >>> head check error\r\n");
      return (-1);
    }
		//CRC校验判断
		cal_crc_value = uiCrc_16(packet_data, ((PACKET_SIZE+PACKET_OVERHEAD)-2));
		get_crc_value = ((uint16_t)packet_data[packet_length-2]<<8)|packet_data[packet_length-1];
		if (cal_crc_value != get_crc_value) {
			prv_vMmodem_Send_Byte(NAK);
			printf("[mmodem] >>> error crc\r\n");
			return (-1);
		}
		
		//文件名数据包
		if (packet_data[PACKET_HEADER] != 0) {
				//文件名数据包有效数据区域
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
		} else {	//文件名数据包空，结束传输
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
	* 函数名:		int8_t vMmodem_Handle(_MMODEM_MSG_INFO_STR *str, uint8_t *data)
	* 作用:			Mmodem串口文件协议处理函数
	* 参数:			str->Mmodem文件信息结构体
	*						*data->接收数据首地址
	*	输出:			0->接收文件成功
	*						-1->接收文件失败
	*/
int8_t vMmodem_Handle(_MMODEM_MSG_INFO_STR *str, uint8_t *data) {
	uint8_t packet_data[1314] = {0};
	uint16_t packet_length, i, write_size;
	uint32_t ul_ymodem_write_cnt = 0;
	int8_t get_data_ok_flag = (-1);
	uint16_t cal_crc_value = 0, get_crc_value = 0;
	
	//判断文件信息是否接收解析成功
	if (prv_cMmodem_Get_File_Info(str) != 0) {
		return (-1);
	}
	
	ul_ymodem_write_cnt = 0;
	
	//循环接收文件数据
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
					//解析帧校验
					if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
						prv_vMmodem_Send_Byte(NAK);
						return (-1);
					}
					//判断数据帧类型
					write_size = (packet_data[0] == SOH)?PACKET_SIZE:PACKET_1K_SIZE;
					
					//CRC校验判断
					cal_crc_value = uiCrc_16(packet_data, ((PACKET_SIZE+PACKET_OVERHEAD)-2));
					get_crc_value = ((uint16_t)packet_data[packet_length-2]<<8)|packet_data[packet_length-1];
					if (cal_crc_value != get_crc_value) {
						prv_vMmodem_Send_Byte(NAK);
						printf("[mmodem] >>> error crc\r\n");
						return (-1);
					}
					
					//操作数据
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
