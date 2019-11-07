/* *****************************************************
*		文件名：		updata.c
*		作用：			APP更新程序驱动
*		创建日期:		2019-10-24
*		作者:				王小平
* ***************************************************** */
#include "updata.h"

/******************* 内部宏定义 **************/
#define	prv_UPDATA_RUN_INFO_ADDR						(0x08020000)
#define	prv_UPDATA_RUN_DATA_ADDR						(0x08040000)
#define	prv_UPDATA_APP_1_ADDR								(0x00000000)
#define	prv_UPDATA_APP_2_ADDR								(0x00100000)
#define	prv_UPDATA_APP_3_ADDR								(0x00200000)
#define	prv_UPDATA_APP_UP_ADDR							(0x00300000)		//定义更新APP的SPI FLASH地址
#define	prv_UPDATA_INFO_END									(0x5A)
#define prv_UPDATA_FLASH_SECTOR_SIZE				(SPI_FLASH_SectorSize)
typedef   void(* prvFunction)(void);

/******************* 变量定义 ******************/
//跳转定义
prvFunction prv_Jump_To_Application;

/******************* 函数定义 **************/

/* ------ 底层驱动 更换设备需要匹配 ------- */
/**
	* 函数名:		void prv_vUpdata_Flash_Sector_Erase(uint32_t addr)
	* 作用:			Flash擦除接口
	* 参数:			addr->Flash Sector addr
	*	输出:			无
	*/
void prv_vUpdata_Flash_Sector_Erase(uint32_t addr) {
	//SPI_FLASH_SectorErase(addr);
}

/**
	* 函数名:		void prv_vUpdata_Flash_Write(uint8_t *data, uint32_t addr, uint32_t len)
	* 作用:			Flash写入数据接口
	* 参数:			addr->Flash Sector addr
	*	输出:			无
	*/
void prv_vUpdata_Flash_Write(uint8_t *data, uint32_t addr, uint32_t len) {
	//SPI_FLASH_BufferWrite(data, addr, len);
}

/**
	* 函数名:		void prv_vUpdata_Flash_Read(uint8_t *data, uint32_t addr, uint32_t len)
	* 作用:			Flash读取数据接口
	* 参数:			addr->Flash Sector addr
	*	输出:			无
	*/
void prv_vUpdata_Flash_Read(uint8_t *data, uint32_t addr, uint32_t len) {
	//SPI_FLASH_BufferRead(data, addr, len);
}

/**
	* 函数名:		int8_t prv_cUpdata_Run_Flash_Write(uint8_t *data, uint32_t addr, uint32_t len)
	* 作用:			内部Flash写入数据函数
	* 参数:			*data->要写入数据的指针
	*						start_addr->内部flash地址
	*						len->待写入数据长度
	*	输出:			0->成功 非0->失败
	*/
int8_t prv_cUpdata_Run_Flash_Write(uint8_t *data, uint32_t addr, uint32_t len) {
	int8_t flag = 0;
	
	flag = cInterNal_Flash_Write(data, addr, len);
	
	return (flag);
}

/**
	* 函数名:		void vInternal_Flash_Read(uint8_t *data, uint32_t start_addr, uint32_t len)
	* 作用:			内部Flash读取数据函数
	* 参数:			*data->要读取数据的指针
	*						start_addr->内部flash地址
	*						len->待读取数据长度
	*	输出:			无
	*/
void prv_vUpdata_Run_Flash_Read(uint8_t *data, uint32_t addr, uint32_t len) {
	vInternal_Flash_Read(data, addr, len);
}

/* ----------------------------------------- */

/**
	* 函数名:		void vUpdata_Init(void)
	* 作用:			更新程序初始化
	* 参数:			无
	*	输出:			无
	*/
void vUpdata_Init(void) {
	
}

/**
	* 函数名:		void prv_vUpdata_Write_App1(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			写入APP1区程序
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			无
	*/
