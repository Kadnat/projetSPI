/*
 * line_follower.c
 *
 *  Created on: Feb 21, 2024
 *      Author: Nathanael
 */

#include "moteurs.h"
#include "main.h"
//PID Variables

	int sensor_read = 0x00000;
	int position;

	float Kp = 0.002; //set up the constants value
	float Ki = 0.001;
	float Kd = 15;
	float Kr = 0;
	int P, I, D, R;
	int lastError = 0;
	int errors[10] = {0,0,0,0,0,0,0,0,0,0};
	int error_sum = 0;
	int last_end = 0;	// 0 -> Left, 1 -> Right
	int last_idle = 0;

	const uint8_t maxspeedr = 150;
	const uint8_t maxspeedl = 150;
	const uint8_t basespeedr = 142;
	const uint8_t basespeedl = 142;
	const int ARR = 1;

	int actives = 0;



	void motor_control (double pos_right, double pos_left)
	{
		if (pos_left < 0 )
		{
			left_motor_control(-(ARR*pos_left), 0);
		}
		else
		{
			left_motor_control(ARR*pos_left, 1);
		}
		if (pos_right < 0 )
		{
			right_motor_control(-(ARR*pos_right), 0);
		}
		else
		{
			right_motor_control(ARR*pos_right, 1);
		}
	}

	void sharp_turn () {

		if (last_idle < 25)
		{
			if (last_end == 1)
				motor_control(-20, 100);
			else
				motor_control(100, -20);
		}
		else
		{
			if (last_end == 1)
				motor_control(-53, 70);
			else
				motor_control(70, -53);
		}
	}

	int sensors5_read ()
	{

		sensor_read = 0x00000;
		int pos = 0;
	  int active = 0;

		if (HAL_GPIO_ReadPin(IR1_GPIO_Port, IR1_Pin)) {
			sensor_read |= 0x00000001;
			pos += 1000;
	    active++;
			last_end = 1;
		}
		if (HAL_GPIO_ReadPin(IR2_GPIO_Port, IR2_Pin)) {
			sensor_read |= 0x00000010;
			pos += 2000;
	    active++;
	  }
		if (HAL_GPIO_ReadPin(IR3_GPIO_Port, IR3_Pin)) {
			sensor_read |= 0x00000100;
			pos += 3000;
	    active++;
	  }
		if (HAL_GPIO_ReadPin(IR4_GPIO_Port, IR4_Pin)) {
			sensor_read |= 0x00001000;
			pos += 4000;
	    active++;
	  }
		if (HAL_GPIO_ReadPin(IR5_GPIO_Port, IR5_Pin)) {
			sensor_read |= 0x00010000;
			pos += 5000;
	    active++;
	    last_end = 0;
	  }


	  actives = active;
		position = pos/active;

		if (actives == 0)
			last_idle++;
		else
			last_idle = 0;

		return pos/active;
	}

	void forward_brake(int pos_right, int pos_left)
	{
		if (actives == 0)
			;
			//sharp_turn();
		else
		  motor_control(pos_right, pos_left);
	}

	void past_errors (int error)
	{
	  for (int i = 9; i > 0; i--)
	      errors[i] = errors[i-1];
	  errors[0] = error;
	}

	int errors_sum (int index, int abs)
	{
	  int sum = 0;
	  for (int i = 0; i < index; i++)
	  {
	    if (abs == 1 & errors[i] < 0)
	      sum += -errors[i];
	    else
	      sum += errors[i];
	  }
	  return sum;
	}

	void PID_control() {
		uint16_t position = sensors5_read();
	  int error = 3000 - position;
		past_errors(error);

	  P = error;
	  I = errors_sum(5, 0);
	  D = error - lastError;
	  R = errors_sum(5, 1);
	  lastError = error;

	  int motorspeed = P*Kp + I*Ki + D*Kd;

	  int motorspeedl = basespeedl + motorspeed - R*Kr;
	  int motorspeedr = basespeedr - motorspeed - R*Kr;

	  if (motorspeedl > maxspeedl)
	    motorspeedl = maxspeedl;
	  if (motorspeedr > maxspeedr)
	    motorspeedr = maxspeedr;

	  printf("motorspeedR=%d\r\n",motorspeedr);
	  printf("motorspeedL=%d\r\n",motorspeedl);
	  forward_brake(motorspeedr, motorspeedl);
	}
