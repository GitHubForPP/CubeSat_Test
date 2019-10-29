/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS V9.0.0  + STM32 �̼�������
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32 ȫϵ�п����� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
/* ������Ӳ��bspͷ�ļ� */
#include "bsp_led.h"
#include "bsp_debug_usart.h"
#include "bsp_sdram.h"   
#include "bsp_spi_flash.h"
#include "bsp_updata_usart.h"

/* �ļ�ϵͳͷ�� */
#include "ff.h"
#include "user_filesystem.h"

/* Moudle */
//ymodem
#include "ymodem.h"		

//orbit
#include "tle.h" 			
#include "SGP4.h"
#include "eci.h"

/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* ���������� */
static TaskHandle_t Test_Task_Handle = NULL;/* LED������ */


/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */


/******************************* ȫ�ֱ������� ************************************/
HeapRegion_t xHeapRegions[] =
	{
	{ ( uint8_t * ) SDRAM_BANK_ADDR, IS42S16400J_SIZE-0x100000 },
	{ NULL, 0 }                
};

 
/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void Test_Task(void* pvParameters);/* Test_Task����ʵ�� */

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x40000);
	
  /* ������Ӳ����ʼ�� */
  BSP_Init();
  
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
															
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���
  
  /* ����Test_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )Test_Task, /* ������ں��� */
                        (const char*    )"Test_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&Test_Task_Handle);/* ������ƿ�ָ�� */
															
  if(pdPASS == xReturn)
    printf("����Test_Task����ɹ�!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}



/**********************************************************************
  * @ ������  �� Test_Task
  * @ ����˵���� Test_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void Test_Task(void* parameter)
{	
//	_YMODEM_MSG_INFO_STR str = {0};
//	vYmodem_Handle(&str);
	printf(">> test task start1\r\n");
  while (1)
  {
    LED_ON;
    //printf(">>> Test_Task Running,LED1_ON\r\n");
    vTaskDelay(500);   /* ��ʱ500��tick */
    
    LED_OFF;     
    //printf(">>> Test_Task Running,LED1_OFF\r\n");
    vTaskDelay(500);   /* ��ʱ500��tick */
  }
}

/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED ��ʼ�� */
	vLedGpioConfig();

	/* ���ڳ�ʼ��	*/
	vDebug_USART_Config();
  
  /* ������ʼ��	*/
  //Key_GPIO_Config();
	
	//��ʼ���ⲿSRAM  
	SDRAM_Init();
	
	/* 16M����flash W25Q128��ʼ�� */
//	SPI_FLASH_Init();
	
	vPortDefineHeapRegions( xHeapRegions );
	
	/*	Ymodem��ʼ�� (����ռ�ʹ��SDRAM ��ȷ��SDRAM�Ѿ���ʼ��)	*/
	vYmodem_Init();
	
	printf(">> bsp init ok\r\n");

}


/********************************END OF FILE****************************/
