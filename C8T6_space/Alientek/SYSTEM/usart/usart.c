
#include "usart.h"	  

////////////////////////////////////////////////////////////////////////////////// 	
//---------------缓存数组------------
static uint8_t s_uart1_receive_buff[USART_1_REC_LEN_MAX];
static uint16_t s_uart1_receive_cnt;

/* *********************************
*	void uart_init(u32 bound)
*	show:	设置串口初始化
*	input:	bound:波特率
*	output:	not
* ********************************* */
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	Uart1_Clean_Receive_Buff();
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

/* *********************************
*	void Uart1_Clean_Receive_Buff(void)
*	show:	清空UART1接收缓存数组
*	input:	not
*	output:	not
* ********************************* */
void Uart1_Clean_Receive_Buff(void) {
	memset(s_uart1_receive_buff, 0, USART_1_REC_LEN_MAX);
	s_uart1_receive_cnt = 0;
}

/* *********************************
*	uint16_t Uart1_Get_Receive_Buff(uint8_t *buff)
*	show:	读取UART1接收缓存数组
*	input:	*buff->获取缓存数据数组首地址
*	output:	UART1接收数据长度
* ********************************* */
uint16_t Uart1_Get_Receive_Buff(uint8_t *buff) {
	memcpy(buff, s_uart1_receive_buff, s_uart1_receive_cnt);
	return s_uart1_receive_cnt;
}

/* *********************************
*	uint16_t Uart1_Get_Receive_Len(void)
*	show:	读取UART1接收缓存长度
*	input:	not
*	output:	UART1接收数据长度
* ********************************* */
uint16_t Uart1_Get_Receive_Len(void) {
	return s_uart1_receive_cnt;
}

//中断服务函数
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d结尾)
	{	
		res = USART_ReceiveData(USART1);	//读取接收到的数据
		if (s_uart1_receive_cnt == USART_1_REC_LEN_MAX) s_uart1_receive_cnt = 0;
		s_uart1_receive_buff[s_uart1_receive_cnt] = res;
		s_uart1_receive_cnt ++;
	} 
} 


//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}

