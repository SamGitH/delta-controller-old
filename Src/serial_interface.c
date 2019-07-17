/*
 * Модуль парсинга входных сообщений
 */
/*
 *команды перемещения начинаются с 'x'
 *дополнительные команды с 'c' (==command)
 *сервисные/настройка команды с 's'
 */

/*
=======================
s - settings

s1 lrod
s2 zero_legth
s3 action_camera_en
s4 acceleration
s5 detection_line
s6 short_move a b c mm
s7 k_axises
s8 z_before_captudre
s9 z_capture
s10 z_past_capture
s11 z_before_drop
s12 capture_line
s13 accel_captur

=======================
c - commands

c1 - axises zero
c2 - manual vacuum on
c3 - manual vacuum off
с4 - auto vacuum on
с5 - auto vacuum off
с6 - auto vacuum off
с7 - show current  array
c8 - write objects drop pos
c9 - show array
 */

#include "main_task.h"
#include "serial_interface.h"
#include "_string.h"
#include "calculates.h"
#include "motion_control.h"
#include "uart.h"
#include "obj_control.h"
#include "settings.h"

extern bool flag_uart_blk;
int flag_parse_serial_ua1 = 0;
extern UART_HandleTypeDef huart1;
char str_rx[SERIAL_BUFSIZE];
char serial_string[150];

extern uint8_t flag_do_move;
extern uint8_t flag_axises_zero;
extern uint8_t flag_camera_en;
bool flag_uart_busy = false;
uint8_t flasg_send_str;
extern uint8_t flag_safe_mode_motion;
extern uint8_t flag_auto; //флаг автоматического захвата и укладки объектов

extern float l_rod;
extern float zero_A; //точка от пересечения лучей до машинного нуля
extern float zero_B;
extern float zero_C;
extern float k_axis[3];
extern long counter_enc;
extern float detection_line;
extern float //object's operation coordinates
z_before_capture,
z_capture,
z_past_capture,
z_before_drop;
extern bool global_switch_vacuum;
extern float capture_line; //650
extern float cur_x, cur_y, cur_z;
extern float obj_drop_x[200],
obj_drop_y[200],
obj_drop_z[200];
extern int mode_motion[200];
extern 	int accel_captur;
extern float zero_x_line;

void parse_commands_machinevision (char *buff_str)
{
	char float_buff[10];

	if(startsWith(buff_str,"o"))
	{
		float x, y;
		uint8_t index_x=indexOf(buff_str,'x');
		uint8_t index_y=indexOf(buff_str,'y');
		uint8_t last_s = length(buff_str);

		substring(buff_str, float_buff,index_x+1,index_y);
		x = atof(float_buff);

		substring(buff_str, float_buff,index_y+1,last_s);
		y = atof(float_buff);

		if (flag_auto == 1) add_object(x,y);
	}
}

