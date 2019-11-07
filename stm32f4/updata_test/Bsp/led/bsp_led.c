/* *****************************************************
*	�ļ���: 				bsp_led.c
*	����: 					ledӦ�ú����ӿ�
*	�����ļ�ʱ��:		2019-10-18
*	����:						��Сƽ
* ***************************************************** */

/**
  ******************************************************************************
  *                              ͷ�ļ�
  ******************************************************************************
  */
#include "bsp_led.h"   

/**
	* ������:		void vLedGpioConfig(void)
	* ����:			LED�������ú���
	* ����:			��
	*	���:			��
	*/
void vLedGpioConfig(void) {		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd ( LED_GPIO_CLK, ENABLE); 

		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = LED_PIN;	

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*��������Ϊ����ģʽ*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*������������Ϊ2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);	
		
		/*�ر�LED��*/
		LED_OFF;		
}
/*********************************************END OF FILE**********************/
