#ifndef CALCULATES_H_
#define CALCULATES_H_
#include "stm32h7xx_hal.h"

#define AXSIS_A 0
#define AXSIS_B 1
#define AXSIS_C 2

uint8_t ini_calc(void);
uint8_t calc_impulses(float time, float pulses, float accel, float brake, uint8_t axsis, float *need_time);
uint8_t calculate_positions(float x, float y, float z, float *a, float *b, float *c);
uint8_t calculate_sphere(float R, float k, float a, float n, float b, uint8_t num_decision, float *car_x, float *car_y, float *car_z);

#endif /* CALCULATES_H_ */
