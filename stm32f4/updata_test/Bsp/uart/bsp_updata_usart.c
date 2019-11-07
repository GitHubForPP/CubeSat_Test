/* *****************************************************
*	文件名: 				bsp_updata_usart.c
*	作用: 					地面升级串口操作文件
*	创建文件时间:		2019/10/21
*	作者:						王小平
* ***************************************************** */

/******************* 头文件 ****************/
#include "bsp_updata_usart.h"

/******************* 内部宏定义 ****************/
#define prvUPDATA_USART_RECEIVE_MAX		(1314)

/******************* 变量定义 ****************/
uint8_t prv_updata_usart_receive_data[prvUPDATA_USART_RECEIVE_MAX] = {0};
uint16_t prv_updata_usart_receive_data_num = 0;

/******************* 函数定义 **************/

/**
	* 函数名:		void vUpdata_USART_Config(void)
	* 作用:			升级串口初始化
	* 参数:			无
	*	输出:			无
	*/
void vUpdata_USART_Config(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

  /* 使能 USART 时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  ;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
  
  /* 配置串UPDATA_USART 模式 */
  /* 波特率设置：115200*/
  USART_InitStructure.USART_BaudRate = 115200;
  /* 字长(数据位+校验位)：8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* 停止位：1个停止位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* 校验位选择：不使用校验 */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* 硬件流控制：不使用硬件流 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART模式控制：同时使能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 完成USART初始化配置 */
  USART_Init(USART2, &USART_InitStructure); 
	
  /* 嵌套向量中断控制器NVIC配置 */
	/* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  /* 抢断优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
  
	/* 使能串口接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
  /* 使能串口 */
  USART_Cmd(USART2, ENABLE);
}

/**
	* 函数名:		void vUpdata_Usart_SendByte(uint8_t ch)
	* 作用:			升级串口发送一个字节
	* 参数:			ch->待发送字节
	*	输出:			无
	*/
void vUpdata_Usart_SendByte(uint8_t ch) {
	/* 发送一个字节数据到USART */
	USART_SendData(USART2, ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	
}

/**
	* 函数名:		uint16_t uiUpdata_Usart_Get_Buff(uint8_t *data)
	* 作用:			拷贝升级串口接收缓存数据
	* 参数:			*data->接收数组首地址
	*	输出:			接收缓存数据长度
	*/
uint16_t uiUpdata_Usart_Get_Buff(uint8_t *data) {
	memcpy(data, prv_updata_usart_receive_data, prv_updata_usart_receive_data_num);
	return (prv_updata_usart_receive_data_num);
}

/**
	* 函数名:		uint16_t uiUpdata_Usart_Get_Num(void)
	* 作用:			读取升级串口接收缓存数据长度
	* 参数:			无
	*	输出:			接收缓存数据长度
	*/
uint16_t uiUpdata_Usart_Get_Num(void) {
	return (prv_updata_usart_receive_data_num);
}

/**
	* 函数名:		void vUpdata_Usart_Clear_Buff(void)
	* 作用:			清空升级串口接收缓存数据
	* 参数:			无
	*	输出:			无
	*/
void vUpdata_Usart_Clear_Buff(void) {
	memset(prv_updata_usart_receive_data, 0, prv_updata_usart_receive_data_num);
	prv_updata_usart_receive_data_num = 0;
}

/**
	* 函数名:		void USART2_IRQHandler(void)
	* 作用:			升级串口接收中断
	* 参数:			无
	*	输出:			无
	*/
void USART2_IRQHandler(void) {
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{	
		if (prv_updata_usart_receive_data_num >= prvUPDATA_USART_RECEIVE_MAX) {
			prv_updata_usart_receive_data_num = 0;
		}
		prv_updata_usart_receive_data[prv_updata_usart_receive_data_num] = USART_ReceiveData(USART2);	//读取接收到的数据
		prv_updata_usart_receive_data_num ++;
	} 
}



