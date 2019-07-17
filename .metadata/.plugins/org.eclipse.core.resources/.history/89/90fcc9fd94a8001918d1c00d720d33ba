

#ifndef MOTION_CONTROL_H_
#define MOTION_CONTROL_H_

#include "stm32h7xx_hal.h"

void domotion(float x, float y, float z);
void vacuum_on(void);
void vacuum_off (void);
uint8_t soft_limit_ini(void);
uint8_t axis_int_stop(uint8_t axis);
uint8_t prepair_moove_carr(float pos_mm, float time,  uint8_t axis, float *need_time);
void action_moove_carr(uint8_t axis);
uint8_t short_move (float a, float b, float c);
void action_loop();
void set_acceleration(int value);


#endif /* MOTION_CONTROL_H_ */
