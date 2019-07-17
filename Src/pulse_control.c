/*
 * Модуль управления приводами
 */
#include "pulse_control.h"
#include "stm32h7xx_it.h"

#define STEP_PORT_A GPIOB
#define STEP_PIN_A GPIO_PIN_14
#define DIR_PORT_A GPIOE
#define DIR_PIN_A GPIO_PIN_8
#define STEP_PORT_B GPIOB
#define STEP_PIN_B GPIO_PIN_7
#define DIR_PORT_B GPIOE
#define DIR_PIN_B GPIO_PIN_12
#define STEP_PORT_C GPIOB
#define STEP_PIN_C GPIO_PIN_0
#define DIR_PORT_C GPIOE
#define DIR_PIN_C GPIO_PIN_10
#define SYS_CLOCK 240000000 //выставляется настройкой - частота тактирования таймеров

extern TIM_HandleTypeDef htim6;

int counter_period = 10000; //выставляется настройкой - кол-во тактов таймеров в одной секунде (частота драйвера)
//чем выше значение - тем выше потрбеление процессорного времени, но выше точность при высокой скорости
int count_last_pulls = 2; //количество последних толчков с равными интервалами, для выравнивания всего времени движения

uint8_t a_flag_go = 0;
uint8_t b_flag_go = 0;
uint8_t c_flag_go = 0;

int prescaller;//=36000;

int8_t flag_axis_a_busy=0;  //семафорные флаги работы привода
int8_t flag_axis_b_busy=0;
int8_t flag_axis_c_busy=0;

struct axis
{
	uint8_t flag_go;
	uint8_t flag_accel;
	uint8_t flag_brake;
	uint8_t flag_last_pull;
	float cur_speed;
	float accel; //измеряется в pull/s*s
	float brake; //измеряется в pull/s*s
	float max_speed; //измеряется в pull/s
	float min_speed; //измеряется в pull/s
	float time_to_start_brake; //(s)
	float motion_time;
	uint8_t dir;
	unsigned int ticks_to_start_brake;
	int pulls_to_start_brake;
	int target_pulses;
	float target_period;
	float speedadd;
	float speedsub;
	unsigned long sys_tick_start;
};

unsigned long sys_tick;
unsigned long acc_tick;
unsigned long mxsp_tick=0;
extern uint8_t debag_flag;

struct axis a;
struct axis b;
struct axis c;

void timers_ini()
{
	int timer_frec = 2*counter_period;
	prescaller = SYS_CLOCK/timer_frec;
	htim6.Init.Prescaler = prescaller-1;

	HAL_TIM_Base_Init(&htim6);
}

void axis_a_motion_ini(
		int pulses,
		float max_speed,
		float min_speed,
		float accel,
		float brake,
		float  time_to_start_brake,
		float	motion_time,
		uint8_t direction)
{
	a.motion_time = motion_time;
	a.max_speed = max_speed;
	a.min_speed = min_speed;
	a.accel = accel;
	a.brake = brake;
	a.time_to_start_brake = time_to_start_brake;
	a.target_pulses = pulses;
	a.dir = direction;
	a.speedadd=a.accel/counter_period;
	a.speedsub=a.brake/counter_period;
	a.ticks_to_start_brake=(counter_period*a.time_to_start_brake);
	a.pulls_to_start_brake = time_to_start_brake;
	a.flag_accel=1;
	a.sys_tick_start = HAL_GetTick();
	flag_axis_a_busy = 1;
}

