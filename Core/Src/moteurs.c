/*
 * moteurs.c
 *
 *  Created on: Feb 18, 2024
 *      Author: Nathanael
 */

#include "PID.h"
#include "stdint.h"
#include "main.h"

void forward(uint8_t enable)
{
	  /* TIM1 counter enable */


	  HAL_TIM_Base_Start(&htim1);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	  HAL_GPIO_WritePin(SENS1_M1_GPIO_Port, SENS1_M1_Pin, 1);
	  HAL_GPIO_WritePin(SENS2_M1_GPIO_Port, SENS2_M1_Pin, 0);

	  while(enable)
	  {
		  pid_calculation(170, SPEED_SENSORM1_GPIO_Port, SPEED_SENSORM1_Pin);
		  pid_calculation(170, SPEED_SENSORM2_GPIO_Port, SPEED_SENSORM2_Pin);
	  }

	  HAL_TIM_Base_Stop(&htim1);
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

}

void backward(uint8_t enable)
{
	  /* TIM1 counter enable */


	  HAL_TIM_Base_Start(&htim1);
	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	  HAL_GPIO_WritePin(SENS1_M1_GPIO_Port, SENS1_M1_Pin, 0);
	  HAL_GPIO_WritePin(SENS2_M1_GPIO_Port, SENS2_M1_Pin, 1);

	  while(enable)
	 	  {
	 		  pid_calculation(170, SPEED_SENSORM1_GPIO_Port, SPEED_SENSORM1_Pin);
	 		  pid_calculation(170, SPEED_SENSORM2_GPIO_Port, SPEED_SENSORM2_Pin);
	 	  }

	  HAL_TIM_Base_Stop(&htim1);
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void stop(uint8_t enable)
{
	//useless

	  HAL_GPIO_WritePin(SENS1_M1_GPIO_Port, SENS1_M1_Pin, 0);
	  HAL_GPIO_WritePin(SENS2_M1_GPIO_Port, SENS2_M1_Pin, 0);
}