void parse_commands (char *buff_str) //user uart
{
	char float_buff[30];

	if(startsWith(buff_str,"x"))
	{
		if (flag_uart_busy == true)
		{
			if (flag_do_move == 1)
				printstr("busy..");
			return;
		}

		float x, y, z;
		uint8_t index_x=indexOf(buff_str,'x');
		uint8_t index_y=indexOf(buff_str,'y');
		uint8_t index_z=indexOf(buff_str,'z');
		uint8_t last_s = length(buff_str);

		substring(buff_str, float_buff,index_x+1,index_y);
		x = atof(float_buff);

		substring(buff_str, float_buff,index_y+1,index_z);
		y = atof(float_buff);

		substring(buff_str, float_buff,index_z+1,last_s);
		z = atof(float_buff);

		flag_safe_mode_motion = 0; //флаг безопасного режима выключен (в случае, если координаты
		//команды будут выходить за пределы доступа, команда будет игнорироваться. При == 1 будет бесконенчый цикл.
		if (flag_auto != 1) domotion(x,y,z);

		return;
	}
	else if(startsWith(buff_str,"s"))
	{
		uint8_t index=indexOf(buff_str,' ');
		uint8_t last_s = length(buff_str);
		substring(buff_str, float_buff, 1,index);
		int set_num = toInt(float_buff);

		if (set_num == 1)
		{
			substring(buff_str, float_buff,index,last_s);
			float value = atof(float_buff);
			l_rod = value;
			printstr("l_rod: ");
			printfloat(value);
		}
		else	if (set_num == 2)
		{
			substring(buff_str, float_buff,index,last_s);
			float value = atof(float_buff);
			zero_A = value;
			zero_B = value;
			zero_C = value;
			ini_calc();
			printstr("zero_legth: ");
			printfloat(value);
		}
		else if (set_num == 3)
		{
			substring(buff_str, float_buff,index,last_s);
			int value = atoi(float_buff);
			flag_camera_en = value;
			printstr("action_camera_en");
			printint(value);
		}
		else if (set_num == 4)
		{
			substring(buff_str, float_buff,index,last_s);
			int value = atoi(float_buff);
			set_acceleration(value);
			printstr("acceleration");
			printint(value);
		}
		else if (set_num == 5)
		{
			substring(buff_str, float_buff,index,last_s);
			float value = atof(float_buff);
			detection_line = value;
			printstr("detection_line");
			printint(value);
		}
		else if (set_num == 6)
		{
			substring(buff_str, float_buff,index,last_s);
			float value = -atof(float_buff);
			short_move(value, value, value);
			printstr("short_move a b c mm");
			printint(-value);
		}
		else if (set_num == 7)
		{
			substring(buff_str, float_buff,index,last_s);
			float value = atof(float_buff);
			k_axis[0]=value;
			k_axis[1]=value;
			k_axis[2]=value;
			printstr("k_axises");
			printfloat(value);
		}
		else if (set_num == 8)
		{
			substring(buff_str, float_buff,index,last_s);
			float value = atof(float_buff);
			z_before_capture=value;
			printstr("z_before_capture");
			printfloat(value);
		}
		else if (set_num == 9)
		{
			substring(buff_str, float_buff,index,last_s);
			float value = atof(float_buff);
			z_capture=value;
			printstr("z_capture");
			printfloat(value);
		}
		else if (set_num == 10)
		{
			substring(buff_str, float_buff,index,last_s);
			float value = atof(float_buff);
			z_past_capture=value;
			printstr("z_past_capture");
			printfloat(value);
		}
		else if (set_num == 11)
		{
			substring(buff_str, float_buff,index,last_s);
			float value = atof(float_buff);
			z_before_drop=value;
			printstr("z_before_drop");
			printfloat(value);
		}
		else if (set_num == 12)
		{
			substring(buff_str, float_buff,index,last_s);
			float value = atof(float_buff);
			capture_line=value;
			printstr("capture_line");
			printfloat(value);
		}
		else if (set_num == 13)
		{
			substring(buff_str, float_buff,index,last_s);
			int value = atoi(float_buff);
			accel_captur=value;
			printstr("accel_captur");
			printfloat(accel_captur);

		}
		else if (set_num == 14)
		{
			substring(buff_str, float_buff,index,last_s);
			float value = atof(float_buff);
			zero_x_line=value;
			printstr("zero_x_line");
			printfloat(zero_x_line);
		}
	}
	else if(startsWith(buff_str,"c"))
	{
		uint8_t index_c=indexOf(buff_str,'c');
		uint8_t last_s = length(buff_str);
		substring(buff_str, float_buff,index_c+1,index_c+3);
		int set_num = toInt(float_buff);
		if (set_num == 1)
		{
			if (flag_auto == 0) flag_axises_zero = 1;
		}
		else if (set_num == 2)
		{
			vacuum_on();
			printstr("manual_vacc_on");
		}
		else if (set_num == 3)
		{
			vacuum_off();
			printstr("manual_vacc_off");
		}
		else if (set_num == 4)
		{
			global_switch_vacuum = true;
			printstr("auto_vacc_on");
		}
		else if (set_num == 5)
		{
			global_switch_vacuum = false;
			printstr("auto_vacc_off");
		}
		else if (set_num == 6)
		{
			save_data();
		}
		else if (set_num == 7)
		{
			printstr("prog_seq: ");
			for (int i = 0; ; i++)
			{
				printint(i);
				printfloat(obj_drop_x[i]);
				printfloat(obj_drop_y[i]);
				printfloat(obj_drop_z[i]);
				printint(mode_motion[i]);
				printstr("-----------");
				if (mode_motion[i] == 2) break;
			}
		}
		else if (set_num == 8)
		{
			uint8_t index_n=indexOf(buff_str,' ');
			uint8_t index_f=indexOf(buff_str,'f');
			substring(buff_str, float_buff,index_n+1,index_f);
			int n = atoi(float_buff);
			obj_drop_x[n] = cur_x;
			obj_drop_y[n] = cur_y;
			obj_drop_z[n] = cur_z;
			substring(buff_str, float_buff,index_f+1,last_s);
			int flag = atoi(float_buff);
			mode_motion[n] = flag;

			for (int i =0; i<100; i++)
			{
				char str[50];
				sprintf(str,"step: %d,%.3f,%.3f,%.3f,%d\r",i,obj_drop_x[i],obj_drop_y[i],obj_drop_z[i],mode_motion[i]);
				printstr_(str);
				if (mode_motion[i] == 2) break;
			}
		}
		else if (set_num == 9)
		{
			save_data();
			printstr("array obj drop coords saved");

			for (int i =0; i<100; i++)
			{
				char str[50];
				sprintf(str,"step: %d,%.3f,%.3f,%.3f,%d\r",i,obj_drop_x[i],obj_drop_y[i],obj_drop_z[i],mode_motion[i]);
				printstr_(str);
				if (mode_motion[i] == 2) break;
			}
		}
		else if (set_num == 10)
		{
			uint8_t index_n=indexOf(buff_str,' ');
			uint8_t index_f=indexOf(buff_str,'f');
			uint8_t index_v=indexOf(buff_str,'v');
			uint8_t index_y=indexOf(buff_str,'y');
			substring(buff_str, float_buff,index_n+1,index_f);
			int n = atoi(float_buff);

			substring(buff_str, float_buff,index_f+1,index_v);
			int flag = atoi(float_buff);
			mode_motion[n] = flag;

			substring(buff_str, float_buff,index_v+1,index_y);
			float val = atof(float_buff);
			obj_drop_x[n] = val;

			substring(buff_str, float_buff,index_y+1,last_s);
			val = atof(float_buff);
			obj_drop_y[n] = val;

			for (int i =0; i<100; i++)
			{
				char str[50];
				sprintf(str,"step: %d,%.3f,%.3f,%.3f,%d\r",i,obj_drop_x[i],obj_drop_y[i],obj_drop_z[i],mode_motion[i]);
				printstr_(str);
				if (mode_motion[i] == 2) break;
			}

			if (flag == 10)
			{
				obj_drop_y[n] = 0;
				obj_drop_z[n] = 0;
			}
			else if (flag == 11)
			{
				obj_drop_y[n] = 0;
				obj_drop_z[n] = 0;
			}
			else if (flag == 12)
			{
				obj_drop_z[n] = 0;
			}
		}
		else if (set_num == 13)
		{
			printstr("demo:");
			uint8_t index_n=indexOf(buff_str,' ');
			substring(buff_str, float_buff,index_n+1,last_s);
			int stage = atoi(float_buff);
			printstr_("stage");printint(stage);
			do_demo(stage);
		}
		else if (set_num == 14)
		{
			printstr("V0.5");
			load_settings();
		}
		else if (set_num == 15)
		{
			uint8_t index_n=indexOf(buff_str,' ');
			substring(buff_str, float_buff,index_n+1,last_s);
			for (int i =0; i<ARRAY_ACTIONS_SIZE; i++)
			{
				char str[50];
				sprintf(str,"step: %d,%.3f,%.3f,%.3f,%d\r",i,obj_drop_x[i],obj_drop_y[i],obj_drop_z[i],mode_motion[i]);
				printstr_(str);
				if (mode_motion[i] == 2) break;
			}
		}
		else if (set_num == 16)
		{
			uint8_t index_n=indexOf(buff_str,' ');
			uint8_t index_x=indexOf(buff_str,'x');
			uint8_t index_y=indexOf(buff_str,'y');
			uint8_t index_z=indexOf(buff_str,'z');
			uint8_t index_f=indexOf(buff_str,'f');

			substring(buff_str, float_buff,index_n+1,index_x);
			int n = atoi(float_buff);

			substring(buff_str, float_buff,index_x+1,index_y);
			float val = atof(float_buff);
			obj_drop_x[n] = val;

			substring(buff_str, float_buff,index_y+1,index_z);
			val = atof(float_buff);
			obj_drop_y[n] = val;

			substring(buff_str, float_buff,index_z+1,index_f);
			val = atof(float_buff);
			obj_drop_z[n] = val;

			substring(buff_str, float_buff,index_f+1,last_s);
			int flag = atoi(float_buff);
			mode_motion[n] = flag;

			if (n<100 && flag != 2)
			{
				char buf[50];
				sprintf(buf,"stp_req: %d\r", n+1);
				printstr_(buf);
			}
		}
		else if (set_num == 17)
		{
			char str[50];
			sprintf(str,"curpos: %.3f,%.3f,%.3f\r",cur_x,cur_y,cur_z);
			printstr_(str);
		}
		else if (set_num == 18)
		{
			reset_seq_steps();
			printstr("reset_seq_steps");
		}
		else if (set_num == 19)
		{
			uint8_t index_n=indexOf(buff_str,' ');

			substring(buff_str, float_buff,index_n+1,last_s);
			flag_auto = atoi(float_buff);
			char str[50];
			sprintf(str,"auto_mode: %d\r",flag_auto);
			printstr_(str);
			sprintf(str,"log: flag auto: %d\r",flag_auto);
			printstr_(str);
		}
		else if (set_num == 20)
		{
			uint8_t index_n=indexOf(buff_str,' ');
			substring(buff_str, float_buff,index_n+1,last_s);

			for (int i =0; i<SETTING_SIZE; i++)
			{
				char str[50];
				sprintf(str,"setting: %d,%.6f\r",i,setting[i]);
				printstr_(str);
			}
		}
		else if (set_num == 21)
		{
			uint8_t index_n=indexOf(buff_str,' ');
			uint8_t index_x=indexOf(buff_str,'s');
			substring(buff_str, float_buff,index_n+1,index_x);
			int n = atoi(float_buff);
			substring(buff_str, float_buff,index_x+1,last_s);
			float val = atof(float_buff);
			setting[n] = val;
			if (n<SETTING_SIZE)
			{
				char buf[50];
				sprintf(buf,"sett_req: %d\r", n+1);
				printstr_(buf);
				if(n == SETTING_SIZE - 1) arrtoset(); //переносим из массива в переменные (костыль)
			}
		}
	}
	else if(startsWith(buff_str,"o"))
	{
		float x, y;
		uint8_t index_x=indexOf(buff_str,'x');
		uint8_t index_y=indexOf(buff_str,'y');
		uint8_t last_s = length(buff_str);

		substring(buff_str, float_buff,index_x+1,index_y);
		x = atof(float_buff);

		substring(buff_str, float_buff,index_y+1,last_s);
		y = atof(float_buff);
		if (flag_auto == 1)add_object(x,y);
	}
	clearArray(float_buff,10);
	flag_parse_serial_ua1 = BP_ERROR;
}

void msg_agent()
{
	if (flasg_send_str==1)
	{
		UART_TXD(serial_string);
		flasg_send_str = 0;
	}
}
