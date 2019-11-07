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
#define UPDATA_APP_1_IDX		(1)
#define UPDATA_APP_2_IDX		(2)
#define UPDATA_APP_3_IDX		(3)
#define UPDATA_APP_RUN_IDX	(4)

/******************* 结构体定义 **************/
typedef struct __UPDATA_APP_INFO_STR{
	uint32_t ul_file_size;
	uint32_t soft_ver;
	uint16_t ui_crc_value;
	uint16_t ui_info_frame_end;
}_UPDATA_APP_INFO_STR;									//APP信息结构体

typedef struct __UPDATA_UP_INFO_STR{
	uint32_t ul_file_size;
	uint32_t soft_ver;
	uint32_t ul_start_idx;
	uint16_t ui_up_crc_value;
	uint16_t ui_all_crc_value;
	uint16_t ui_info_frame_end;
	uint8_t test_flag;
	uint8_t updata_flag;
}_UPDATA_UP_INFO_STR;									//更新区信息结构体

/******************* 函数声明 **************/
void vUpdata_Init(void);
void vUpdata_Write_Run(_UPDATA_APP_INFO_STR *str, uint8_t *data);
void vUpdata_Write_UP(_UPDATA_UP_INFO_STR *str, uint8_t *data);
int8_t cUpdata_Read_UP(_UPDATA_UP_INFO_STR *str, uint8_t *data);
void vUpdata_Write_App(_UPDATA_APP_INFO_STR *str, uint8_t *data);
int8_t cUpdata_Read_App(uint8_t id, _UPDATA_APP_INFO_STR *str, uint8_t *data);
void vUpdata_Jump_To_Run(void);
void vUpdata_Test(_UPDATA_APP_INFO_STR *str, uint8_t *data);


#endif

