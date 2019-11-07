/* *****************************************************
*	文件名: 				bsp_led.c
*	作用: 					led应用函数接口
*	创建文件时间:		2019-10-18
*	作者:						王小平
* ***************************************************** */

/**
  ******************************************************************************
  *                              头文件
  ******************************************************************************
  */
#include "bsp_led.h"   

/**
	* 函数名:		void vLedGpioConfig(void)
	* 作用:			LED引脚配置函数
	* 参数:			无
	*	输出:			无
	*/
void vLedGpioConfig(void) {		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_AHB1PeriphClockCmd ( LED_GPIO_CLK, ENABLE); 

		/*选择要控制的GPIO引脚*/															   
		GPIO_InitStructure.GPIO_Pin = LED_PIN;	

		/*设置引脚模式为输出模式*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*设置引脚为上拉模式*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*设置引脚速率为2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
		GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);	
		
		/*关闭LED灯*/
		LED_OFF;		
}
/*********************************************END OF FILE**********************/