void axis_a_timer()
{
	//=====================
	static unsigned long ticks_motion=0;
	static  float tics=0;
	static  float compensator=0;
	static unsigned int pulses=0;

	if (a_flag_go==2)
	{
		ticks_motion=0;
		tics=0;
		compensator=0;
		pulses=0;
		flag_axis_a_busy=0;
		a.cur_speed=0;
		a_flag_go=0;
	}

	if (a_flag_go==0) return;

	ticks_motion++;
	tics++;

	if (a.flag_accel==1)a.cur_speed+=a.speedadd; 	// фаза ускорения

	if (a.cur_speed >=a.max_speed)// фаза равномерного движения
	{
		a.flag_accel=0;
	}

	if (a.flag_accel==0 && a.flag_brake==1) // фаза замедления
	{
		a.cur_speed-=a.speedsub;
	}

	if (a.flag_last_pull==1) // фаза временнОго выравнивания последнего толчка
	{
		a.flag_last_pull=0;
		a.cur_speed=count_last_pulls/(((a.motion_time/*-0.009*/)*counter_period-ticks_motion)/counter_period);/*a.motion_time*/
	}

	a.target_period=counter_period/a.cur_speed;
	//=====================
	a.target_period=a.target_period/2;

	if (tics<a.target_period) return;

	compensator += (tics - a.target_period);
	tics=0;

	if (compensator>1.00)
	{
		compensator-=1.00;
		tics+=1.00;
	}

	if (a.dir == 1) HAL_GPIO_WritePin(DIR_PORT_A, DIR_PIN_A, GPIO_PIN_RESET);
	else if (a.dir == 0) HAL_GPIO_WritePin(DIR_PORT_A, DIR_PIN_A, GPIO_PIN_SET);

	static uint8_t pin_flag=0;

	if (pin_flag == 0)
	{
		HAL_GPIO_WritePin(STEP_PORT_A, STEP_PIN_A, GPIO_PIN_SET);
		pin_flag=1;
		pulses++;
		if (pulses == a.pulls_to_start_brake-1){a.flag_brake = 1;}
		if (pulses == a.target_pulses-count_last_pulls){a.flag_brake = 0;a.flag_last_pull = 1;}
		if (pulses == a.target_pulses)
		{
			ticks_motion=0;
		}
	}
	else
	{
		HAL_GPIO_WritePin(STEP_PORT_A, STEP_PIN_A, GPIO_PIN_RESET);
		pin_flag=0;

		if (pulses == a.target_pulses)
		{
			a_flag_go=0;
			pulses = 0;
			tics=0;
			flag_axis_a_busy=0;
			a.cur_speed=0;
		}
	}
}

void axis_b_motion_ini(
		int pulses,
		float max_speed,
		float min_speed,
		float accel,
		float brake,
		float  time_to_start_brake,
		float	motion_time,
		uint8_t direction)
{
	b.motion_time = motion_time;
	b.max_speed = max_speed;
	b.min_speed = min_speed;
	b.accel = accel;
	b.brake = brake;
	b.time_to_start_brake = time_to_start_brake;
	b.target_pulses = pulses;
	b.dir = direction;
	b.speedadd=b.accel/counter_period;
	b.speedsub=b.brake/counter_period;
	b.ticks_to_start_brake=(counter_period*b.time_to_start_brake);
	b.pulls_to_start_brake = time_to_start_brake;
	b.flag_accel=1;
	b.sys_tick_start = HAL_GetTick();
	flag_axis_b_busy = 1;
}

void axis_b_timer()
{
	static unsigned long ticks_motion=0;
	static  float tics=0;
	static  float compensator=0;
	static unsigned int pulses=0;

	if (b_flag_go==2)
	{
		ticks_motion=0;
		tics=0;
		compensator=0;
		pulses=0;
		flag_axis_b_busy=0;
		b.cur_speed=0;
		b_flag_go=0;
	}

	if (b_flag_go==0) return;

	//=====================
	ticks_motion++;
	tics++;

	if (b.flag_accel==1)b.cur_speed+=b.speedadd; 	// фаза ускорения
	if (b.cur_speed >=b.max_speed)// фаза равномерного движения
	{
		b.flag_accel=0;
	}


	if (b.flag_accel==0 && b.flag_brake==1 ) // фаза замедления
	{
		b.cur_speed-=b.speedsub;
	}
	if (b.flag_last_pull==1) // фаза временнОго выравнивания последнего толчка
	{
		b.flag_last_pull=0;
		b.cur_speed=count_last_pulls/(((b.motion_time/*-0.009*/)*counter_period-ticks_motion)/counter_period);/*a.motion_time*/
	}

	b.target_period=counter_period/b.cur_speed;
	//=====================

	b.target_period=b.target_period/2;

	if (tics<b.target_period) return;

	compensator += (tics - b.target_period);
	tics=0;

	if (compensator>1.00)
	{
		compensator-=1.00;
		tics+=1.00;
	}

	if (b.dir == 1) HAL_GPIO_WritePin(DIR_PORT_B, DIR_PIN_B, GPIO_PIN_RESET);
	else if (b.dir == 0) HAL_GPIO_WritePin(DIR_PORT_B, DIR_PIN_B, GPIO_PIN_SET);

	static uint8_t pin_flag=0;

	if (pin_flag == 0)
	{
		HAL_GPIO_WritePin(STEP_PORT_B, STEP_PIN_B, GPIO_PIN_SET);
		pin_flag=1;
		pulses++;
		if (pulses == b.pulls_to_start_brake-1){b.flag_brake = 1;}
		if (pulses == b.target_pulses-count_last_pulls){b.flag_brake = 0;b.flag_last_pull = 1;}
		if (pulses == b.target_pulses)
		{
			ticks_motion=0;
		}
	}
	else
	{
		HAL_GPIO_WritePin(STEP_PORT_B, STEP_PIN_B, GPIO_PIN_RESET);
		pin_flag=0;

		if (pulses == b.target_pulses)
		{
			b_flag_go=0;
			pulses = 0;
			tics=0;
			flag_axis_b_busy=0;
			b.cur_speed=0;
		}
	}
}

