/* *****************************************************
*		�ļ�����		bsp_internal_flash.c
*		���ã�			�ڲ�Flash�����ļ�
*		��������:		2019-10-28
*		����:				��Сƽ
* ***************************************************** */
#include "bsp_internal_flash.h"

/**
	* ������:		void vInternal_Flash_Init(void)
	* ����:			�ڲ�FLASH BSP��ʼ��
	* ����:			addr->Flash Sector addr
	*	���:			��
	*/
void vInternal_Flash_Init(void) {

}

/**
  * @brief  ��������ĵ�ַ���������ڵ�sector
  *					���磺
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address����ַ
  * @retval ��ַ���ڵ�sector
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
	* ������:		int8_t cInterNal_Flash_Write(uint8_t *data, uint32_t start_addr, uint32_t len)
	* ����:			�ڲ�Flashд�����ݺ���
	* ����:			*data->Ҫд�����ݵ�ָ��
	*						start_addr->�ڲ�flash��ַ
	*						len->��д�����ݳ���
	*	���:			0->�ɹ� ��0->ʧ��
	*/
int8_t cInterNal_Flash_Write(uint8_t *data, uint32_t start_addr, uint32_t len) {
	
	uint32_t ulStartSector = 0, ulEndSector = 0;
	uint32_t ulAddress = 0, ulSectorCounter = 0;
	uint32_t ulWriteCnt = 0;
	uint8_t i = 0;

	__IO uint32_t ulData32 = 0;
	
  /* FLASH ���� ********************************/
  /* ʹ�ܷ���FLASH���ƼĴ��� */
  FLASH_Unlock();
    
  /* �����û����� (�û�����ָ������û��ʹ�õĿռ䣬�����Զ���)**/
  /* �������FLASH�ı�־λ */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 


	ulStartSector = ulInternal_Flash_GetSector(start_addr);
	ulEndSector = ulInternal_Flash_GetSector(start_addr+len);

  /* ��ʼ�������� */
  ulSectorCounter = ulStartSector;
  while (ulSectorCounter <= ulEndSector) 
  {
    /* VoltageRange_3 �ԡ��֡��Ĵ�С���в��� */ 
    if (FLASH_EraseSector(ulSectorCounter, VoltageRange_3) != FLASH_COMPLETE)
    { 
      /*�����������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;
    }
    /* ������ָ����һ������ */
    if (ulSectorCounter == FLASH_Sector_11)
    {
      ulSectorCounter += 40;
    } 
    else 
    {
      ulSectorCounter += 8;
    }
  }

  /* �ԡ��֡��Ĵ�СΪ��λд������ ********************************/
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
      /*д��������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;
    }
  }
	

  /* ��FLASH��������ֹ���ݱ��۸�*/
  FLASH_Lock(); 
	
	return 0;
}

/**
	* ������:		void vInternal_Flash_Read(uint8_t *data, uint32_t start_addr, uint32_t len)
	* ����:			�ڲ�Flash��ȡ�����ݺ���
	* ����:			*data->Ҫ��ȡ���ݵ�ָ��
	*						start_addr->�ڲ�flash��ַ
	*						len->����ȡ���ݳ���
	*	���:			��
	*/
void vInternal_Flash_Read(uint8_t *data, uint32_t start_addr, uint32_t len) {
	uint8_t *pdata = (uint8_t *)(start_addr);
	
	memcpy(data, pdata, len);
}	


