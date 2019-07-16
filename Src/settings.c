#include "settings.h"
#include "flash.h"
#include "uart.h"
#include <stdio.h>
#include "obj_control.h"


#define A_L_ROD A_FL1 //adress lrod = address float #1 etc
#define A_ZEROC A_FL2
#define A_ZBEFORE_CAPTURE A_FL3
#define A_ZCAPTURE A_FL4
#define A_ZPAST_CAPTURE A_FL5
#define A_ZBEFORE_DROP A_FL6
#define A_CAPTURE_LINE A_FL7
#define A_DETECTION_LINE A_FL8
#define A_ZERO_X_LINE A_FL9

#define A_COUNT_DROP_POS A_FL11
#define A_ACCEL_CAPTURED A_FL10



extern UART_HandleTypeDef huart1;

extern float 	l_rod,
				zero_C,
				z_before_capture,
				z_capture,
				z_past_capture,
				z_before_drop,
				capture_line,
				detection_line;

extern float 		obj_drop_x[ARRAY_ACTIONS_SIZE],
				  	obj_drop_y[ARRAY_ACTIONS_SIZE],
					obj_drop_z[ARRAY_ACTIONS_SIZE];

extern int mode_motion[ARRAY_ACTIONS_SIZE];

extern 	int accel_captur;

extern float zero_x_line;

void temp_filarray()
{
	for (int i=0; i<ARRAY_ACTIONS_SIZE; i++)
	{
		obj_drop_x[i] = i;
		obj_drop_y[i] = i;
		obj_drop_z[i] = i;
		mode_motion[i] = i;
	}
}
/*flash_earse(A_FLOAT_SECTOR);*/
/*	save_float(A_L_ROD,l_rod);
save_float(A_ZEROC, zero_C);
save_float(A_ZBEFORE_CAPTURE,z_before_capture);
save_float(A_ZCAPTURE,z_capture);
save_float(A_ZPAST_CAPTURE,z_past_capture);
save_float(A_ZBEFORE_DROP,z_before_drop);
save_float(A_DETECTION_LINE, detection_line);
save_float(A_ACCEL_CAPTURED, (float)accel_captur);
//write_flash32(A_ACCEL_CAPTURED, (uint32_t)accel_captur);
save_float(A_ZERO_X_LINE,zero_x_line);
printstr("settings saved");*/

void arrtoset()
{
	l_rod = setting[0];
	zero_C = setting[1];
	z_before_capture = setting[2];
	z_capture = setting[3];
	z_past_capture = setting[4];
	//z_before_drop = setting[5];
	detection_line = setting[5];
	accel_captur = (int)setting[6];
	zero_x_line = setting[7];
}

void settoarr()
{
	 setting[0] = l_rod;
	 setting[1] = zero_C;
	 setting[2] = z_before_capture;
	 setting[3] = z_capture;
	 setting[4] = z_past_capture;
	// setting[5] = z_before_drop;
	 setting[5] = detection_line;
	 setting[6] = (float)accel_captur;
	 setting[7] = zero_x_line;
}

void save_data()
{
	flash_earse(FLASH_SECTOR);	 //очищаем весь сектор т.к. он один на все сохраняемые данные
	settoarr();
	save_obj_drop_coord();
	save_settings();
	printstr("data eeprom saved");
}

uint8_t save_obj_drop_coord()
{

	if(save_array(obj_drop_x, ARRAY_ACTIONS_SIZE, A_X_DROPCOORD) == 0) return 0;
	if(save_array(obj_drop_y, ARRAY_ACTIONS_SIZE, A_Y_DROPCOORD) == 0) return 0;
	if(save_array(obj_drop_z, ARRAY_ACTIONS_SIZE, A_Z_DROPCOORD) == 0) return 0;
	if(save_array(mode_motion, ARRAY_ACTIONS_SIZE, A_MODE_MOTION) == 0) return 0;
 return true;
}

uint8_t load_obj_drop_coord()
{
	read_array_float(obj_drop_x, ARRAY_ACTIONS_SIZE, A_X_DROPCOORD);
	read_array_float(obj_drop_y, ARRAY_ACTIONS_SIZE, A_Y_DROPCOORD);
	read_array_float(obj_drop_z, ARRAY_ACTIONS_SIZE, A_Z_DROPCOORD);
	read_array_int(mode_motion, ARRAY_ACTIONS_SIZE, A_MODE_MOTION);
}

void save_settings()
{
	if(save_array(setting, SETTING_SIZE, A_SETTINGS) == 0) return 0;
 return true;



}

void load_settings()
{



	read_array_float(setting, SETTING_SIZE, A_SETTINGS);
	arrtoset();
	for (int i = 0; i<SETTING_SIZE; i++)
				{
				char str[50];
				sprintf(str,"setting: %d,%.3f\r",i,setting[i]);
				printstr_(str);
				if (mode_motion[i] == 2) break;
				}

	printstr("load settings:");
	UART_TX("l_rod: ");
	printfloat(l_rod);
	UART_TX("zero_C: ");
	printfloat(zero_C);
	UART_TX("z_before_capture: ");
	printfloat(z_before_capture);
	UART_TX("z_capture: ");
	printfloat(z_capture);
	UART_TX("z_past_capture: ");
	printfloat(z_past_capture);
	UART_TX("z_before_drop: ");
	printfloat(z_before_drop);
	UART_TX("capture_line: ");
	printfloat(capture_line);
	UART_TX("detection_line: ");
	printfloat(detection_line);
	UART_TX("accel_captur: ");
	printint(accel_captur);
	UART_TX("zero_x_line: ");
	printfloat(zero_x_line);



	load_obj_drop_coord();

	printstr("data loaded");

	for (int i = 0; i<ARRAY_ACTIONS_SIZE; i++)
				{
				char str[50];
				sprintf(str,"step: %d,%.3f,%.3f,%.3f,%d\r",i,obj_drop_x[i],obj_drop_y[i],obj_drop_z[i],mode_motion[i]);
				printstr_(str);
				if (mode_motion[i] == 2) break;
				}



}


void read_table(int n)
{
//	for (int i=0 ;i<100; i++)
	//{
		char buf[50];
		sprintf(buf,"stp_req %d\r", n+1);
		printstr_(buf);

	//}
}
