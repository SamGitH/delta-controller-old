/*
 *Модуль преобразования метрических велечин в имупльсы, проверка софтлимитов
 */

#include "motion_control.h"
#include "calculates.h"
#include "uart.h"
#include "_string.h"

#define PORT_SWITCH_LIMIT_A GPIOB
#define PIN_SWITCH_LIMIT_A GPIO_PIN_12

#define PORT_SWITCH_LIMIT_B GPIOB
#define PIN_SWITCH_LIMIT_B GPIO_PIN_13

#define PORT_SWITCH_LIMIT_C GPIOB
#define PIN_SWITCH_LIMIT_C GPIO_PIN_14

extern uint8_t a_flag_go;
extern uint8_t b_flag_go;
extern uint8_t c_flag_go;

extern uint8_t flag_axis_a_busy;
extern uint8_t flag_axis_b_busy;
extern uint8_t flag_axis_c_busy;

uint8_t limit_switch_a = 0;
uint8_t limit_switch_b = 0;
uint8_t limit_switch_c = 0;

uint8_t ini_a_ok = 0;
uint8_t ini_b_ok = 0;
uint8_t ini_c_ok = 0;


float softlimit_max = 670;
float softlimit_min = 70;

float old_pos[3] = {0};
float k_axis[3] = {2.0, 2.0, 2.0};
float accel[3] = {10000,10000,10000};
float pos_steps[3] = {0};


uint8_t flag_do_move = 0;
extern bool flag_uart_busy;
uint8_t flag_axises_zero = 0;

float axis_a=0, axis_b=0, axis_c=0;

uint8_t flag_action_busy = 0;
bool global_switch_vacuum = 1;
uint8_t flag_safe_mode_motion = 0;

void domotion(float x, float y, float z)
{
	if (calculate_positions(x,y,z, &axis_a, &axis_b, &axis_c))
	{
		flag_do_move = 1;
	}
}

void vacuum_on()//Включение присосок
{
	if (global_switch_vacuum == true) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}

void vacuum_off ()//Отключение присосок
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
}

void action_loop()//Запуск движения кареток
{
	if (flag_do_move == 1)
	{
		if (flag_action_busy == 0)
		{
			flag_action_busy=1;
			int answ = short_move(-axis_a,-axis_b,-axis_c);
			if (answ == 0)
			{
				printstr("log: Out range:");
				if (flag_safe_mode_motion == 1)
				{
					printstr("log: Safe mode. stop work, please reset");
					while (true){}
				}
			}
		}
		if (flag_axis_a_busy==0 && flag_axis_b_busy==0 && flag_axis_c_busy==0)
		{
			flag_uart_busy = false;
			flag_do_move = 0;
			flag_action_busy = 0;
		}
	}
	else if (flag_axises_zero ==1)
	{
		flag_axises_zero = 0;
		flag_uart_busy = false;
		if(soft_limit_ini()==1)
		{
			printstr("Mashine_zero");
		}
	}
}

void set_acceleration(int value)
{
	accel[0] = value;
	accel[1] = value;
	accel[2] = value;
}

uint8_t soft_limit_ini()//Расчёт ограничения движения кареток
{
	float null;
	ini_a_ok = 0;
	ini_b_ok = 0;
	ini_c_ok = 0;
	limit_switch_a = 0;
	limit_switch_b = 0;
	limit_switch_c = 0;

	HAL_Delay(20);

	limit_switch_a = 0;
	limit_switch_b = 0;
	limit_switch_c = 0;

	for (int i=0; i<3;i++)
	{

		calc_impulses(10, softlimit_max*k_axis[i], accel[i]*k_axis[i], accel[0]*k_axis[i], i, &null);
		action_move_carr(i);
	}
	while (ini_a_ok==0 || ini_b_ok==0 || ini_c_ok==0)
	{
		if(limit_switch_a == 1 && ini_a_ok == 0)
		{
			limit_switch_a = 0;
			printstr("ini_axises_a_ok");
			axis_int_stop(0);
			ini_a_ok = 1;

		}
		else if(limit_switch_b == 1 && ini_b_ok == 0)
		{
			limit_switch_b = 0;
			printstr("ini_axises_b_ok");
			axis_int_stop(1);
			ini_b_ok = 1;

		}
		else if(limit_switch_c == 1 && ini_c_ok == 0)
		{
			limit_switch_c = 0;
			printstr("ini_axises_c_ok");
			axis_int_stop(2);

			ini_c_ok = 1;
		}
	}
	if (ini_a_ok == 1 && ini_b_ok == 1 && ini_c_ok == 1)
	{
		printstr("ini_axises_ok");
		return 1;
	}
	else
	{
		printstr("ini_axises_fail");
		return 0;
	}
}

void axis_int_stop(uint8_t axis)
{
	if (axis == 0)
	{
		old_pos[0] = 0;
		a_flag_go = 2; 	//значение 2 обнуляет счетчики импульсов
		flag_axis_a_busy=0;
	}
	else if (axis == 1)
	{
		old_pos[1] = 0;
		b_flag_go = 2;
		flag_axis_b_busy=0;
	}
	else if (axis == 2)
	{
		old_pos[2] = 0;
		c_flag_go = 2;
		flag_axis_c_busy=0;
	}
}

uint8_t prepair_move_carr(float pos_mm, float time,  uint8_t axis, float *need_time)
{
	*need_time = 0;
	pos_steps[axis] = pos_mm*k_axis[axis];
	float steps = pos_steps[axis] - old_pos[axis];

	if (old_pos[axis] == pos_steps[axis])
		return 0;

	return calc_impulses(time, steps,
			accel[axis]*k_axis[axis], accel[axis]*k_axis[axis],
			axis, need_time);
}

void action_move_carr(uint8_t axis)//Переключение флагов движения кареток
{
	old_pos[axis]=pos_steps[axis];

	if (axis == 0)
	{
		a_flag_go = 1;
	}
	else if (axis == 1)
	{
		b_flag_go = 1;
	}
	else if (axis == 2)
	{
		c_flag_go = 1;
	}
}

uint8_t short_move (float a, float b, float c)
{
	float need_time_raw = 0;
	float need_time = 0;
	int steps[3] = {a,b,c};

	if (a<-softlimit_max || a>softlimit_min) return 0; // проверяем, не выходит ли команда за допустимые значения
	if (b<-softlimit_max || b>softlimit_min) return 0;
	if (c<-softlimit_max || c>softlimit_min) return 0;

	for (uint8_t i=0; i<3;i++)
	{
		if (prepair_move_carr(steps[i], need_time,i, &need_time_raw)!=1)
		{
			if (need_time<need_time_raw) need_time = need_time_raw+0.001;
		}
	}

	for (uint8_t i=0; i<3;i++)
	{
		if (prepair_move_carr(steps[i], need_time, i, &need_time_raw)==1)
		{

		}
	}

	for (uint8_t i=0; i<3;i++)
	{
		action_move_carr(i);
	}

	return 1;
}
