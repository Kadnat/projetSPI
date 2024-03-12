/*
 * ESP01.h
 *
 *  Created on: Mar 12, 2024
 *      Author: Nathanael
 */

#ifndef INC_ESP01_H_
#define INC_ESP01_H_

#include "usart.h"
#include "stdbool.h"

#define BUFFER_MAX_SIZE 100
#define RXBUFFER_MAX_SIZE 750

enum STATE {CONNECTED, DISCONNECTED};

typedef struct{

 UART_HandleTypeDef *huart;
 char buffer[BUFFER_MAX_SIZE];
 volatile uint8_t rxbufferIndex;
 volatile uint8_t rxBuffer[RXBUFFER_MAX_SIZE];
 uint8_t byteRx;
 volatile char dataIPD[50];
 int idIPD, lengthIPD;
 enum STATE ServerState;
 bool IPDActive;
}ItemUnit;


void choose_device(ItemUnit *const me, UART_HandleTypeDef *huart);
void __printf(ItemUnit *const me, const char *string);
void TEST_AT(ItemUnit *esp);
void clearBuffer(ItemUnit *const me);
void EspInit(ItemUnit *const esp);
void get_server_command(ItemUnit *esp);

#endif /* INC_ESP01_H_ */
