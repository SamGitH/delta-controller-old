
#ifndef SETTINGS_H_
#define SETTINGS_H_
#include "stm32h7xx_hal.h"


#define UART_TXD(strstr) HAL_UART_Transmit_DMA(&huart1,(uint8_t*)strstr  ,length(strstr))
#define UART_TX(strstr) HAL_UART_Transmit(&huart1,(uint8_t*)strstr  ,length(strstr),0xFFFF)

#define SETTING_SIZE 7
float setting[SETTING_SIZE];

//temp fun
void temp_filarray();
void arrtoset();
void save_data();
void read_table(int n);
uint8_t save_obj_drop_coord();
uint8_t load_obj_drop_coord();
//void save_settings();
void load_settings();


#endif /* SETTINGS_H_ */
