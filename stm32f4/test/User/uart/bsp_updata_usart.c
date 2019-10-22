/* *****************************************************
*	�ļ���: 				bsp_updata_usart.c
*	����: 					�����������ڲ����ļ�
*	�����ļ�ʱ��:		2019/10/21
*	����:						��Сƽ
* ***************************************************** */

/******************* ͷ�ļ� ****************/
#include "bsp_updata_usart.h"

/******************* �ڲ��궨�� ****************/
#define prvUPDATA_USART_RECEIVE_MAX		(1314)

/******************* �������� ****************/
uint8_t prv_updata_usart_receive_data[prvUPDATA_USART_RECEIVE_MAX] = {0};
uint16_t prv_updata_usart_receive_data_num = 0;

/******************* �������� **************/

/**
	* ������:		void vUpdata_USART_Config(void)
	* ����:			�������ڳ�ʼ��
	* ����:			��
	*	���:			��
	*/
void vUpdata_USART_Config(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

  /* ʹ�� USART ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  /* GPIO��ʼ�� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  ;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
  
  /* ���ô�UPDATA_USART ģʽ */
  /* ���������ã�115200*/
  USART_InitStructure.USART_BaudRate = 115200;
  /* �ֳ�(����λ+У��λ)��8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* ֹͣλ��1��ֹͣλ */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* У��λѡ�񣺲�ʹ��У�� */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* Ӳ�������ƣ���ʹ��Ӳ���� */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* ���USART��ʼ������ */
  USART_Init(USART2, &USART_InitStructure); 
	
  /* Ƕ�������жϿ�����NVIC���� */
	/* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  /* �������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
  
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
  /* ʹ�ܴ��� */
  USART_Cmd(USART2, ENABLE);
}

/**
	* ������:		void vUpdata_Usart_SendByte(uint8_t ch)
	* ����:			�������ڷ���һ���ֽ�
	* ����:			ch->�������ֽ�
	*	���:			��
	*/
void vUpdata_Usart_SendByte(uint8_t ch) {
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(USART2, ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	
}

/**
	* ������:		uint16_t uiUpdata_Usart_Get_Buff(uint8_t *data)
	* ����:			�����������ڽ��ջ�������
	* ����:			*data->���������׵�ַ
	*	���:			���ջ������ݳ���
	*/
uint16_t uiUpdata_Usart_Get_Buff(uint8_t *data) {
	memcpy(data, prv_updata_usart_receive_data, prv_updata_usart_receive_data_num);
	return (prv_updata_usart_receive_data_num);
}

/**
	* ������:		uint16_t uiUpdata_Usart_Get_Num(void)
	* ����:			��ȡ�������ڽ��ջ������ݳ���
	* ����:			��
	*	���:			���ջ������ݳ���
	*/
uint16_t uiUpdata_Usart_Get_Num(void) {
	return (prv_updata_usart_receive_data_num);
}

/**
	* ������:		void vUpdata_Usart_Clear_Buff(void)
	* ����:			����������ڽ��ջ�������
	* ����:			��
	*	���:			��
	*/
void vUpdata_Usart_Clear_Buff(void) {
	memset(prv_updata_usart_receive_data, 0, prv_updata_usart_receive_data_num);
	prv_updata_usart_receive_data_num = 0;
}

/**
	* ������:		void USART2_IRQHandler(void)
	* ����:			�������ڽ����ж�
	* ����:			��
	*	���:			��
	*/
void USART2_IRQHandler(void) {
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
	{	
		if (prv_updata_usart_receive_data_num >= prvUPDATA_USART_RECEIVE_MAX) {
			prv_updata_usart_receive_data_num = 0;
		}
		prv_updata_usart_receive_data[prv_updata_usart_receive_data_num] = USART_ReceiveData(USART2);	//��ȡ���յ�������
		prv_updata_usart_receive_data_num ++;
	} 
}



