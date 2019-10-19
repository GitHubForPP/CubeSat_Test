#include "stdio.h"
//stm32
#include "stm32f10x.h"
//alientek
#include "sys.h"
#include "usart.h"

//
#include "ymodem.h"
#include "tle.h" 
#include "SGP4.h"
#include "eci.h"

_TLE_STR test_tle_str = {0};
_SGP4_STR test_sgp4_str = {0};
_ECI_STR test_eci_str = {0};

uint32_t time = 0;

int main() {	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	cpu_systick_init();
	printf("system init ok\r\n");
	
//	Tle_Initialize(&test_tle_str,
//					"1 43840U 18102K   18345.81224123  .00000559  00000-0  39437-4 0  9994",
//					"2 43840  97.6245  58.2408 0015178 267.4429 188.4118 15.08969989   697");
	//printf("year->%d tick %d\r\n", Year(test_tle_str.epoch_), test_tle_str.epoch_);

//	SGP4_Initialise(&test_sgp4_str, test_tle_str);

//	SGP4_FindPosition(&test_sgp4_str, &test_eci_str, test_tle_str.epoch_);

//	printf("x %f y %f z %f\r\n", test_eci_str.m_position.x, test_eci_str.m_position.y, test_eci_str.m_position.z);
//	printf("s %d\r\n", test_eci_str.dt% TicksPerMinute);
	//Ymodem_Handle();
	
	while (1) {
		if (clock_time() - time >= (1000/10)) {
			printf("hello!\r\n");
			//printf("ram data %x %x\r\n", *(uint8_t *)(0x20002800), *(uint8_t *)(0x20002800+8481-1));
			time = clock_time();
		}
	}
}

