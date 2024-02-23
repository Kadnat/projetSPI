/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
int getSpeed(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define RCC_OSC32_OUT_Pin GPIO_PIN_15
#define RCC_OSC32_OUT_GPIO_Port GPIOC
#define PWM_M1_Pin GPIO_PIN_0
#define PWM_M1_GPIO_Port GPIOC
#define PWM_M2_Pin GPIO_PIN_1
#define PWM_M2_GPIO_Port GPIOC
#define SENS1_M1_Pin GPIO_PIN_2
#define SENS1_M1_GPIO_Port GPIOC
#define SENS2_M1_Pin GPIO_PIN_3
#define SENS2_M1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define SPEED_SENSORM1_Pin GPIO_PIN_4
#define SPEED_SENSORM1_GPIO_Port GPIOC
#define SPEED_SENSORM1_EXTI_IRQn EXTI4_IRQn
#define SPEED_SENSORM2_Pin GPIO_PIN_5
#define SPEED_SENSORM2_GPIO_Port GPIOC
#define PWM_M3_Pin GPIO_PIN_1
#define PWM_M3_GPIO_Port GPIOB
#define SPEED_SENSORM10_Pin GPIO_PIN_10
#define SPEED_SENSORM10_GPIO_Port GPIOB
#define SENS1_M2_Pin GPIO_PIN_6
#define SENS1_M2_GPIO_Port GPIOC
#define SENS2_M2_Pin GPIO_PIN_7
#define SENS2_M2_GPIO_Port GPIOC
#define IR5_Pin GPIO_PIN_9
#define IR5_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define IR1_Pin GPIO_PIN_10
#define IR1_GPIO_Port GPIOC
#define IR2_Pin GPIO_PIN_11
#define IR2_GPIO_Port GPIOC
#define IR3_Pin GPIO_PIN_12
#define IR3_GPIO_Port GPIOC
#define IR4_Pin GPIO_PIN_2
#define IR4_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
