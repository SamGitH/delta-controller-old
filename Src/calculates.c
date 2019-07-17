/*
 * Модуль служит для расчёта времени замедления и ускорения,
 * а также для перерасчёта декартовой системы координат в положение кареток на лучах
 */
#include "calculates.h"
#include "pulse_control.h"
#include "math.h"
#include "serial_interface.h"

float l_rod = 975;

float offsetA[3] = {0,50,0};
float offsetB[3] = {43.3,-25.0,0};
float offsetC[3] = {-43.3,-25.0,0};

float positionA[3] = {0};
float positionB[3] = {0};
float positionC[3] = {0};

float zero_A = 1110.0; //точка от пересечения лучей до машинного нуля
float zero_B = 1110.0;
float zero_C = 1110.0;

float coord_zero_A[3]; //абс координаты машинных нулей
float coord_zero_B[3];
float coord_zero_C[3];

float cur_x, cur_y, cur_z;

uint8_t ini_calc()
{
	calculate_sphere(zero_A,0,0.0,1.0,0.0,1,&coord_zero_A[0],&coord_zero_A[1],&coord_zero_A[2]);
	calculate_sphere(zero_B,-1.732,0.0,-2.0,0.0,2,&coord_zero_B[0],&coord_zero_B[1],&coord_zero_B[2]);
	calculate_sphere(zero_C,1.732,0.0,-2.0,0.0,2,&coord_zero_C[0],&coord_zero_C[1],&coord_zero_C[2]);
	return 1;
}

uint8_t calculate_positions(float x, float y, float z, float *a, float *b, float *c)//Вычисление позиции кареток(a, b, c лучи)
{
	if (z>l_rod) return 0;

	positionA[0] = offsetA[0] + x;
	positionA[1] = offsetA[1] + y;
	positionA[2] = offsetA[2] + z;

	positionB[0] = offsetB[0] + x;
	positionB[1] = offsetB[1] + y;
	positionB[2] = offsetB[2] + z;

	positionC[0] = offsetC[0] + x;
	positionC[1] = offsetC[1] + y;
	positionC[2] = offsetC[2] + z;


	float first_dot_A[3];
	float first_dot_B[3];
	float first_dot_C[3];

	float second_dot_A[3];
	float second_dot_B[3];
	float second_dot_C[3];
	//=============================   Задаем геометрию лучей
	first_dot_A[0] = -positionA[0];
	first_dot_A[1] = -positionA[1];
	first_dot_A[2] = -positionA[2];

	first_dot_B[0] = -positionB[0];
	first_dot_B[1] = -positionB[1];
	first_dot_B[2] = -positionB[2];

	first_dot_C[0] = -positionC[0];
	first_dot_C[1] = -positionC[1];
	first_dot_C[2] = -positionC[2];

	second_dot_A[0] = first_dot_A[0]+0;
	second_dot_A[1] = first_dot_A[1]+1;
	second_dot_A[2] = first_dot_A[2]+1;

	second_dot_B[0] = first_dot_B[0]+0.866;
	second_dot_B[1] = first_dot_B[1]-0.5;
	second_dot_B[2] = first_dot_B[2]+1;

	second_dot_C[0] = first_dot_C[0]-0.866;
	second_dot_C[1] = first_dot_C[1]-0.5;
	second_dot_C[2] = first_dot_C[2]+1;


	float kA, kB,kC, aA, aB, aC, nA, nB, nC, bA, bB, bC;

	kA = (second_dot_A[0]-first_dot_A[0])/(second_dot_A[1]-first_dot_A[1]);
	kB = (second_dot_B[0]-first_dot_B[0])/(second_dot_B[1]-first_dot_B[1]);
	kC = (second_dot_C[0]-first_dot_C[0])/(second_dot_C[1]-first_dot_C[1]);

	aA = first_dot_A[0]-first_dot_A[1]*kA;
	aB = first_dot_B[0]-first_dot_B[1]*kB;
	aC = first_dot_C[0]-first_dot_C[1]*kC;

	nA = (second_dot_A[2]-first_dot_A[2])/(second_dot_A[1]-first_dot_A[1]);
	nB = (second_dot_B[2]-first_dot_B[2])/(second_dot_B[1]-first_dot_B[1]);
	nC = (second_dot_C[2]-first_dot_C[2])/(second_dot_C[1]-first_dot_C[1]);

	bA = first_dot_A[2]-first_dot_A[1]*nA;
	bB = first_dot_B[2]-first_dot_B[1]*nB;
	bC = first_dot_C[2]-first_dot_C[1]*nC;

	float sfA[3]; // координаты каретки относительно начала тяги
	float sfB[3];
	float sfC[3];

	calculate_sphere(l_rod,kA,aA,nA,bA,1,&sfA[0],&sfA[1],&sfA[2]);
	calculate_sphere(l_rod,kB,aB,nB,bB,2,&sfB[0],&sfB[1],&sfB[2]);
	calculate_sphere(l_rod,kC,aC,nC,bC,2,&sfC[0],&sfC[1],&sfC[2]);

	float absA[3];  //абсолютные координаты кареток
	float absB[3];
	float absC[3];

	for (int i=0; i<3; i++)
	{
		absA[i] = sfA[i]+positionA[i];
		absB[i] = sfB[i]+positionB[i];
		absC[i] = sfC[i]+positionC[i];

	}

	if ((coord_zero_A[2]-absA[2]) < 0.0) *a = -sqrt(pow((coord_zero_A[0]-absA[0]),2)+pow((coord_zero_A[1]-absA[1]),2)+pow((coord_zero_A[2]-absA[2]),2));
	else *a = sqrt(pow((coord_zero_A[0]-absA[0]),2)+pow((coord_zero_A[1]-absA[1]),2)+pow((coord_zero_A[2]-absA[2]),2));
	if ((coord_zero_B[2]-absB[2]) < 0.0) *b = -sqrt(pow((coord_zero_B[0]-absB[0]),2)+pow((coord_zero_B[1]-absB[1]),2)+pow((coord_zero_B[2]-absB[2]),2));
	else *b = sqrt(pow((coord_zero_B[0]-absB[0]),2)+pow((coord_zero_B[1]-absB[1]),2)+pow((coord_zero_B[2]-absB[2]),2));
	if ((coord_zero_C[2]-absC[2]) < 0.0) *c = -sqrt(pow((coord_zero_C[0]-absC[0]),2)+pow((coord_zero_C[1]-absC[1]),2)+pow((coord_zero_C[2]-absC[2]),2));
	else *c = sqrt(pow((coord_zero_C[0]-absC[0]),2)+pow((coord_zero_C[1]-absC[1]),2)+pow((coord_zero_C[2]-absC[2]),2));

	cur_x = x;
	cur_y = y;
	cur_z = z;

	return 1;
}

