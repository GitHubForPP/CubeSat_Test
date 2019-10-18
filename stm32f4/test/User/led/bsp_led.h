/* *****************************************************
*	�ļ���: 				bsp_led.h
*	����: 					ledӦ�ú����ӿ�
*	�����ļ�ʱ��:		2019-10-18
*	����:						��Сƽ
* ***************************************************** */
	
#ifndef __BSP_LED_H__
#define	__BSP_LED_H__

/**
  ******************************************************************************
  *                              ͷ�ļ�
  ******************************************************************************
  */
	
/* STMͷ�ļ� */
#include "stm32f4xx.h"

/**
  ******************************************************************************
  *                              �궨��
  ******************************************************************************
  */

//���Ŷ���
/*******************************************************/
#define LED_PIN                  GPIO_Pin_12                 
#define LED_GPIO_PORT            GPIOD                       
#define LED_GPIO_CLK             RCC_AHB1Periph_GPIOD
/************************************************************/

/** ����LED������ĺ꣬
	* LED�͵�ƽ��������ON=0��OFF=1
	* ��LED�ߵ�ƽ�����Ѻ����ó�ON=1 ��OFF=0 ����
	*/
#define ON  0
#define OFF 1

#define LED(a)	if (a)	\
					GPIO_SetBits(LED_GPIO_PORT,LED_PIN);\
					else		\
					GPIO_ResetBits(LED_GPIO_PORT,LED_PIN)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//����Ϊ�ߵ�ƽ
#define digitalLo(p,i)			 {p->BSRRH=i;}		//����͵�ƽ
#define digitalToggle(p,i)	 {p->ODR ^=i;}		//�����ת״̬

#define LED_TOGGLE		digitalToggle(LED_GPIO_PORT,LED_PIN)
#define LED_OFF				digitalHi(LED_GPIO_PORT,LED_PIN)
#define LED_ON				digitalLo(LED_GPIO_PORT,LED_PIN)

/**
  ******************************************************************************
  *                              ��������
  ******************************************************************************
  */
void vLedGpioConfig(void);

#endif /* __LED_H */
