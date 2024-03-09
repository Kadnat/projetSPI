/*
 * servo.c
 *
 *  Created on: Mar 6, 2024
 *      Author: Nathanael
 */

#include "tim.h"
#include "stdint.h"
#include "servo.h"

const uint32_t PSC_Value = 64000000/3276800;
const uint32_t ARR_Value = 64000000/(50.0*(PSC_Value+1))-1;

const uint16_t Period_Min = ARR_Value*(1/20.0);
const uint16_t Period_Max = ARR_Value*(2/20.0);





void SERVO_MoveTo(float af_Angle)
{
	uint16_t au16_Pulse = 0;


	au16_Pulse = ((af_Angle*(Period_Max - Period_Min))/180.0)+ Period_Min;

	TIM1->CCR1 = au16_Pulse;
}
