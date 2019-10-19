
#include "usart.h"	  

////////////////////////////////////////////////////////////////////////////////// 	
//---------------��������------------
static uint8_t s_uart1_receive_buff[USART_1_REC_LEN_MAX];
static uint16_t s_uart1_receive_cnt;

/* *********************************
*	void uart_init(u32 bound)
*	show:	���ô��ڳ�ʼ��
*	input:	bound:������
*	output:	not
* ********************************* */
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	Uart1_Clean_Receive_Buff();
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

/* *********************************
*	void Uart1_Clean_Receive_Buff(void)
*	show:	���UART1���ջ�������
*	input:	not
*	output:	not
* ********************************* */
void Uart1_Clean_Receive_Buff(void) {
	memset(s_uart1_receive_buff, 0, USART_1_REC_LEN_MAX);
	s_uart1_receive_cnt = 0;
}

/* *********************************
*	uint16_t Uart1_Get_Receive_Buff(uint8_t *buff)
*	show:	��ȡUART1���ջ�������
*	input:	*buff->��ȡ�������������׵�ַ
*	output:	UART1�������ݳ���
* ********************************* */
uint16_t Uart1_Get_Receive_Buff(uint8_t *buff) {
	memcpy(buff, s_uart1_receive_buff, s_uart1_receive_cnt);
	return s_uart1_receive_cnt;
}

/* *********************************
*	uint16_t Uart1_Get_Receive_Len(void)
*	show:	��ȡUART1���ջ��泤��
*	input:	not
*	output:	UART1�������ݳ���
* ********************************* */
uint16_t Uart1_Get_Receive_Len(void) {
	return s_uart1_receive_cnt;
}

//�жϷ�����
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d��β)
	{	
		res = USART_ReceiveData(USART1);	//��ȡ���յ�������
		if (s_uart1_receive_cnt == USART_1_REC_LEN_MAX) s_uart1_receive_cnt = 0;
		s_uart1_receive_buff[s_uart1_receive_cnt] = res;
		s_uart1_receive_cnt ++;
	} 
} 


//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}

