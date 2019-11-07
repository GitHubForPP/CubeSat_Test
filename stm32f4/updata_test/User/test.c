#include "test.h"

void test_init(void) {
	uint8_t *p_data = (uint8_t *)(SDRAM_BANK_ADDR);
	cInterNal_Flash_Write(p_data, 0x0000000, 1000);
	
	vInternal_Flash_Read(p_data, 0x0000000, 1000);
	
	vUpdata_Jump_To_Run();
}



