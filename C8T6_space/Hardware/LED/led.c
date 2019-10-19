/* *****************************************************
*		led.c
*		单机测试LED显示驱动
*		创建日期:	20190515
*		作者:	baymaxPP
* ***************************************************** */
#include "led.h"

/* *********************************
*	void JT_LedInit(void)
*	show:	led init
*	input:	not
*	output:	not
* ********************************* */
void JT_LedInit(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(JT_LED_IO_PERIPH, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = JT_LED_RUN_OK_PIN|JT_LED_DATA_ERROR_PIN|JT_LED_TRANS_ERROR_PIN|JT_LED_TEST_PIN; 	//0-7
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
  GPIO_Init(JT_LED_IO_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(JT_LED_IO_PORT, JT_LED_RUN_OK_PIN);
	GPIO_SetBits(JT_LED_IO_PORT, JT_LED_DATA_ERROR_PIN);
	GPIO_SetBits(JT_LED_IO_PORT, JT_LED_TRANS_ERROR_PIN);
	GPIO_SetBits(JT_LED_IO_PORT, JT_LED_TEST_PIN);
}

/* *********************************
*	void JT_LedSetLight(uint16_t gpio, uint8_t light_flag)
*	show:	set led light
*	input:	gpio-> gpio num
*					light_flag-> light falg  0:灭 1:亮
*	output:	not
* ********************************* */
void JT_LedSetLight(uint16_t gpio, uint8_t light_flag) {
	if (light_flag == 1) {
		GPIO_ResetBits(JT_LED_IO_PORT, gpio);
	}else GPIO_SetBits(JT_LED_IO_PORT, gpio);
}


