/* *****************************************************
*		led.h
*		单机测试LED显示驱动
*		创建日期:	20190515
*		作者:	baymaxPP
* ***************************************************** */
#ifndef __DEF_LED_H__
#define __DEF_LED_H__

//stm32
#include "stm32f10x.h"
//sys
#include "sys.h"

#define JT_LED_IO_PERIPH					(RCC_APB2Periph_GPIOB)
#define JT_LED_IO_PORT						(GPIOB)
#define JT_LED_TEST_PIN						(GPIO_Pin_11)
#define JT_LED_TRANS_ERROR_PIN		(GPIO_Pin_10)
#define JT_LED_DATA_ERROR_PIN			(GPIO_Pin_9)
#define JT_LED_RUN_OK_PIN					(GPIO_Pin_8)

/* *********************************
*	void JT_LedInit(void)
*	show:	led init
* ********************************* */
void JT_LedInit(void);

/* *********************************
*	void JT_LedSetLight(uint16_t gpio, uint8_t light_flag)
*	show:	set led light
*	input:	gpio-> gpio num
*					light_flag-> light falg  0:灭 1:亮
*	output:	not
* ********************************* */
void JT_LedSetLight(uint16_t gpio, uint8_t light_flag);


#endif