void axis_c_motion_ini(
		int pulses,
		float max_speed,
		float min_speed,
		float accel,
		float brake,
		float  time_to_start_brake,
		float	motion_time,
		uint8_t direction)
{
	c.motion_time = motion_time;
	c.max_speed = max_speed;
	c.min_speed = min_speed;
	c.accel = accel;
	c.brake = brake;
	c.time_to_start_brake = time_to_start_brake;
	c.target_pulses = pulses;
	c.dir = direction;
	c.speedadd=b.accel/counter_period;
	c.speedsub=b.brake/counter_period;
	c.ticks_to_start_brake=(counter_period*c.time_to_start_brake);
	c.pulls_to_start_brake = time_to_start_brake;
	c.flag_accel=1;
	c.sys_tick_start = HAL_GetTick();
	flag_axis_c_busy = 1;
}

void axis_c_timer()
{
	static unsigned long ticks_motion=0;
	static  float tics=0;
	static  float compensator=0;
	static unsigned int pulses=0;

	if (c_flag_go==2)
	{
		ticks_motion=0;
		tics=0;
		compensator=0;
		pulses=0;
		flag_axis_c_busy=0;
		c.cur_speed=0;
		c_flag_go=0;
	}

	if (c_flag_go==0) return;

	//=====================
	ticks_motion++;
	tics++;

	if (c.flag_accel==1)c.cur_speed+=c.speedadd; 	// фаза ускорения
	if (c.cur_speed >=c.max_speed)// фаза равномерного движения
	{
		c.flag_accel=0;
	}

	if (c.flag_accel==0 && c.flag_brake==1) // фаза замедления
	{
		c.cur_speed-=c.speedsub;
	}

	if (c.flag_last_pull==1) // фаза временнОго выравнивания последнего толчка
	{
		c.flag_last_pull=0;
		c.cur_speed=count_last_pulls/(((c.motion_time/*-0.009*/)*counter_period-ticks_motion)/counter_period);/*a.motion_time*/
	}

	c.target_period=counter_period/c.cur_speed;
	//=====================

	c.target_period=c.target_period/2;

	if (tics<c.target_period) return;

	compensator += (tics - c.target_period);
	tics=0;

	if (compensator>1.00)
	{
		compensator-=1.00;
		tics+=1.00;
	}

	if (c.dir == 1) HAL_GPIO_WritePin(DIR_PORT_C, DIR_PIN_C, GPIO_PIN_RESET);
	else if (c.dir == 0) HAL_GPIO_WritePin(DIR_PORT_C, DIR_PIN_C, GPIO_PIN_SET);

	static uint8_t pin_flag=0;

	if (pin_flag == 0)
	{
		HAL_GPIO_WritePin(STEP_PORT_C, STEP_PIN_C, GPIO_PIN_SET);
		pin_flag=1;
		pulses++;
		if (pulses == c.pulls_to_start_brake-1){c.flag_brake = 1;}
		if (pulses == c.target_pulses-count_last_pulls){c.flag_brake = 0;c.flag_last_pull = 1;}
		if (pulses == c.target_pulses)
		{
			ticks_motion=0;
		}
	}
	else
	{
		HAL_GPIO_WritePin(STEP_PORT_C, STEP_PIN_C, GPIO_PIN_RESET);
		pin_flag=0;

		if (pulses == c.target_pulses)
		{
			c_flag_go=0;
			pulses = 0;
			tics=0;
			flag_axis_c_busy=0;
			c.cur_speed=0;
		}
	}
}
