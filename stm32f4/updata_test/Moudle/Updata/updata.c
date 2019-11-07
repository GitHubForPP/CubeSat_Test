/* *****************************************************
*		�ļ�����		updata.c
*		���ã�			APP���³�������
*		��������:		2019-10-24
*		����:				��Сƽ
* ***************************************************** */
#include "updata.h"

/******************* �ڲ��궨�� **************/
#define	prv_UPDATA_RUN_INFO_ADDR						(0x08020000)
#define	prv_UPDATA_RUN_DATA_ADDR						(0x08040000)
#define	prv_UPDATA_APP_1_ADDR								(0x00000000)
#define	prv_UPDATA_APP_2_ADDR								(0x00100000)
#define	prv_UPDATA_APP_3_ADDR								(0x00200000)
#define	prv_UPDATA_APP_UP_ADDR							(0x00300000)		//�������APP��SPI FLASH��ַ
#define	prv_UPDATA_INFO_END									(0x5A)
#define prv_UPDATA_FLASH_SECTOR_SIZE				(SPI_FLASH_SectorSize)
typedef   void(* prvFunction)(void);

/******************* �������� ******************/
//��ת����
prvFunction prv_Jump_To_Application;

/******************* �������� **************/

/* ------ �ײ����� �����豸��Ҫƥ�� ------- */
/**
	* ������:		void prv_vUpdata_Flash_Sector_Erase(uint32_t addr)
	* ����:			Flash�����ӿ�
	* ����:			addr->Flash Sector addr
	*	���:			��
	*/
void prv_vUpdata_Flash_Sector_Erase(uint32_t addr) {
	//SPI_FLASH_SectorErase(addr);
}

/**
	* ������:		void prv_vUpdata_Flash_Write(uint8_t *data, uint32_t addr, uint32_t len)
	* ����:			Flashд�����ݽӿ�
	* ����:			addr->Flash Sector addr
	*	���:			��
	*/
void prv_vUpdata_Flash_Write(uint8_t *data, uint32_t addr, uint32_t len) {
	//SPI_FLASH_BufferWrite(data, addr, len);
}

/**
	* ������:		void prv_vUpdata_Flash_Read(uint8_t *data, uint32_t addr, uint32_t len)
	* ����:			Flash��ȡ���ݽӿ�
	* ����:			addr->Flash Sector addr
	*	���:			��
	*/
void prv_vUpdata_Flash_Read(uint8_t *data, uint32_t addr, uint32_t len) {
	//SPI_FLASH_BufferRead(data, addr, len);
}

/**
	* ������:		int8_t prv_cUpdata_Run_Flash_Write(uint8_t *data, uint32_t addr, uint32_t len)
	* ����:			�ڲ�Flashд�����ݺ���
	* ����:			*data->Ҫд�����ݵ�ָ��
	*						start_addr->�ڲ�flash��ַ
	*						len->��д�����ݳ���
	*	���:			0->�ɹ� ��0->ʧ��
	*/
int8_t prv_cUpdata_Run_Flash_Write(uint8_t *data, uint32_t addr, uint32_t len) {
	int8_t flag = 0;
	
	flag = cInterNal_Flash_Write(data, addr, len);
	
	return (flag);
}

/**
	* ������:		void vInternal_Flash_Read(uint8_t *data, uint32_t start_addr, uint32_t len)
	* ����:			�ڲ�Flash��ȡ���ݺ���
	* ����:			*data->Ҫ��ȡ���ݵ�ָ��
	*						start_addr->�ڲ�flash��ַ
	*						len->����ȡ���ݳ���
	*	���:			��
	*/
void prv_vUpdata_Run_Flash_Read(uint8_t *data, uint32_t addr, uint32_t len) {
	vInternal_Flash_Read(data, addr, len);
}

/* ----------------------------------------- */

/**
	* ������:		void vUpdata_Init(void)
	* ����:			���³����ʼ��
	* ����:			��
	*	���:			��
	*/
void vUpdata_Init(void) {
	
}