void prv_vUpdata_Write_App1(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	uint8_t sector_num = 0, i = 0;
	uint32_t data_len = 0;
	
	str->ui_info_frame_end = prv_UPDATA_INFO_END;
	data_len = str->ul_file_size;
	sector_num = ((data_len%SPI_FLASH_SectorSize)==0)?(data_len/SPI_FLASH_SectorSize):((data_len/SPI_FLASH_SectorSize)+1);
	
	//擦除信息加数据区
	for (i = 0; i < sector_num+1; ++i) {
		prv_vUpdata_Flash_Sector_Erase(prv_UPDATA_APP_1_ADDR+(i*SPI_FLASH_SectorSize));	 	 
	}
	
	//写入信息区
	prv_vUpdata_Flash_Write((uint8_t *)str, prv_UPDATA_APP_1_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	//写入数据区
	prv_vUpdata_Flash_Write(data, prv_UPDATA_APP_1_ADDR+SPI_FLASH_SectorSize, data_len);
}

/**
	* 函数名:		void prv_vUpdata_Write_App2(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			写入APP2区程序
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			无
	*/
void prv_vUpdata_Write_App2(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	uint8_t sector_num = 0, i = 0;
	uint32_t data_len = 0;
	
	str->ui_info_frame_end = prv_UPDATA_INFO_END;
	data_len = str->ul_file_size;
	sector_num = ((data_len%SPI_FLASH_SectorSize)==0)?(data_len/SPI_FLASH_SectorSize):((data_len/SPI_FLASH_SectorSize)+1);
	
	//擦除信息加数据区
	for (i = 0; i < sector_num+1; ++i) {
		prv_vUpdata_Flash_Sector_Erase(prv_UPDATA_APP_2_ADDR+(i*SPI_FLASH_SectorSize));	 	 
	}
	
	//写入信息区
	prv_vUpdata_Flash_Write((uint8_t *)str, prv_UPDATA_APP_2_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	//写入数据区
	prv_vUpdata_Flash_Write(data, prv_UPDATA_APP_2_ADDR+SPI_FLASH_SectorSize, data_len);
}

/**
	* 函数名:		void prv_vUpdata_Write_App3(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			写入APP3区程序
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			无
	*/
void prv_vUpdata_Write_App3(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	uint8_t sector_num = 0, i = 0;
	uint32_t data_len = 0;
	
	str->ui_info_frame_end = prv_UPDATA_INFO_END;
	data_len = str->ul_file_size;
	sector_num = ((data_len%SPI_FLASH_SectorSize)==0)?(data_len/SPI_FLASH_SectorSize):((data_len/SPI_FLASH_SectorSize)+1);
	
	//擦除信息加数据区
	for (i = 0; i < sector_num+1; ++i) {
		prv_vUpdata_Flash_Sector_Erase(prv_UPDATA_APP_3_ADDR+(i*SPI_FLASH_SectorSize));	 	 
	}
	
	//写入信息区
	prv_vUpdata_Flash_Write((uint8_t *)str, prv_UPDATA_APP_3_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	//写入数据区
	prv_vUpdata_Flash_Write(data, prv_UPDATA_APP_3_ADDR+SPI_FLASH_SectorSize, data_len);
}

/**
	* 函数名:		void vUpdata_Write_Run(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			写入RUN区程序
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			无
	*/
void vUpdata_Write_Run(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	uint32_t data_len = 0;
	
	str->ui_info_frame_end = prv_UPDATA_INFO_END;
	data_len = str->ul_file_size;
	
	//写入信息区
	prv_cUpdata_Run_Flash_Write((uint8_t *)str, prv_UPDATA_RUN_INFO_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	//写入数据区
	prv_cUpdata_Run_Flash_Write(data, prv_UPDATA_RUN_DATA_ADDR, data_len);
}

/**
	* 函数名:		void vUpdata_Write_UP(_UPDATA_UP_INFO_STR *str, uint8_t *data)
	* 作用:			写入更新区程序
	* 参数:			str->更新区信息结构体
	*						*data->缓存数据首地址
	*	输出:			无
	*/
void vUpdata_Write_UP(_UPDATA_UP_INFO_STR *str, uint8_t *data) {
	uint8_t sector_num = 0, i = 0;
	uint32_t data_len = 0;
	
	str->ui_info_frame_end = prv_UPDATA_INFO_END;
	data_len = str->ul_file_size;
	sector_num = ((data_len%SPI_FLASH_SectorSize)==0)?(data_len/SPI_FLASH_SectorSize):((data_len/SPI_FLASH_SectorSize)+1);
	
	//擦除信息加数据区
	for (i = 0; i < sector_num+1; ++i) {
		prv_vUpdata_Flash_Sector_Erase(prv_UPDATA_APP_UP_ADDR+(i*SPI_FLASH_SectorSize));	 	 
	}
	
	//写入信息区
	prv_vUpdata_Flash_Write((uint8_t *)str, prv_UPDATA_APP_UP_ADDR, sizeof(_UPDATA_UP_INFO_STR));
	
	//写入数据区
	prv_vUpdata_Flash_Write(data, prv_UPDATA_APP_UP_ADDR+SPI_FLASH_SectorSize, data_len);
}

/**
	* 函数名:		void prv_cUpdata_Read_App1(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			读取APP1区程序
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			0->成功 非0->失败
	*/
int8_t prv_cUpdata_Read_App1(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	
	//读取信息区
	prv_vUpdata_Flash_Read((uint8_t *)str, prv_UPDATA_APP_1_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	if (str->ui_info_frame_end != prv_UPDATA_INFO_END) return (-1);
	if (str->soft_ver == 0) return (-1);
	
	//读取数据区
	prv_vUpdata_Flash_Read(data, prv_UPDATA_APP_1_ADDR+SPI_FLASH_SectorSize, str->ul_file_size);
	
	return 0;
}

/**
	* 函数名:		void prv_cUpdata_Read_App2(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			读取APP2区程序
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			0->成功 非0->失败
	*/
int8_t prv_cUpdata_Read_App2(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	
	//读取信息区
	prv_vUpdata_Flash_Read((uint8_t *)str, prv_UPDATA_APP_2_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	if (str->ui_info_frame_end != prv_UPDATA_INFO_END) return (-1);
	if (str->soft_ver == 0) return (-1);
	
	//读取数据区
	prv_vUpdata_Flash_Read(data, prv_UPDATA_APP_2_ADDR+SPI_FLASH_SectorSize, str->ul_file_size);
	
	return 0;
}

/**
	* 函数名:		void prv_cUpdata_Read_App3(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			读取APP3区程序
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			0->成功 非0->失败
	*/
int8_t prv_cUpdata_Read_App3(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	
	//读取信息区
	prv_vUpdata_Flash_Read((uint8_t *)str, prv_UPDATA_APP_3_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	if (str->ui_info_frame_end != prv_UPDATA_INFO_END) return (-1);
	if (str->soft_ver == 0) return (-1);
	
	//读取数据区
	prv_vUpdata_Flash_Read(data, prv_UPDATA_APP_3_ADDR+SPI_FLASH_SectorSize, str->ul_file_size);
	
	return 0;
}

/**
	* 函数名:		void prv_cUpdata_Read_Run(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			读取RUN区程序
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			0->成功 非0->失败
	*/
int8_t prv_cUpdata_Read_Run(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	
	//读取信息区
	prv_vUpdata_Run_Flash_Read((uint8_t *)str, prv_UPDATA_RUN_INFO_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	if (str->ui_info_frame_end != prv_UPDATA_INFO_END) return (-1);
	if (str->soft_ver == 0) return (-1);
	
	//读取数据区
	prv_vUpdata_Run_Flash_Read(data, prv_UPDATA_RUN_DATA_ADDR, str->ul_file_size);
	
	return 0;
}

/**
	* 函数名:		void cUpdata_Read_UP(_UPDATA_UP_INFO_STR *str, uint8_t *data)
	* 作用:			读取更新区程序
	* 参数:			str->更新区信息结构体
	*						*data->缓存数据首地址
	*	输出:			0->成功 非0->失败
	*/
int8_t cUpdata_Read_UP(_UPDATA_UP_INFO_STR *str, uint8_t *data) {
	
	//读取信息区
	prv_vUpdata_Flash_Read((uint8_t *)str, prv_UPDATA_APP_UP_ADDR, sizeof(_UPDATA_UP_INFO_STR));
	
	if (str->ui_info_frame_end != prv_UPDATA_INFO_END) return (-1);
	
	//读取数据区
	prv_vUpdata_Flash_Read(data, prv_UPDATA_APP_UP_ADDR+SPI_FLASH_SectorSize, str->ul_file_size);
	
	return 0;
}

/**
	* 函数名:		void prv_vUpdata_Printf_Msg(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			打印APP数据
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			无
	*/
void prv_vUpdata_Printf_Msg(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	printf(">> file_len %d\r\n", str->ul_file_size);
	uint16_t i = 0;
	
	for (i = 0; i < str->ul_file_size; ++i) {
		printf("%x ", data[i]);
	}
	printf("\r\n");
}

/**
	* 函数名:		void vUpdata_Write_App(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			写入应用程序数据
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			无
	*/
void vUpdata_Write_App(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	prv_vUpdata_Write_App1(str, data);
	prv_vUpdata_Write_App2(str, data);
	prv_vUpdata_Write_App3(str, data);
	vUpdata_Write_Run(str, data);
}

/**
	* 函数名:		int8_t cUpdata_Read_App(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			读取APP区程序
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			0->成功 非0->失败
	*/
int8_t cUpdata_Read_App(uint8_t id, _UPDATA_APP_INFO_STR *str, uint8_t *data) {
	int8_t back_flag = (-1);
	switch (id) {
		case UPDATA_APP_1_IDX: {
				back_flag = prv_cUpdata_Read_App1(str, data);
			}break;
		case UPDATA_APP_2_IDX: {
				back_flag = prv_cUpdata_Read_App2(str, data);
			}break;
		case UPDATA_APP_3_IDX: {
				back_flag = prv_cUpdata_Read_App3(str, data);
			}break;
		case UPDATA_APP_RUN_IDX: {
				back_flag = prv_cUpdata_Read_Run(str, data);
			}break;
		default: break;
	}
	
	return back_flag;
}

/**
	* 函数名:		void vUpdata_Jump_To_Run(void)
	* 作用:			跳转到RUN区执行
	* 参数:			无
	*	输出:			无
	*/
void vUpdata_Jump_To_Run(void) {
	uint32_t prv_JumpAddress;
	prv_JumpAddress = *(__IO uint32_t *)(prv_UPDATA_RUN_DATA_ADDR + 4);
	prv_Jump_To_Application =(prvFunction)prv_JumpAddress;
	__set_MSP(*(__IO uint32_t *)prv_UPDATA_RUN_DATA_ADDR);
	prv_Jump_To_Application();
}

/**
	* 函数名:		void vUpdata_Test(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* 作用:			更新程序测试函数
	* 参数:			str->app信息结构体
	*						*data->缓存数据首地址
	*	输出:			无
	*/
void vUpdata_Test(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	_UPDATA_APP_INFO_STR test_str = {0};
	
	prv_vUpdata_Write_App1(str, data);
	
	if (prv_cUpdata_Read_App1(&test_str, data) != 0) {
		printf(">> get app error\r\n");
	}else {
		prv_vUpdata_Printf_Msg(str, data);
	}
}

