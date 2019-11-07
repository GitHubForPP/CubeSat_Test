/* *****************************************************
*		文件名：		ymodem.c
*		作用：			Ymodem串口文件协议
*		创建日期:		2019-09-30
*		作者:				王小平
* ***************************************************** */

/******************* 头文件 ****************/
#include "ymodem.h"

/******************* 变量定义 ****************/


/******************* 函数定义 **************/

/**
	* 函数名:		void vYmodem_Init(void)
	* 作用:			Ymodem初始化
	* 参数:			无
	*	输出:			无
	*/
void vYmodem_Init(void) {
	
}

/**
	* 函数名:		int8_t prv_cYmodem_TimeOut(uint32_t t)
	* 作用:			Ymodem接收数据超时函数
	* 参数:			t->超时数值标准
	*	输出:			0->接收新数据完成
	*						-1->超时无新数据
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
	
	if ((in_cnt == new_cnt) && (new_cnt == 0)) {	//超时无新数据
		return (-1);
	}
	
	return 0;
}

/**
	* 函数名:		void prv_vYmodem_Clear_Buff(void) 
	* 作用:			Ymodem清除接收缓存
	* 参数:			无
	*	输出:			无
	*/
void prv_vYmodem_Clear_Buff(void) {
	vUpdata_Usart_Clear_Buff();
}

/**
	* 函数名:		void prv_vYmodem_Send_Byte(uint8_t byte)
	* 作用:			Ymodem发送一个字节
	* 参数:			byte->待发送字节
	*	输出:			无
	*/
void prv_vYmodem_Send_Byte(uint8_t byte) {
	vUpdata_Usart_SendByte(byte);
}

/**
	* 函数名:		uint16_t prv_uiYmodem_Get_Buff(uint8_t *data)
	* 作用:			Ymodem接收缓存数据
	* 参数:			*data->待接收缓存数据首地址
	*	输出:			接收缓存数据长度
	*/
uint16_t prv_uiYmodem_Get_Buff(uint8_t *data) {
	uint16_t packet_length = 0;
	
	packet_length = uiUpdata_Usart_Get_Buff(data);
	return (packet_length);
}

/**
	* 函数名:		int8_t prv_cYmodem_Get_File_Info(_YMODEM_MSG_INFO_STR *str)
	* 作用:			Ymodem接收解析文件信息函数
	* 参数:			str->Ymodem文件信息结构体
	*	输出:			0->文件信息正确
	*						-1->文件信息错误
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
	prv_vYmodem_Clear_Buff();	//清空接收缓存
	prv_vYmodem_Send_Byte(YMODEM_C);	//发送开始Ymodem数据传输
	
	//等待数据返回
	if (prv_cYmodem_TimeOut(NAK_TIMEOUT) != 0) {		
		//超时退出 记得释放申请的内存
		//printf("no data\r\n");
		#ifdef YMODEM_RTOS_FLAG
		vPortFree(packet_data); vPortFree(file_size);
		#endif		
		return (-1);
	}
	
	//读取文件信息帧
	packet_length = prv_uiYmodem_Get_Buff(packet_data);
	if (packet_length == (PACKET_SIZE+PACKET_OVERHEAD)) {	//正确数据
		if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
				#ifdef YMODEM_RTOS_FLAG
				vPortFree(packet_data); vPortFree(file_size); 
				#endif
        return (-1);
    }
		//todo 添加CRC校验判断
		//文件名数据包
		if (packet_data[PACKET_HEADER] != 0) {
				//文件名数据包有效数据区域
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
		} else {	//文件名数据包空，结束传输
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
	* 函数名:		int8_t vYmodem_Handle(_YMODEM_MSG_INFO_STR *str)
	* 作用:			Ymodem串口文件协议处理函数
	* 参数:			str->Ymodem文件信息结构体
	*	输出:			0->接收文件成功
	*						-1->接收文件失败
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
	
	//判断文件信息是否接收解析成功
	if (prv_cYmodem_Get_File_Info(str) != 0) {
		#ifdef YMODEM_RTOS_FLAG
		vPortFree(packet_data); vPortFree(file_size); 
		#endif
		return (-1);
	}
	
	ul_ymodem_write_cnt = 0;
	
	//循环接收文件数据
	while (prv_cYmodem_TimeOut(NAK_TIMEOUT) == 0) {
		packet_length = prv_uiYmodem_Get_Buff(packet_data);
		prv_vYmodem_Clear_Buff();
		//printf("%x\r\n", packet_data[0]);
		switch (packet_data[0]) {
			case SOH: 
			case STX: {
					if (stop_flag) {
						//结束
						stop_flag = 0;
						prv_vYmodem_Send_Byte(ACK); 
						break;
					}
					//解析帧校验
					if (packet_data[PACKET_SEQNO_INDEX] != ((packet_data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff)) {
							#ifdef YMODEM_RTOS_FLAG
							vPortFree(packet_data); vPortFree(file_size); 
							#endif
							return (-1);
					}
					//判断数据帧类型
					write_size = (packet_data[0] == SOH)?PACKET_SIZE:PACKET_1K_SIZE;
					
					//操作数据
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
						//第一次EOT发送NACK
						prv_vYmodem_Send_Byte(NAK);
					}else {
						//第二次EOT发送ACK
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