/**
	* ������:		void prv_vUpdata_Write_App1(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			д��APP1������
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			��
	*/
void prv_vUpdata_Write_App1(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	uint8_t sector_num = 0, i = 0;
	uint32_t data_len = 0;
	
	str->ui_info_frame_end = prv_UPDATA_INFO_END;
	data_len = str->ul_file_size;
	sector_num = ((data_len%SPI_FLASH_SectorSize)==0)?(data_len/SPI_FLASH_SectorSize):((data_len/SPI_FLASH_SectorSize)+1);
	
	//������Ϣ��������
	for (i = 0; i < sector_num+1; ++i) {
		prv_vUpdata_Flash_Sector_Erase(prv_UPDATA_APP_1_ADDR+(i*SPI_FLASH_SectorSize));	 	 
	}
	
	//д����Ϣ��
	prv_vUpdata_Flash_Write((uint8_t *)str, prv_UPDATA_APP_1_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	//д��������
	prv_vUpdata_Flash_Write(data, prv_UPDATA_APP_1_ADDR+SPI_FLASH_SectorSize, data_len);
}

/**
	* ������:		void prv_vUpdata_Write_App2(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			д��APP2������
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			��
	*/
void prv_vUpdata_Write_App2(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	uint8_t sector_num = 0, i = 0;
	uint32_t data_len = 0;
	
	str->ui_info_frame_end = prv_UPDATA_INFO_END;
	data_len = str->ul_file_size;
	sector_num = ((data_len%SPI_FLASH_SectorSize)==0)?(data_len/SPI_FLASH_SectorSize):((data_len/SPI_FLASH_SectorSize)+1);
	
	//������Ϣ��������
	for (i = 0; i < sector_num+1; ++i) {
		prv_vUpdata_Flash_Sector_Erase(prv_UPDATA_APP_2_ADDR+(i*SPI_FLASH_SectorSize));	 	 
	}
	
	//д����Ϣ��
	prv_vUpdata_Flash_Write((uint8_t *)str, prv_UPDATA_APP_2_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	//д��������
	prv_vUpdata_Flash_Write(data, prv_UPDATA_APP_2_ADDR+SPI_FLASH_SectorSize, data_len);
}

/**
	* ������:		void prv_vUpdata_Write_App3(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			д��APP3������
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			��
	*/
void prv_vUpdata_Write_App3(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	uint8_t sector_num = 0, i = 0;
	uint32_t data_len = 0;
	
	str->ui_info_frame_end = prv_UPDATA_INFO_END;
	data_len = str->ul_file_size;
	sector_num = ((data_len%SPI_FLASH_SectorSize)==0)?(data_len/SPI_FLASH_SectorSize):((data_len/SPI_FLASH_SectorSize)+1);
	
	//������Ϣ��������
	for (i = 0; i < sector_num+1; ++i) {
		prv_vUpdata_Flash_Sector_Erase(prv_UPDATA_APP_3_ADDR+(i*SPI_FLASH_SectorSize));	 	 
	}
	
	//д����Ϣ��
	prv_vUpdata_Flash_Write((uint8_t *)str, prv_UPDATA_APP_3_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	//д��������
	prv_vUpdata_Flash_Write(data, prv_UPDATA_APP_3_ADDR+SPI_FLASH_SectorSize, data_len);
}

/**
	* ������:		void vUpdata_Write_Run(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			д��RUN������
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			��
	*/
void vUpdata_Write_Run(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	uint32_t data_len = 0;
	
	str->ui_info_frame_end = prv_UPDATA_INFO_END;
	data_len = str->ul_file_size;
	
	//д����Ϣ��
	prv_cUpdata_Run_Flash_Write((uint8_t *)str, prv_UPDATA_RUN_INFO_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	//д��������
	prv_cUpdata_Run_Flash_Write(data, prv_UPDATA_RUN_DATA_ADDR, data_len);
}

/**
	* ������:		void vUpdata_Write_UP(_UPDATA_UP_INFO_STR *str, uint8_t *data)
	* ����:			д�����������
	* ����:			str->��������Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			��
	*/
void vUpdata_Write_UP(_UPDATA_UP_INFO_STR *str, uint8_t *data) {
	uint8_t sector_num = 0, i = 0;
	uint32_t data_len = 0;
	
	str->ui_info_frame_end = prv_UPDATA_INFO_END;
	data_len = str->ul_file_size;
	sector_num = ((data_len%SPI_FLASH_SectorSize)==0)?(data_len/SPI_FLASH_SectorSize):((data_len/SPI_FLASH_SectorSize)+1);
	
	//������Ϣ��������
	for (i = 0; i < sector_num+1; ++i) {
		prv_vUpdata_Flash_Sector_Erase(prv_UPDATA_APP_UP_ADDR+(i*SPI_FLASH_SectorSize));	 	 
	}
	
	//д����Ϣ��
	prv_vUpdata_Flash_Write((uint8_t *)str, prv_UPDATA_APP_UP_ADDR, sizeof(_UPDATA_UP_INFO_STR));
	
	//д��������
	prv_vUpdata_Flash_Write(data, prv_UPDATA_APP_UP_ADDR+SPI_FLASH_SectorSize, data_len);
}

/**
	* ������:		void prv_cUpdata_Read_App1(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			��ȡAPP1������
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			0->�ɹ� ��0->ʧ��
	*/
int8_t prv_cUpdata_Read_App1(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	
	//��ȡ��Ϣ��
	prv_vUpdata_Flash_Read((uint8_t *)str, prv_UPDATA_APP_1_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	if (str->ui_info_frame_end != prv_UPDATA_INFO_END) return (-1);
	if (str->soft_ver == 0) return (-1);
	
	//��ȡ������
	prv_vUpdata_Flash_Read(data, prv_UPDATA_APP_1_ADDR+SPI_FLASH_SectorSize, str->ul_file_size);
	
	return 0;
}

/**
	* ������:		void prv_cUpdata_Read_App2(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			��ȡAPP2������
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			0->�ɹ� ��0->ʧ��
	*/
int8_t prv_cUpdata_Read_App2(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	
	//��ȡ��Ϣ��
	prv_vUpdata_Flash_Read((uint8_t *)str, prv_UPDATA_APP_2_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	if (str->ui_info_frame_end != prv_UPDATA_INFO_END) return (-1);
	if (str->soft_ver == 0) return (-1);
	
	//��ȡ������
	prv_vUpdata_Flash_Read(data, prv_UPDATA_APP_2_ADDR+SPI_FLASH_SectorSize, str->ul_file_size);
	
	return 0;
}

/**
	* ������:		void prv_cUpdata_Read_App3(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			��ȡAPP3������
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			0->�ɹ� ��0->ʧ��
	*/
int8_t prv_cUpdata_Read_App3(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	
	//��ȡ��Ϣ��
	prv_vUpdata_Flash_Read((uint8_t *)str, prv_UPDATA_APP_3_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	if (str->ui_info_frame_end != prv_UPDATA_INFO_END) return (-1);
	if (str->soft_ver == 0) return (-1);
	
	//��ȡ������
	prv_vUpdata_Flash_Read(data, prv_UPDATA_APP_3_ADDR+SPI_FLASH_SectorSize, str->ul_file_size);
	
	return 0;
}

/**
	* ������:		void prv_cUpdata_Read_Run(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			��ȡRUN������
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			0->�ɹ� ��0->ʧ��
	*/
int8_t prv_cUpdata_Read_Run(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	
	//��ȡ��Ϣ��
	prv_vUpdata_Run_Flash_Read((uint8_t *)str, prv_UPDATA_RUN_INFO_ADDR, sizeof(_UPDATA_APP_INFO_STR));
	
	if (str->ui_info_frame_end != prv_UPDATA_INFO_END) return (-1);
	if (str->soft_ver == 0) return (-1);
	
	//��ȡ������
	prv_vUpdata_Run_Flash_Read(data, prv_UPDATA_RUN_DATA_ADDR, str->ul_file_size);
	
	return 0;
}

/**
	* ������:		void cUpdata_Read_UP(_UPDATA_UP_INFO_STR *str, uint8_t *data)
	* ����:			��ȡ����������
	* ����:			str->��������Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			0->�ɹ� ��0->ʧ��
	*/
int8_t cUpdata_Read_UP(_UPDATA_UP_INFO_STR *str, uint8_t *data) {
	
	//��ȡ��Ϣ��
	prv_vUpdata_Flash_Read((uint8_t *)str, prv_UPDATA_APP_UP_ADDR, sizeof(_UPDATA_UP_INFO_STR));
	
	if (str->ui_info_frame_end != prv_UPDATA_INFO_END) return (-1);
	
	//��ȡ������
	prv_vUpdata_Flash_Read(data, prv_UPDATA_APP_UP_ADDR+SPI_FLASH_SectorSize, str->ul_file_size);
	
	return 0;
}

/**
	* ������:		void prv_vUpdata_Printf_Msg(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			��ӡAPP����
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			��
	*/
void prv_vUpdata_Printf_Msg(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	printf(">> file_len %d\r\n", str->ul_file_size);
	uint16_t i = 0;
	
	for (i = 0; i < str->ul_file_size; ++i) {
		printf("%x ", data[i]);
	}
	printf("\r\n");
}

/**
	* ������:		void vUpdata_Write_App(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			д��Ӧ�ó�������
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			��
	*/
void vUpdata_Write_App(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	prv_vUpdata_Write_App1(str, data);
	prv_vUpdata_Write_App2(str, data);
	prv_vUpdata_Write_App3(str, data);
	vUpdata_Write_Run(str, data);
}

/**
	* ������:		int8_t cUpdata_Read_App(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			��ȡAPP������
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			0->�ɹ� ��0->ʧ��
	*/
int8_t cUpdata_Read_App(uint8_t id, _UPDATA_APP_INFO_STR *str, uint8_t *data) {
	int8_t back_flag = (-1);
	switch (id) {
		case UPDATA_APP_1_IDX: {
				back_flag = prv_cUpdata_Read_App1(str, data);
			}break;
		case UPDATA_APP_2_IDX: {
				back_flag = prv_cUpdata_Read_App2(str, data);
			}break;
		case UPDATA_APP_3_IDX: {
				back_flag = prv_cUpdata_Read_App3(str, data);
			}break;
		case UPDATA_APP_RUN_IDX: {
				back_flag = prv_cUpdata_Read_Run(str, data);
			}break;
		default: break;
	}
	
	return back_flag;
}

/**
	* ������:		void vUpdata_Jump_To_Run(void)
	* ����:			��ת��RUN��ִ��
	* ����:			��
	*	���:			��
	*/
void vUpdata_Jump_To_Run(void) {
	uint32_t prv_JumpAddress;
	prv_JumpAddress = *(__IO uint32_t *)(prv_UPDATA_RUN_DATA_ADDR + 4);
	prv_Jump_To_Application =(prvFunction)prv_JumpAddress;
	__set_MSP(*(__IO uint32_t *)prv_UPDATA_RUN_DATA_ADDR);
	prv_Jump_To_Application();
}

/**
	* ������:		void vUpdata_Test(_UPDATA_APP_INFO_STR *str, uint8_t *data)
	* ����:			���³�����Ժ���
	* ����:			str->app��Ϣ�ṹ��
	*						*data->���������׵�ַ
	*	���:			��
	*/
void vUpdata_Test(_UPDATA_APP_INFO_STR *str, uint8_t *data) {
	_UPDATA_APP_INFO_STR test_str = {0};
	
	prv_vUpdata_Write_App1(str, data);
	
	if (prv_cUpdata_Read_App1(&test_str, data) != 0) {
		printf(">> get app error\r\n");
	}else {
		prv_vUpdata_Printf_Msg(str, data);
	}
}

