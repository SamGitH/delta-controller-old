#ifndef PULSE_CONTROL_H_

#define PULSE_CONTROL_H_

#include "stm32h7xx_hal.h"

void timers_ini(void);

void axis_a_timer(void);

void axis_a_motion_ini(
		int pulses,
		float max_speed,
		float min_speed,
		float accel,
		float brake,
		float  time_to_start_brake,
		float	motion_time,
		uint8_t direction
);

void axis_b_timer(void);

void axis_b_motion_ini(
		int pulses,
		float max_speed,
		float min_speed,
		float accel,
		float brake,
		float  time_to_start_brake,
		float	motion_time,
		uint8_t direction
);

void axis_c_timer(void);

void axis_c_motion_ini(
		int pulses,
		float max_speed,
		float min_speed,
		float accel,
		float brake,
		float  time_to_start_brake,
		float	motion_time,
		uint8_t direction
);

#endif /* PULSE_CONTROL_H_ */
