/* *****************************************************
*		文件名：		updata.h
*		作用：			APP更新程序驱动
*		创建日期:		2019-10-24
*		作者:				王小平
* ***************************************************** */
#ifndef __UPDATA_H__
#define __UPDATA_H__

/******************* 头文件 ****************/
//stm32
#include "stm32f4xx.h"
#include "string.h"

//bsp
#include "bsp_spi_flash.h"
#include "bsp_internal_flash.h"

/******************* 宏定义定义 **************/
#define UPDATA_APP_1_IDX				(1)
#define UPDATA_APP_2_IDX				(2)
#define UPDATA_APP_3_IDX				(3)
#define UPDATA_APP_RUN_IDX			(4)
#define UPDATA_APP_TEST_UP_IDX	(5)

/******************* 结构体定义 **************/
typedef struct __UPDATA_APP_INFO_STR{
	uint32_t ul_file_size;
	uint32_t soft_ver;
	uint16_t ui_crc_value;
	uint16_t ui_info_frame_end;
}_UPDATA_APP_INFO_STR;									//APP信息结构体

/******************* 函数声明 **************/
void vUpdata_Init(void);
void vUpdata_Write_App(uint8_t id, _UPDATA_APP_INFO_STR *str, uint8_t *data);
int8_t cUpdata_Read_App(uint8_t id, _UPDATA_APP_INFO_STR *str, uint8_t *data);
void vUpdata_Jump_To_Run(void);


#endif

