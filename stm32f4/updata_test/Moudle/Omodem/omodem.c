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
	* 函数名:		int8_t vOmodem_Handle(uint8_t *data)
	* 作用:			Omodem在轨协议处理函数
	*						*data->接收数据首地址
	*	输出:			0->接收文件成功
	*						-1->接收文件失败
	*/
int8_t vOmodem_Handle(uint8_t *data) {
	uint8_t packet_data[1314] = {0}, cnt = 0, sync_num = 0, len = 0;
	uint8_t i = 0, j = 0;
	uint16_t packet_length;
	int8_t get_data_ok_flag = (-1);
	uint32_t now_idx = 0, all_idx = 0, start_addr = 0;
	
	//循环接收文件数据
	while (prv_cOmodem_TimeOut(OMODEM_NAK_TIMEOUT) == 0) {
		packet_length = prv_uiOmodem_Get_Buff(packet_data);
		prv_vOmodem_Clear_Buff();
		
		printf("packet_length %d\r\n", packet_length);
		
		/* 判断长度 */
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
