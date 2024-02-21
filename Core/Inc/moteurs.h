/*
 * moteurs.h
 *
 *  Created on: Feb 21, 2024
 *      Author: Nathanael
 */

#ifndef INC_MOTEURS_H_
#define INC_MOTEURS_H_

#include "stdint.h"

void forward(uint8_t speedL, uint8_t speedR);
void backward(uint8_t speedL, uint8_t speedR);
void stop(void);
void left_motor_control(uint8_t speed, uint8_t sens);
void right_motor_control(uint8_t speed, uint8_t sens);


#endif /* INC_MOTEURS_H_ */
