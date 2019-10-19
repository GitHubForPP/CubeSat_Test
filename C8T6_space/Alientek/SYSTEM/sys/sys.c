#include "sys.h"

static volatile uint32_t current_clock = 0;

/* ****************************
*		������: void cpu_systick_init(void)
*		����:		sysytick��ʼ��
*		����: data
*		���: ��
* ************************* */
void cpu_systick_init(void) {
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD = SystemCoreClock/800; 			//ÿ10ms���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK
}

//systick�жϺ���
void SysTick_Handler(void)
{
  current_clock++;
}

//��ȡ��ǰsystickʱ���
uint32_t clock_time(void)
{
  return current_clock;
}

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

