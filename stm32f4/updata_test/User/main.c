/* *****************************************************
*	文件名: 				main.c
*	作用: 					Boot main文件
*	创建文件时间:		2019-10-24
*	作者:						王小平
* ***************************************************** */
/* 头文件 */
//stm32
#include "stm32f4xx.h"
#include "stdlib.h"

//bsp
#include "bsp_led.h"
#include "bsp_SysTick.h"
#include "bsp_sdram.h"  
#include "bsp_spi_flash.h"
#include "bsp_debug_usart.h"
#include "bsp_internal_flash.h"

//moudle
#include "updata.h"
#include "mmodem.h"
#include "test.h"

/************** 宏定义 ************/
#define MAIN_LOOP_TIME						(5*1000)	//ms
#define MAIN_ONE_LOOP_TIME				(50)			//ms

/************** 函数声明 ************/
void vBSP_Init(void);
void vMoudle_Init(void);

/************** 变量定义 ************/
uint16_t ui_loop_cnt = 0;

//标记
int8_t uc_app_run_flag = 0, uc_updata_app_flag = 0, read_app_cnt = 0;

//升级数据
//_UPDATA_UP_INFO_STR	 updata_up_str = {0};
//_UPDATA_APP_INFO_STR updata_app_str = {0};
_MMODEM_MSG_INFO_STR updata_msg_str = {0};

//缓存数据
uint8_t *p_data = (uint8_t *)(SDRAM_BANK_ADDR);
uint8_t *p_up_data = (uint8_t *)(SDRAM_BANK_ADDR+(1024*1024));

//test
uint32_t i = 0;

/* ---------------------------------- */

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* BSP初始化 */
	vBSP_Init();
	
	/* 模块初始化 */
	vMoudle_Init();
	
//	for (i = 0; i < 700; ++i) {
//		if (i%16 == 0 && i != 0) printf("\r\n");
//		printf("%x\t", *(uint8_t *)(0x10000+0x08000000+i));
//	}
//	printf("\r\n");
	
	test_init();

	while(1) {
		/* LED闪烁 */
		LED_ON;
		vDelay_ms(MAIN_ONE_LOOP_TIME/2);
		LED_OFF;
		vDelay_ms(MAIN_ONE_LOOP_TIME/2);
		/* ----------- */
		
		/* 10S时间到 */
		if (++ui_loop_cnt >= (MAIN_LOOP_TIME/MAIN_ONE_LOOP_TIME)) {
			//ui_loop_cnt = 0;
		}
		/* ------------ */
		
		/* 1S周期 */
		if (ui_loop_cnt%(1000/MAIN_ONE_LOOP_TIME) == 0) {
			if (vMmodem_Handle(&updata_msg_str, p_data) == 0) {
				printf("get updata file ok\r\n");
				for (i = 0; i < updata_msg_str.ul_file_size; ++i) {
					printf("%x ", p_data[i]);
				}
				printf("\r\n\r\n");
				cInterNal_Flash_Write(p_data, updata_msg_str.ul_start_addr+0x08000000, updata_msg_str.ul_file_size);
			}
		}
		/* --------------- */
	}   
}

/******************* 函数定义 **************/
/**
	* 函数名:		void vBSP_Init(void)
	* 作用:			BSP初始化
	* 参数:			无
	*	输出:			无
	*/
void vBSP_Init(void) {
	/* Debug 串口初始化 */
	vDebug_USART_Config();
	
	/* LED 端口初始化 */
	vLedGpioConfig();	 
	
	/* SDRAM 初始化 */
	SDRAM_Init();
	
	/* 内部FLASH初始化 */
	vInternal_Flash_Init();
	
	/* 升级 串口初始化 */
	vUpdata_USART_Config();	
 
  /* 配置SysTick 为10us中断一次,时间到后触发定时中断，
	*进入stm32fxx_it.c文件的SysTick_Handler处理，通过数中断次数计时
	*/
	SysTick_Init();
	
	printf(">> BOOT bsp init ok\r\n");
}

/**
	* 函数名:		void vMoudle_Init(void)
	* 作用:			模块初始化
	* 参数:			无
	*	输出:			无
	*/
void vMoudle_Init(void) {
	/* 串口文件协议初始化 */
	vMmodem_Init();
	
	/* 更新程序初始化 */
	vUpdata_Init();
}





/*********************************************END OF FILE**********************/

