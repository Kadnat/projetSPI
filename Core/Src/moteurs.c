/*
 * moteurs.c
 *
 *  Created on: Feb 18, 2024
 *      Author: Nathanael
 */

#include "PID.h"
#include "stdint.h"
#include "main.h"
#include "tim.h"



void forward(uint8_t speedL, uint8_t speedR)
{
	  /* TIM1 counter enable */


	  HAL_TIM_Base_Start(&htim1);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

	  HAL_GPIO_WritePin(SENS1_M1_GPIO_Port, SENS1_M1_Pin, 1);
	  HAL_GPIO_WritePin(SENS2_M1_GPIO_Port, SENS2_M1_Pin, 0);

	  HAL_GPIO_WritePin(SENS1_M2_GPIO_Port, SENS1_M2_Pin, 1);
	  HAL_GPIO_WritePin(SENS2_M2_GPIO_Port, SENS2_M2_Pin, 0);

		  pid_calculation(speedL, SPEED_SENSORM1_GPIO_Port, SPEED_SENSORM1_Pin);
		  //pid_calculation(speedR, SPEED_SENSORM2_GPIO_Port, SPEED_SENSORM2_Pin);


	  //HAL_TIM_Base_Stop(&htim1);
	  //HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	  //HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);

}

void backward(uint8_t speedL, uint8_t speedR)
{
	  /* TIM1 counter enable */


	  HAL_TIM_Base_Start(&htim1);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

	  HAL_GPIO_WritePin(SENS1_M1_GPIO_Port, SENS1_M1_Pin, 0);
	  HAL_GPIO_WritePin(SENS2_M1_GPIO_Port, SENS2_M1_Pin, 1);

	  HAL_GPIO_WritePin(SENS1_M2_GPIO_Port, SENS1_M2_Pin, 0);
	  HAL_GPIO_WritePin(SENS2_M2_GPIO_Port, SENS2_M2_Pin, 1);


	 		  pid_calculation(speedL, SPEED_SENSORM1_GPIO_Port, SPEED_SENSORM1_Pin);
	 		  pid_calculation(speedR, SPEED_SENSORM2_GPIO_Port, SPEED_SENSORM2_Pin);


	  HAL_TIM_Base_Stop(&htim1);
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
}

void stop(void)
{
	//useless

	  HAL_GPIO_WritePin(SENS1_M1_GPIO_Port, SENS1_M1_Pin, 0);
	  HAL_GPIO_WritePin(SENS2_M1_GPIO_Port, SENS2_M1_Pin, 0);

	  HAL_GPIO_WritePin(SENS1_M2_GPIO_Port, SENS1_M2_Pin, 0);
	  HAL_GPIO_WritePin(SENS2_M2_GPIO_Port, SENS2_M2_Pin, 0);
}

void turn_left(void)
{
	  /* TIM1 counter enable */


	  HAL_TIM_Base_Start(&htim1);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

	  HAL_GPIO_WritePin(SENS1_M1_GPIO_Port, SENS1_M1_Pin, 1);
	  HAL_GPIO_WritePin(SENS2_M1_GPIO_Port, SENS2_M1_Pin, 0);

	  HAL_GPIO_WritePin(SENS1_M2_GPIO_Port, SENS1_M2_Pin, 1);
	  HAL_GPIO_WritePin(SENS2_M2_GPIO_Port, SENS2_M2_Pin, 0);


	 		  pid_calculation(130, SPEED_SENSORM1_GPIO_Port, SPEED_SENSORM1_Pin);
	 		  pid_calculation(170, SPEED_SENSORM2_GPIO_Port, SPEED_SENSORM2_Pin);


	  HAL_TIM_Base_Stop(&htim1);
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
}

void turn_right(void)
{
	  /* TIM1 counter enable */


	  HAL_TIM_Base_Start(&htim1);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

	  HAL_GPIO_WritePin(SENS1_M1_GPIO_Port, SENS1_M1_Pin, 1);
	  HAL_GPIO_WritePin(SENS2_M1_GPIO_Port, SENS2_M1_Pin, 0);

	  HAL_GPIO_WritePin(SENS1_M2_GPIO_Port, SENS1_M2_Pin, 1);
	  HAL_GPIO_WritePin(SENS2_M2_GPIO_Port, SENS2_M2_Pin, 0);


	 		  pid_calculation(170, SPEED_SENSORM1_GPIO_Port, SPEED_SENSORM1_Pin);
	 		  pid_calculation(130, SPEED_SENSORM2_GPIO_Port, SPEED_SENSORM2_Pin);


	  HAL_TIM_Base_Stop(&htim1);
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
}

void left_motor_control(uint8_t speed, uint8_t sens)
{
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);


	HAL_GPIO_WritePin(SENS1_M1_GPIO_Port, SENS1_M1_Pin, sens);
	HAL_GPIO_WritePin(SENS2_M1_GPIO_Port, SENS2_M1_Pin, (sens^1));

	pid_calculation(speed, SPEED_SENSORM1_GPIO_Port, SPEED_SENSORM1_Pin);
}

void right_motor_control(uint8_t speed, uint8_t sens)
{
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);


	HAL_GPIO_WritePin(SENS1_M2_GPIO_Port, SENS1_M2_Pin, sens);
	HAL_GPIO_WritePin(SENS2_M2_GPIO_Port, SENS2_M2_Pin, (sens^1));

	pid_calculation(speed, SPEED_SENSORM2_GPIO_Port, SPEED_SENSORM2_Pin);
}
