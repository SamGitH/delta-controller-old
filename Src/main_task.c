#include "main_task.h"
#include "calculates.h"
#include "pulse_control.h"
#include "motion_control.h"
#include "uart.h"
#include "uart2.h"
#include "obj_control.h"
#include "flash.h"
#include "settings.h"
#include "serial_interface.h"

uint8_t flag_start = 0;
uint8_t flag_axises_inned = 0;

extern float l_rod;
extern float zero_C;
extern float z_before_capture,//object's operation coordinates
			 z_capture,
			 z_past_capture,
			 z_before_drop;


void main_ini()
{
	uart_ini();
	uart_ini_u2();

	printstr_u2("uart 2 test \n");

	printstr("log:========HELLO========");
	printstr("V0.6");

	//temp_filarray();
	save_data();
  	load_settings();

	/*int test_input[4] ={1,2,3,4};
	int test_output[4];

	 save_array(test_input, 4, A_BUFF_SECTOR);
	 read_array_int(test_output, 4, A_BUFF_SECTOR);

	 for(int i =0; i<4; i++)
	 {
		 printint(test_output[i]);
	 }*/

	timers_ini();
	ini_calc();
	//flag_axises_inned = soft_limit_ini();  //ini machine zero
}

void main_task()
{
	action_loop();
	readATSerialBuff();
	readATSerialBuff_u2();
	put_obj();
	msg_agent();
}
