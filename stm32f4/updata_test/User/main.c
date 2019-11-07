/* *****************************************************
*	�ļ���: 				main.c
*	����: 					Boot main�ļ�
*	�����ļ�ʱ��:		2019-10-24
*	����:						��Сƽ
* ***************************************************** */
/* ͷ�ļ� */
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

/************** �궨�� ************/
#define MAIN_LOOP_TIME						(5*1000)	//ms
#define MAIN_ONE_LOOP_TIME				(50)			//ms

/************** �������� ************/
void vBSP_Init(void);
void vMoudle_Init(void);

/************** �������� ************/
uint16_t ui_loop_cnt = 0;

//���
int8_t uc_app_run_flag = 0, uc_updata_app_flag = 0, read_app_cnt = 0;

//��������
//_UPDATA_UP_INFO_STR	 updata_up_str = {0};
//_UPDATA_APP_INFO_STR updata_app_str = {0};
_MMODEM_MSG_INFO_STR updata_msg_str = {0};

//��������
uint8_t *p_data = (uint8_t *)(SDRAM_BANK_ADDR);
uint8_t *p_up_data = (uint8_t *)(SDRAM_BANK_ADDR+(1024*1024));

//test
uint32_t i = 0;

/* ---------------------------------- */

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	/* BSP��ʼ�� */
	vBSP_Init();
	
	/* ģ���ʼ�� */
	vMoudle_Init();
	
//	for (i = 0; i < 700; ++i) {
//		if (i%16 == 0 && i != 0) printf("\r\n");
//		printf("%x\t", *(uint8_t *)(0x10000+0x08000000+i));
//	}
//	printf("\r\n");
	
	test_init();

	while(1) {
		/* LED��˸ */
		LED_ON;
		vDelay_ms(MAIN_ONE_LOOP_TIME/2);
		LED_OFF;
		vDelay_ms(MAIN_ONE_LOOP_TIME/2);
		/* ----------- */
		
		/* 10Sʱ�䵽 */
		if (++ui_loop_cnt >= (MAIN_LOOP_TIME/MAIN_ONE_LOOP_TIME)) {
			//ui_loop_cnt = 0;
		}
		/* ------------ */
		
		/* 1S���� */
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

/******************* �������� **************/
/**
	* ������:		void vBSP_Init(void)
	* ����:			BSP��ʼ��
	* ����:			��
	*	���:			��
	*/
void vBSP_Init(void) {
	/* Debug ���ڳ�ʼ�� */
	vDebug_USART_Config();
	
	/* LED �˿ڳ�ʼ�� */
	vLedGpioConfig();	 
	
	/* SDRAM ��ʼ�� */
	SDRAM_Init();
	
	/* �ڲ�FLASH��ʼ�� */
	vInternal_Flash_Init();
	
	/* ���� ���ڳ�ʼ�� */
	vUpdata_USART_Config();	
 
  /* ����SysTick Ϊ10us�ж�һ��,ʱ�䵽�󴥷���ʱ�жϣ�
	*����stm32fxx_it.c�ļ���SysTick_Handler������ͨ�����жϴ�����ʱ
	*/
	SysTick_Init();
	
	printf(">> BOOT bsp init ok\r\n");
}

/**
	* ������:		void vMoudle_Init(void)
	* ����:			ģ���ʼ��
	* ����:			��
	*	���:			��
	*/
void vMoudle_Init(void) {
	/* �����ļ�Э���ʼ�� */
	vMmodem_Init();
	
	/* ���³����ʼ�� */
	vUpdata_Init();
}





/*********************************************END OF FILE**********************/
