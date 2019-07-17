#ifndef FLASH_H_
#define FLASH_H_

#ifndef FLASH_H
#define FLASH_H

#include "_string.h"
#include "stm32h7xx_hal.h"
#include "uart.h"

#define FLASH_SECTOR 15			// - номер последнего сектора STM32h743
#define ADD_15_SECTOR 0x081E0000 // - адрес последнего сектора STM32h743
#define BLOCK_SIZE 128000/5		// - число, на которое делим блок на разные нужды

#define A_SETTINGS  ADD_15_SECTOR + BLOCK_SIZE * 0
#define A_X_DROPCOORD ADD_15_SECTOR + BLOCK_SIZE * 1 //массив координат по 500 штук (100*4байта)
#define A_Y_DROPCOORD ADD_15_SECTOR + BLOCK_SIZE * 2
#define A_Z_DROPCOORD ADD_15_SECTOR + BLOCK_SIZE * 3
#define A_MODE_MOTION ADD_15_SECTOR + BLOCK_SIZE * 4

#define STEP_ADDR 32

void read_array_float(float *data, int count, uint32_t addr_array);
void read_array_int(int *data, int count, uint32_t addr_array);
bool save_array(void *data, int count, uint32_t addr_array);
void save_float(uint32_t addr, float fl);
float load_float(uint32_t addr);
uint32_t read_flash32(uint32_t Data_adr);
bool write_flash32(uint32_t data_adr, uint32_t data);
bool flash_erase(uint32_t addr);

#endif

#endif /* FLASH_H_ */
