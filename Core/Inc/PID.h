/*
 * PID.h
 *
 *  Created on: Feb 18, 2024
 *      Author: Nathanael
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "stm32f334x8.h"
#include "stdint.h"

void pid_calculation(int r_speed, GPIO_TypeDef * GPIO_PORT, uint16_t GPIO_PIN);
extern uint32_t tickL, tickR, speedL, speedR;
extern int first_front;

#endif /* INC_PID_H_ */
