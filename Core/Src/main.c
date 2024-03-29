/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PID.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//letf motor
uint32_t ICVal3_Left = 0;
int Is_First_Captured_Left=0;
uint32_t ICVal1_Left=0,ICVal2_Left=0,Difference_Left=0;
uint32_t cpt_ms=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  printf("start=0\r\n");
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);

  printf("start=1\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(cpt_ms>=20)
	  {
		  forward(100,110);
		  cpt_ms=0;
	  }








  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{


		if(Is_First_Captured_Left==0)
		{
			ICVal1_Left= HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
			Is_First_Captured_Left=1;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3,TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if(Is_First_Captured_Left==1)
		{
			ICVal2_Left= HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);

			Is_First_Captured_Left=2;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3,TIM_INPUTCHANNELPOLARITY_RISING);

			if(ICVal2_Left>ICVal1_Left)
			{
				Difference_Left = ICVal2_Left-ICVal1_Left; //microsecondes
			}
			else if(ICVal2_Left<ICVal1_Left)
			{
				Difference_Left =  (0xffff-ICVal1_Left)+ICVal2_Left;
			}

		}
		else if(Is_First_Captured_Left==2)
		{
			ICVal3_Left = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
			// Calculer la durée de l'état bas ici
			Difference_Left += ICVal3_Left - ICVal2_Left;
			Is_First_Captured_Left=0;
			__HAL_TIM_SET_COUNTER(htim,0);
			__HAL_TIM_DISABLE_IT(&htim2,TIM_IT_CC3);
		}
	}
}


int getSpeed(void)
{
	int speed=0;
	float duree=0, RPM=0;

	__HAL_TIM_ENABLE_IT(&htim2,TIM_IT_CC3);



	 duree=(float)Difference_Left*(1.0/64000.0);

	 RPM = (60.0*1000.0) / (20.0*duree);

	 speed= RPM;

	return speed;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim6 )
	{
		cpt_ms++;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
