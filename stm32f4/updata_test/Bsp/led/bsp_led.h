/* *****************************************************
*	文件名: 				bsp_led.h
*	作用: 					led应用函数接口
*	创建文件时间:		2019-10-18
*	作者:						王小平
* ***************************************************** */
	
#ifndef __BSP_LED_H__
#define	__BSP_LED_H__

/**
  ******************************************************************************
  *                              头文件
  ******************************************************************************
  */
	
/* STM头文件 */
#include "stm32f4xx.h"

/**
  ******************************************************************************
  *                              宏定义
  ******************************************************************************
  */

//引脚定义
/*******************************************************/
#define LED_PIN                  GPIO_Pin_12                 
#define LED_GPIO_PORT            GPIOD                       
#define LED_GPIO_CLK             RCC_AHB1Periph_GPIOD
/************************************************************/

/** 控制LED灯亮灭的宏，
	* LED低电平亮，设置ON=0，OFF=1
	* 若LED高电平亮，把宏设置成ON=1 ，OFF=0 即可
	*/
#define ON  0
#define OFF 1

#define LED(a)	if (a)	\
					GPIO_SetBits(LED_GPIO_PORT,LED_PIN);\
					else		\
					GPIO_ResetBits(LED_GPIO_PORT,LED_PIN)


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//设置为高电平
#define digitalLo(p,i)			 {p->BSRRH=i;}		//输出低电平
#define digitalToggle(p,i)	 {p->ODR ^=i;}		//输出反转状态

#define LED_TOGGLE		digitalToggle(LED_GPIO_PORT,LED_PIN)
#define LED_OFF				digitalHi(LED_GPIO_PORT,LED_PIN)
#define LED_ON				digitalLo(LED_GPIO_PORT,LED_PIN)

/**
  ******************************************************************************
  *                              函数声明
  ******************************************************************************
  */
void vLedGpioConfig(void);

#endif /* __LED_H */
