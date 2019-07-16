/*
 * flash.h
 *
 *  Created on: Jul 10, 2019
 *      Author: user
 */

#ifndef FLASH_H_
#define FLASH_H_

#ifndef FLASH_H
#define FLASH_H

#include "_string.h"
#include "stm32h7xx_hal.h"
#include "uart.h"


//========stm32f3
/*
#define BLOCK_SIZE 2048
#define ADD_121_SECTOR 0x0803C800
#define ADD_122_SECTOR 0x0803D000
#define ADD_123_SECTOR 0x0803D800
#define ADD_124_SECTOR 0x0803E000  //- адрес последнего -3 сектора STM32F303
#define ADD_125_SECTOR 0x0803E800  //- адрес предпредпоследнего сектора STM32F303
#define ADD_126_SECTOR 0x0803F000 // -адрес предпоследнего сектора STM32F303
#define ADD_127_SECTOR 0x0803F800 // - адрес последнего сектора STM32F303 (128 page*(2kb (2048 bytes)))
*/

//========stm32h7
#define FLASH_SECTOR 15			// - номер последнего сектора STM32h743
#define ADD_15_SECTOR 0x081E0000 // - адрес последнего сектора STM32h743
#define BLOCK_SIZE 128000/5		// - число, на которое делим блок на разные нужды

//#define A_FLOAT_SECTOR  ADD_15_SECTOR + BLOCK_SIZE * 0
#define A_SETTINGS  ADD_15_SECTOR + BLOCK_SIZE * 0
//#define A_INT_SECTOR ADD_15_SECTOR + BLOCK_SIZE
//#define A_BUFF_SECTOR ADD_15_SECTOR + BLOCK_SIZE * 6
//#define A_ARR_SECTOR ADD_15_SECTOR + BLOCK_SIZE * 7


#define A_X_DROPCOORD ADD_15_SECTOR + BLOCK_SIZE * 1 //массив координат по 500 штук (100*4байта)
#define A_Y_DROPCOORD ADD_15_SECTOR + BLOCK_SIZE * 2
#define A_Z_DROPCOORD ADD_15_SECTOR + BLOCK_SIZE * 3
#define A_MODE_MOTION ADD_15_SECTOR + BLOCK_SIZE * 4

/*#define A_INT1 A_INT_SECTOR
#define A_INT2 A_INT_SECTOR+32
#define A_INT3 A_INT_SECTOR+32*2
*/
#define STEP_ADDR 32
#define A_FL1 A_FLOAT_SECTOR
#define A_FL2 A_FLOAT_SECTOR+STEP_ADDR
#define A_FL3 A_FLOAT_SECTOR+STEP_ADDR*2
#define A_FL4 A_FLOAT_SECTOR+STEP_ADDR*3
#define A_FL5 A_FLOAT_SECTOR+STEP_ADDR*4
#define A_FL6 A_FLOAT_SECTOR+STEP_ADDR*5
#define A_FL7 A_FLOAT_SECTOR+STEP_ADDR*6
#define A_FL8 A_FLOAT_SECTOR+STEP_ADDR*7
#define A_FL9 A_FLOAT_SECTOR+STEP_ADDR*8
#define A_FL10 A_FLOAT_SECTOR+STEP_ADDR*9
#define A_FL11 A_FLOAT_SECTOR+STEP_ADDR*10
#define A_FL12 A_FLOAT_SECTOR+STEP_ADDR*11
#define A_FL13 A_FLOAT_SECTOR+STEP_ADDR*12
#define A_FL14 A_FLOAT_SECTOR+STEP_ADDR*13
#define A_FL15 A_FLOAT_SECTOR+STEP_ADDR*14
#define A_FL16 A_FLOAT_SECTOR+STEP_ADDR*15
#define A_FL17 A_FLOAT_SECTOR+STEP_ADDR*16

bool read_array_float(float *data, int count, uint32_t addr_array);
bool read_array_int(int *data, int count, uint32_t addr_array);
bool save_array(void *data, int count, uint32_t addr_array);
//bool add_data(void *data, uint32_t data_addr, uint32_t block_addr);
//bool add_data_buf(void *data, uint32_t data_addr, uint32_t block_addr);
bool save_float(uint32_t addr, float fl);
float load_float(uint32_t addr);
uint32_t read_flash32(uint32_t Data_adr);
//bool write_flash64(uint32_t data_adr, uint64_t data);
bool write_flash32(uint32_t data_adr, uint32_t data);
bool flash_earse(uint32_t addr);
bool save_string(uint32_t addr, char *str);
void read_flash_string(uint32_t addr, char * str);
void add_flash_string(uint32_t addr, char * str);
void read_flash_string_limt(uint32_t addr, char * str, int limit);
bool save_string_limit(uint32_t addr, char *str, int limit);
#endif


#endif /* FLASH_H_ */