uint8_t calculate_sphere(float R, float k,
		float a, float n,
		float b, uint8_t num_decision,
		float *car_x, float *car_y, float *car_z)//Функция вычисления пересечения прямых со сферой
{
	float y;
	float D;

	D = pow((2*k*a+2*n*b),2)-4*(k*k+n*n+1)*(a*a+b*b-R*R);
	if (D<0.0) return 0;

	if (num_decision == 1) y=(-(2*k*a+2*n*b)+ sqrt(D))/(2*(k*k+n*n+1));
	else if (num_decision == 2) y=(-(2*k*a+2*n*b)- sqrt(D))/(2*(k*k+n*n+1));

	*car_x = k*y+a;
	*car_y = y;
	*car_z = n*y+b;

	return 1;
}

uint8_t calc_impulses(float time, float pulses,
		float accel, float brake,
		uint8_t axsis, float *need_time)//Функция вычисления правила движения коретки
{
	float D;
	float max_speed;
	float time_of_accel;
	float time_of_brake;
	//float time_of_max_speed;
	float pulses_of_accel;
	float pulses_of_brake;
	float pulses_of_max_speed;
	uint8_t direction=0;

	if(pulses<0)
	{
		pulses=pulses*-1.0;
		direction=1;
	}

	D = (4*time*time-8*pulses*((accel+brake)/(accel*brake))); //находим дискриминант.
	if (D<0) //Если он больше 0, то система успеет разогнаться и замедлиться
	{
		*need_time = sqrt((8*pulses*((accel+brake)/(accel*brake)))/4); // возвращаем мин необх времени для данного действия
		return 0;
	}
	else
	{
		max_speed = (2*time-sqrt(D))/(2*(accel+brake)/(accel*brake));
	}

	time_of_accel = max_speed/accel;
	time_of_brake = max_speed/brake;
	//time_of_max_speed = time - time_of_accel - time_of_brake;
	pulses_of_accel = max_speed* time_of_accel/2;
	if (pulses_of_accel<2.0) pulses_of_accel = 2.1;
	pulses_of_brake = max_speed* time_of_brake/2;
	if (pulses_of_brake<2.0) pulses_of_brake = 2.1;
	pulses_of_max_speed = pulses - pulses_of_accel - pulses_of_brake;

	if (axsis == AXSIS_A)
	{
		axis_a_motion_ini(
				pulses,
				max_speed,
				0.1,
				accel,
				brake,
				(pulses_of_accel+pulses_of_max_speed),
				time,
				direction
		);
	}

	else if (axsis == AXSIS_B)
	{
		axis_b_motion_ini(
				pulses,
				max_speed,
				0.1,
				accel,
				brake,
				(pulses_of_accel+pulses_of_max_speed),
				time,
				direction
		);
	}

	else if (axsis == AXSIS_C)
	{
		axis_c_motion_ini(
				pulses,
				max_speed,
				0.1,
				accel,
				brake,
				(pulses_of_accel+pulses_of_max_speed),
				time,
				direction
		);
	}

	return 1;
}
