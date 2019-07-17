#include "obj_control.h"
#include "uart.h"
#include "stm32h7xx_hal.h"
#include "calculates.h"
#include "motion_control.h"


#include <stdio.h>


#define N_OBJ 25

float obj_x[N_OBJ];
float obj_y[N_OBJ];
uint8_t obj_en[N_OBJ] = {0};


uint8_t flag_camera_en = 1;

extern uint8_t flag_do_move;


//settings vars
float encoder_k = 0.03332*2;
static int enc_prescaler = 10;
float detection_line = -655.0;

float capture_line = -290.0;
float lost_line = 280.0;

float add_y_dist = 30;//добавка перед захватом

//float h_prepare = -200;

float max_y = 140;  //ограничения по ширине ленты
float min_y = -140;

float zero_x_line = -219;


float z_before_capture = -90,
			z_capture = -145,
			z_past_capture = -90,
			z_before_drop = -20;

int obj_counter = 0;
int obl_losted_counter = 0;

uint8_t flag_auto = 0;
uint8_t block_move = 0; //блокировка перемещения, если происходит операция укладки объекта

//===debug_vars
long counter_enc = 0;
extern char serial_string[150];
extern uint8_t flasg_send_str;


void add_object(float x, float y) //функция добавления массива
{
	static int num_objs = 0;
	if (y>max_y || y<min_y)
	{
		sprintf(serial_string,"log: obj out_of_range %.2f,%.2f\r",x,y);
		flasg_send_str = 1;
		return;
	}

	if (obj_en[num_objs]==0)
	{
	obj_x[num_objs] = y+zero_x_line;
	obj_y[num_objs] = x+detection_line;//ukazivaem polojenie zamechennogo objecta otnositelno koord delty


		//sprintf(string,"log: obj %.2f,%.2f\r",x,y);
//		printstrdma(string);

	uint8_t flag_en_obj = 0;
	for (int i = 0; i < N_OBJ; i++) //проходим по всему массиву, что бы выяснить, есть ли активные цели (obj_en == 1)
	{
		if(obj_en[i]==1) flag_en_obj = 1; //если есть хоть один, то поменяем флаг
	}
	if (flag_en_obj == 0) //если нет других активных объектов, переведем каретку в режим готовности
	{
		 if (flag_auto == 1 && block_move == 0) domotion(obj_x[num_objs], capture_line, z_before_capture);	//..переведем карету
	}
	/*	printstr("Add obj");
	printint(num_objs);
	printfloat(obj_y[num_objs]);
	printfloat(obj_x[num_objs]);

	printstr("======");*/
	obj_en[num_objs]=1; //peremennaya nalichiya objecta
	num_objs++;
	if (num_objs==N_OBJ)num_objs=0;
  }
	else
	{
		sprintf(serial_string,"log: obj buffer overflowed\r");
		flasg_send_str = 1;
	}
}


static int enc_counter = 0;
uint8_t flag_action_end = 0;
int temp_i_obj;
int i_curr_obj = -2;
void encoder_pulce()
{
	counter_enc++;
	float max_y_obj = -10000;
	int i_max_y_obj = -2;
	enc_counter++;
	if (enc_counter<enc_prescaler) return;
	enc_counter=0;

		for (int i=0; i<N_OBJ; i++)
	{
		if (obj_en[i]==1)
		{
			obj_y[i]+=encoder_k*(float)enc_prescaler;

			if (obj_y[i]>lost_line) // esli object vne zoni deistv
			{
				obj_en[i]=0; //udalyaem object
				obl_losted_counter++;


				sprintf(serial_string,"log: obj losted %d\r",obl_losted_counter);
		    flasg_send_str = 1;

			//	printstr("log: obj losted\r");
				continue;
			}

				if (obj_y[i]>max_y_obj)
				{
					max_y_obj = obj_y[i];
					i_max_y_obj = i;
				}
		}
	}
	if (i_max_y_obj!=-2 && obj_y[i_max_y_obj]>capture_line && flag_auto == 1 ) obj_action(i_max_y_obj);
	i_curr_obj = i_max_y_obj;
 }



