#include "sys.h"

static volatile uint32_t current_clock = 0;

/* ****************************
*		函数名: void cpu_systick_init(void)
*		作用:		sysytick初始化
*		输入: data
*		输出: 无
* ************************* */
void cpu_systick_init(void) {
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD = SystemCoreClock/800; 			//每10ms秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK
}

//systick中断函数
void SysTick_Handler(void)
{
  current_clock++;
}

//获取当前systick时间戳
uint32_t clock_time(void)
{
  return current_clock;
}

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

