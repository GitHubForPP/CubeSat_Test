/* *****************************************************
*		文件名：		omodem.c
*		作用：			我的omodem在轨文件协议
*		创建日期:		2019-11-08
*		作者:				王小平
* ***************************************************** */

/******************* 头文件 ****************/
#include "omodem.h"

/******************* 变量定义 ****************/


/******************* 函数定义 **************/

/**
	* 函数名:		void vOmodem_Init(void)
	* 作用:			Omodem初始化
	* 参数:			无
	*	输出:			无
	*/
void vOmodem_Init(void) {
	
}

/**
	* 函数名:		int8_t prv_cOmodem_TimeOut(uint32_t t)
	* 作用:			Omodem接收数据超时函数
	* 参数:			t->超时数值标准
	*	输出:			0->接收新数据完成
	*						-1->超时无新数据
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
	
	if ((in_cnt == new_cnt) && (new_cnt == 0)) {	//超时无新数据
		return (-1);
	}
	
	return 0;
}

/**
	* 函数名:		void prv_vOmodem_Clear_Buff(void) 
	* 作用:			Omodem清除接收缓存
	* 参数:			无
	*	输出:			无
	*/
void prv_vOmodem_Clear_Buff(void) {
	vUpdata_Usart_Clear_Buff();
}

/**
	* 函数名:		void prv_vOmodem_Send_Byte(uint8_t byte)
	* 作用:			Omodem发送一个字节
	* 参数:			byte->待发送字节
	*	输出:			无
	*/
void prv_vOmodem_Send_Byte(uint8_t byte) {
	vUpdata_Usart_SendByte(byte);
}

/**
	* 函数名:		uint16_t prv_uiOmodem_Get_Buff(uint8_t *data)
	* 作用:			Omodem接收缓存数据
	* 参数:			*data->待接收缓存数据首地址
	*	输出:			接收缓存数据长度
	*/
uint16_t prv_uiOmodem_Get_Buff(uint8_t *data) {
	uint16_t packet_length = 0;
	
	packet_length = uiUpdata_Usart_Get_Buff(data);
	return (packet_length);
}

/**
	* 函数名:		int8_t cOmodem_Analysis(_OMODEM_DATA_STR *str, uint8_t *wait_analysis_buff, uint16_t len)
	* 作用:			Omodem在轨解析单帧并且赋值函数
	*	参数:			*str->Omodem协议结构体
	*						*wait_analysis_buff->接收数据首地址
	*						len->等待解析数据长度
	*	输出:			0->解析成功
	*						-1->解析失败
	*/
int8_t cOmodem_Analysis(_OMODEM_DATA_STR *str, uint8_t *wait_analysis_buff, uint16_t len) {
	uint32_t now_idx = 0, all_idx = 0, start_addr = 0;
	uint8_t cnt = OMODEM_PACKET_NOW_ID_IDX, sync_num = 0;
	uint8_t i = 0, j = 0;
	
	/* 判断长度 */
	if (len == OMODEM_PACKET_SIZE) {
		memcpy(&now_idx, &wait_analysis_buff[cnt], 4); cnt+=4;
		memcpy(&all_idx, &wait_analysis_buff[cnt], 4); cnt+=4;
		sync_num = wait_analysis_buff[cnt++];
		
		printf("now_idx %x all_idx %x sync_num %d\r\n", now_idx, all_idx, sync_num);
		str->now_id = now_idx;
		str->all_id = all_idx;
		str->sync_num = sync_num;
		
		for (i = 0; i < sync_num; ++i) {
			memcpy(&start_addr, &wait_analysis_buff[cnt], 4); cnt+=4;
			len = wait_analysis_buff[cnt++];
			
			str->start_addr[i] = start_addr;
			str->sync_len[i] = len;
			memcpy(&str->sync_buff[i][0], &wait_analysis_buff[cnt], len);
			
			printf("start_addr %x len %d\r\n", start_addr, len);
			for (j = 0; j < len; ++j) {
				printf("%x\t", wait_analysis_buff[cnt+j]);
				if ((j%16 == 0) && (j>0)) {
					printf("\r\n");
				}
			}
			printf("\r\n");
			cnt += len;
		}
	}else {
		return (-1);
	}
	
	return 0;
}

/**
	* 函数名:		int8_t cOmodem_Handle(_OMODEM_DATA_STR *str)
	* 作用:			Omodem在轨协议处理函数
	*	参数			*data->接收数据首地址
	*	输出:			0->解析成功
	*						-1->解析失败
	*/
int8_t cOmodem_Handle(_OMODEM_DATA_STR *str) {
	uint8_t packet_data[1314] = {0};
	uint16_t packet_length;
	int8_t get_data_ok_flag = (-1);
	
	//循环接收文件数据
	while (prv_cOmodem_TimeOut(OMODEM_NAK_TIMEOUT) == 0) {
		packet_length = prv_uiOmodem_Get_Buff(packet_data);
		prv_vOmodem_Clear_Buff();
		
		get_data_ok_flag = cOmodem_Analysis(str, packet_data, packet_length);
	}
	
	return get_data_ok_flag;
}