int flag_put_obj = -2;
uint8_t put_obj_busy = 0;
void obj_action(int i)
{

	if (flag_camera_en && flag_do_move == 0 && put_obj_busy == 0)
	{
		if (flag_put_obj==-2)
		{
			flag_put_obj = i; //peredaem v funkciy nomer objecta. Funkciya rabotaet v cycle, zapustitsya avtomatich
		}
	}
}


	    float   obj_drop_x[ARRAY_ACTIONS_SIZE],
				obj_drop_y[ARRAY_ACTIONS_SIZE],
				obj_drop_z[ARRAY_ACTIONS_SIZE];

	int mode_motion[ARRAY_ACTIONS_SIZE];

	int accel_captur = 10000;

	extern uint8_t flag_safe_mode_motion;


	static int state_package = 0;

void put_obj()
{
	static int local_i = 0;
	static int state = 0 ;
	if (flag_put_obj==-2)return;

		if (state == 0)  // podhod zaxvata nad cel'y
	{
		 put_obj_busy = 1;
		 flag_safe_mode_motion = 1;

		if (flag_do_move == 0)
		{

		local_i = flag_put_obj;

			set_acceleration(accel_captur);
			domotion(obj_x[local_i], obj_y[local_i], z_before_capture);
      temp_i_obj = flag_put_obj;

	  	state++;
  	}

	}
	else if (state == 1)	// zaxvat cely
	{

		if (flag_do_move == 0)
		{
			block_move = 1;
		vacuum_on();
		domotion(obj_x[local_i], obj_y[local_i], z_capture);
	  state++;

  	}

	}
	else if (state == 2) // podjem
	{
		if (flag_do_move == 0)
		{
			domotion(obj_x[local_i], obj_y[local_i], z_past_capture);
		  obj_en[temp_i_obj]=0;
	  	state++;
  	}

	}
	else if (state == 3)	// peremrshenie v предварительную точку разгрузки
	{

	//	if (flag_do_move == 0)
	//		{
		//			 domotion(obj_drop_x[0],obj_drop_y[0] , obj_drop_z[0]);
					 state++;
		//	}
	}
		else if (state == 4)	// peremrshenie v tochky razgruzki
	{

		if (flag_do_move == 0)
			{

					if(mode_motion[state_package] == 0) //перемещение и переход к сл. действию
					{
					translate_step(state_package);
					domotion(obj_drop_x[state_package],obj_drop_y[state_package] , obj_drop_z[state_package]);
					state_package++;


					}
          else if(mode_motion[state_package] == 1) //Конец операции, переход к сл. стадии
					{

				//		domotion(obj_drop_x[state_package],obj_drop_y[state_package] , obj_drop_z[state_package]);
						translate_step(state_package);
						state++;
						block_move = 0;
						state_package++;

					}
					else if(mode_motion[state_package] == 2) //Конец цикла, переход к первой команде
					{
					translate_step(state_package);
       //     domotion(obj_drop_x[state_package],obj_drop_y[state_package] , obj_drop_z[state_package]);
						state_package=0;
						state++;
						block_move = 0;
					}
					else if(mode_motion[state_package] == 10) //Изменить ускорение. Значение хранится в переменной X
					{
					translate_step(state_package);

						set_acceleration((int)obj_drop_x[state_package]);
						state_package++;

					}
					else if(mode_motion[state_package] == 11) //Включить задержку.Зачение хранится в переменной X
					{

						static long timer;
						static uint8_t flag = 0;
						if (flag == 0)
						{
							translate_step(state_package);
							timer = HAL_GetTick();
							flag = 1;
						}
						else
						{
							if (timer+(long)obj_drop_x[state_package]<HAL_GetTick())
							{
							flag = 0;
							state_package++;
							}
						}

					}
					else if(mode_motion[state_package] == 12) //Управление вакуумом
					{

						if ((int)obj_drop_x[state_package]==1) vacuum_on();
						else if ((int)obj_drop_x[state_package]==0) vacuum_off();

						static long timer;
						static uint8_t flag = 0;
						if (flag == 0)
						{
							translate_step(state_package);
							timer = HAL_GetTick();
							flag = 1;
						}
						else
						{
							if (timer+(long)obj_drop_y[state_package]<HAL_GetTick())
							{
					  	translate_step(state_package);
							flag = 0;
							state_package++;

							}
						}

					}
			}
	}
	else if (state == 5)	// peremrshenie v tochku zahvata sled cely
	{
		if (flag_do_move == 0)
			{

				//vacuum_off();
				//HAL_Delay(100);
		float x,y;
				if (obj_y[i_curr_obj]>detection_line && obj_y[i_curr_obj]<capture_line && obj_en[i_curr_obj]==1) //если объект в очереди, но еще не доступен для захвата
		{
			y = capture_line; //подводим устройство к линии захвата
			x = obj_x[i_curr_obj];
		}
    else if (obj_y[i_curr_obj]>detection_line && obj_en[i_curr_obj]==1) //если объект в зоне захвата
		{
			y = obj_y[i_curr_obj]+add_y_dist; //подводим непосредственно к объекту и вычитаем добавку
			x = obj_x[i_curr_obj];
		}
  		else
		{
		y = capture_line;//в остальных случаях просто оставляем над лентой
		x = zero_x_line;
		}
	/*	printstr("x: ");
		printfloat(obj_x[i_curr_obj]);
		printstr("y: ");
		printfloat(y);
		printstr("=======");*/
		domotion(x, y, z_before_capture);
		state++;
  	}
	}

		else if (state == 6)
	{
		if (flag_do_move == 0)
			{
			state = 0;
			put_obj_busy = 0;
	  	flag_put_obj=-2;
			obj_counter++ ;
			sprintf(serial_string,"obj_count: %d\r",obj_counter);
		  flasg_send_str = 1;
			}
	}
}

