#include "flash.h"


bool save_array(void *data, int count, uint32_t addr_array)
{
	uint32_t *arr_a = (uint32_t*)data;

	HAL_FLASH_Unlock();
	for (int i=0; i<count; i++)
	{
			uint32_t a[8];
			a[0] = (uint32_t*)arr_a;
			write_flash32(addr_array+i*32, a[0]);//save_float(addr_array+i*32, *arr_a+i);
			arr_a+=8;
	}
	 HAL_FLASH_Lock();
	 return true;
}

bool read_array_int(int *data, int count, uint32_t addr_array)
{
		for (int i=0; i<count; i++)
	{
		*data = (int)read_flash32(addr_array+i*4);
		data++;
	}
}

bool read_array_float(float *data, int count, uint32_t addr_array)
{
		for (int i=0; i<count; i++)
	{
		float read_float = load_float(addr_array+i*4);
		*data = read_float;
		data++;
	}
}



bool save_float(uint32_t addr, float fl)
{
	uint32_t a[8];
	a[0] = (uint32_t*)&fl;
	write_flash32(addr, a[0]);
}

float load_float(uint32_t addr)
{

	uint32_t read_raw;
	read_raw = read_flash32(addr);
	float read_float = *(float*)(&read_raw);
	return read_float;
}


uint32_t read_flash32(uint32_t Data_adr)
{
	return *(uint32_t*) Data_adr;
}



bool write_flash32(uint32_t data_adr, uint32_t data)
{
	HAL_FLASH_Unlock();

	  if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD,data_adr,(uint32_t)data) == HAL_OK)
         {
     		 HAL_FLASH_Lock();
			 return true;
         }
         else
         {
           HAL_FLASH_Lock();
           return false;
         }

}

bool flash_earse(uint32_t addr)
{

FLASH_EraseInitTypeDef EraseInitStruct;
uint32_t PageError = 0;


	 EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
     EraseInitStruct.Sector = addr;
     EraseInitStruct.NbSectors = 1;
     EraseInitStruct.Banks = FLASH_BANK_2;
     EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_4;

	   HAL_FLASH_Unlock();

	if( HAL_FLASHEx_Erase(&EraseInitStruct, &PageError)== HAL_OK)
         {
					 HAL_FLASH_Lock();
					 return true;
         }
         else
         {
            HAL_FLASH_Lock();
            return false;
         }
}




