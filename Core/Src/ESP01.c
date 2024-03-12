/*
 * ESP01.c
 *
 *  Created on: Mar 9, 2024
 *      Author: Nathanael
 */

#include "ESP01.h"
#include "stdio.h"

void choose_device(ItemUnit *const me, UART_HandleTypeDef *huart) {
	me->huart = huart;

}

void __printf(ItemUnit *const me, const char *string) {
	if (string[0] != '\0') {

		sprintf(me->buffer, string);
		HAL_UART_Transmit(me->huart, (uint8_t*) me->buffer, strlen(me->buffer),
				1000);
		HAL_Delay(50);
		memset(me->buffer, 0, sizeof(me->buffer));

	}
}

void TEST_AT(ItemUnit *esp) {

	uint8_t ESPEventCase = 0, stateTry = 0;

	while (ESPEventCase == 0) {
		__printf(esp, "AT\r\n");
		HAL_Delay(500);

		if (strstr((char*) (esp->rxBuffer), "OK") != NULL) {
			ESPEventCase = 1;
		}

		if (strstr((char*) (esp->rxBuffer), "OK") == NULL && stateTry == 5) {
			stateTry = 0;
			HAL_Delay(500);
		}
		stateTry++;
		clearBuffer(esp);
	}
}


void EspInit(ItemUnit *const esp) {
	uint8_t ESPInitCase = 0;
	while (ESPInitCase == 0) {
		__printf(esp, "AT\r\n");
		if (strstr((char*) (esp->rxBuffer), "OK") != NULL) {

			ESPInitCase = 1;
		}

		clearBuffer(esp);
	}
	while (ESPInitCase == 1) {
		__printf(esp, "AT+CIPMUX?\r\n");
		HAL_Delay(1500);

		if (strstr((char*) (esp->rxBuffer), "+CIPMUX:1") != NULL) {

			ESPInitCase = 2;
		} else {

			__printf(esp, "AT+CIPMUX=1\r\n");
			HAL_Delay(700);
			__printf(esp, "AT+CIPSERVER=1,80\r\n");
		}
		clearBuffer(esp);
	}


}

void clearBuffer(ItemUnit *const me)
{
	memset((char*) me->rxBuffer, 0, strlen((char*) me->rxBuffer));
	me->rxbufferIndex = 0;
}


void get_server_command(ItemUnit *esp){
    char tempData[50]; // Tableau intermédiaire pour stocker les données

    if(esp->ServerState == CONNECTED){
    	if(esp->IPDActive == true){


			if (strstr((char*) (esp->rxBuffer), "IPD") != NULL) {
				sscanf((char*)esp->rxBuffer, "\r\n+IPD,%d,%d:%s\r\n", &(esp->idIPD), &(esp->lengthIPD), tempData);
				// Si lengthIPD est supérieur à 2, alors nous avons des données valides
				if(esp->lengthIPD > 2){
					esp->lengthIPD -= 2; // Enlever 2 à la taille de length pour prendre en compte uniquement la data
					strcpy(esp->dataIPD, tempData); // Copier les données du tableau intermédiaire à dataIPD
					// Maintenant, "idIPD" contient l'ID de connexion, "lengthIPD" contient la taille des données, et "dataIPD" contient les données extraites.
				}
			}
			clearBuffer(esp);
    	}
    }
}


void get_server_status(ItemUnit *esp){
	if (strstr((char*) (esp->rxBuffer), "CONNECT") != NULL){
		esp->ServerState = CONNECTED;
		clearBuffer(esp);
	}
	else if(strstr((char*) (esp->rxBuffer), "CLOSED") != NULL){
		esp->ServerState = DISCONNECTED;
		clearBuffer(esp);
	}

}