void do_demo(int state_package)
{
	        printfloat(obj_drop_x[state_package]);
					printfloat(obj_drop_y[state_package]);
					printfloat(obj_drop_z[state_package]);
					printint(mode_motion[state_package]);
					printstr("-----------");

	if (flag_do_move == 0)
			{

					if(mode_motion[state_package] == 0) //перемещение и переход к сл. действию
					{
					translate_step(state_package);
					domotion(obj_drop_x[state_package],obj_drop_y[state_package] , obj_drop_z[state_package]);

					}
          else if(mode_motion[state_package] == 1) //Конец операции, переход к сл. стадии
					{
				//		domotion(obj_drop_x[state_package],obj_drop_y[state_package] , obj_drop_z[state_package]);
						translate_step(state_package);


					}
					else if(mode_motion[state_package] == 2) //Конец цикла, переход к первой команде
					{
            //domotion(obj_drop_x[state_package],obj_drop_y[state_package] , obj_drop_z[state_package]);
						translate_step(state_package);

					}
					else if(mode_motion[state_package] == 10) //Изменить ускорение. Значение хранится в переменной X
					{
						translate_step(state_package);
						set_acceleration((int)obj_drop_x[state_package]);

					}
					else if(mode_motion[state_package] == 11) //Включить задержку.Зачение хранится в переменной X
					{
						translate_step(state_package);
						static long timer;
						static uint8_t flag = 0;
						if (flag == 0)
						{
							timer = HAL_GetTick();
							flag = 1;
						}
						else
						{
							if (timer+(long)obj_drop_x[state_package]<HAL_GetTick())
							{

							flag = 0;

							}
						}

					}
					else if(mode_motion[state_package] == 12) //Управление вакуумом
					{
						translate_step(state_package);

						if ((int)obj_drop_x[state_package]==1) vacuum_on();
						else if ((int)obj_drop_x[state_package]==0) vacuum_off();

						static long timer;
						static uint8_t flag = 0;
						if (flag == 0)
						{
							timer = HAL_GetTick();
							flag = 1;
						}
						else
						{
							if (timer+(long)obj_drop_y[state_package]<HAL_GetTick())
							{

							flag = 0;

							}
						}

					}
			}
}


void translate_step(int n)
{

	sprintf(serial_string,"cstp: %d\r",n);
	flasg_send_str = 1;
}

void reset_seq_steps()
{
	state_package = 0;
}
