/* *****************************************************
*		文件名：		bsp_internal_flash.c
*		作用：			内部Flash操作文件
*		创建日期:		2019-10-28
*		作者:				王小平
* ***************************************************** */
#include "bsp_internal_flash.h"

/**
	* 函数名:		void vInternal_Flash_Init(void)
	* 作用:			内部FLASH BSP初始化
	* 参数:			addr->Flash Sector addr
	*	输出:			无
	*/
void vInternal_Flash_Init(void) {

}

/**
  * @brief  根据输入的地址给出它所在的sector
  *					例如：
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address：地址
  * @retval 地址所在的sector
  */
uint32_t ulInternal_Flash_GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  

  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_Sector_11;  
  }

  else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_Sector_12;  
  }
  else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_Sector_13;  
  }
  else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_Sector_14;  
  }
  else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_Sector_15;  
  }
  else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_Sector_16;  
  }
  else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_Sector_17;  
  }
  else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_Sector_18;  
  }
  else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
  {
    sector = FLASH_Sector_19;  
  }
  else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
  {
    sector = FLASH_Sector_20;  
  } 
  else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
  {
    sector = FLASH_Sector_21;  
  }
  else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
  {
    sector = FLASH_Sector_22;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
  {
    sector = FLASH_Sector_23;  
  }
	
  return sector;
}

/**
	* 函数名:		int8_t cInterNal_Flash_Write(uint8_t *data, uint32_t start_addr, uint32_t len)
	* 作用:			内部Flash写入数据函数
	* 参数:			*data->要写入数据的指针
	*						start_addr->内部flash地址
	*						len->待写入数据长度
	*	输出:			0->成功 非0->失败
	*/
int8_t cInterNal_Flash_Write(uint8_t *data, uint32_t start_addr, uint32_t len) {
	
	uint32_t ulStartSector = 0, ulEndSector = 0;
	uint32_t ulAddress = 0, ulSectorCounter = 0;
	uint32_t ulWriteCnt = 0;
	uint8_t i = 0;

	__IO uint32_t ulData32 = 0;
	
  /* FLASH 解锁 ********************************/
  /* 使能访问FLASH控制寄存器 */
  FLASH_Unlock();
    
  /* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
  /* 清除各种FLASH的标志位 */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	ulStartSector = ulInternal_Flash_GetSector(start_addr);
	ulEndSector = ulInternal_Flash_GetSector(start_addr+len);

  /* 开始擦除操作 */
  ulSectorCounter = ulStartSector;
  while (ulSectorCounter <= ulEndSector) 
  {
    /* VoltageRange_3 以“字”的大小进行操作 */ 
    if (FLASH_EraseSector(ulSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
    { 
      /*擦除出错，返回，实际应用中可加入处理 */
			return -1;
    }
    /* 计数器指向下一个扇区 */
    if (ulSectorCounter == FLASH_Sector_11)
    {
      ulSectorCounter += 40;
    } 
    else 
    {
      ulSectorCounter += 8;
    }
  }

  /* 以“字”的大小为单位写入数据 ********************************/
  ulAddress = start_addr;

  while (ulAddress < (start_addr+len))
  {
		if (((start_addr+len) - ulAddress) >= 4) {
			ulData32 = data[ulWriteCnt]|(uint32_t)data[ulWriteCnt+1]<<8|(uint32_t)data[ulWriteCnt+2]<<16|(uint32_t)data[ulWriteCnt+3]<<24;
		}else {
			ulData32 = 0;
			for (i = 0; i < 4; ++i) {
				ulData32 >>= 8;
				if (i >= ((start_addr+len) - ulAddress)) {
					ulData32 |= 0xFF000000;
				}else {
					ulData32 |= (uint32_t)data[ulWriteCnt+i]<<24;
				}
			}
		}
		
    if (FLASH_ProgramWord(ulAddress, ulData32) == FLASH_COMPLETE)
    {
      ulAddress = ulAddress + 4;
			ulWriteCnt = ulWriteCnt + 4;
    }
    else
    { 
      /*写入出错，返回，实际应用中可加入处理 */
			return -1;
    }
  }
	

  /* 给FLASH上锁，防止内容被篡改*/
  FLASH_Lock(); 
	
	return 0;
}

/**
	* 函数名:		void vInternal_Flash_Read(uint8_t *data, uint32_t start_addr, uint32_t len)
	* 作用:			内部Flash读取数尽⑤函数
	* 参数:			*data->要读取数据的指针
	*						start_addr->内部flash地址
	*						len->待读取数据长度
	*	输出:			无
	*/
void vInternal_Flash_Read(uint8_t *data, uint32_t start_addr, uint32_t len) {
	uint8_t *pdata = (uint8_t *)(start_addr);
	
	memcpy(data, pdata, len);
}	


