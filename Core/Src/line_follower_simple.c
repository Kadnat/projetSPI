/*
 * line_follower_simple.c
 *
 *  Created on: Feb 21, 2024
 *      Author: Nathanael
 */
#include "moteurs.h"
#include "main.h"

void go_forward(void)
{
	if((HAL_GPIO_ReadPin(IR1_GPIO_Port, IR1_Pin)==0) && (HAL_GPIO_ReadPin(IR2_GPIO_Port, IR2_Pin)==0))
	{
		forward(170, 170);
	}
	else if((HAL_GPIO_ReadPin(IR1_GPIO_Port, IR1_Pin)==1) && (HAL_GPIO_ReadPin(IR2_GPIO_Port, IR2_Pin)==0))
	{
		forward(130, 170);
	}
	else if((HAL_GPIO_ReadPin(IR1_GPIO_Port, IR1_Pin)==0) && (HAL_GPIO_ReadPin(IR2_GPIO_Port, IR2_Pin)==1))
	{
		forward(170, 130);
	}
	else if((HAL_GPIO_ReadPin(IR1_GPIO_Port, IR1_Pin)==0) && (HAL_GPIO_ReadPin(IR2_GPIO_Port, IR2_Pin)==1))
	{
		stop();
	}
}
