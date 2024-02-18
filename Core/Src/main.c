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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//ALL the variables have been defined here

static int i = 0;int bit =0;int speed =0;int res =0;
static uint32_t cpt_time=0;

// refernce and pid related constants
int ref_speed = 180;float duty =0;int iteration_time =0;

//errors variables
float previous_error = 0;float current_error = 0;


// variables for the pid function that is actually not a function
float KP = 0.1;int KI = 10;int KD = 1;

// some extra variables working as temporary storage
int input = 0;int integration_sum = 0;

// To Get Current Count
int GETVAL(void){
	 return SysTick->VAL;
}
// To account for Multiple Cycles of Timer
// for more than the period of the timer
void HAL_IncTick(void)
{
	i++;
	cpt_time++;
}


// Timer Start Function
// when called the timer starts counting
void Timer_start_func(void){
	  SysTick->LOAD  = 64000 - 1;                                  /* set reload register */
	  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Systick Interrupt */
	  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
	  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
	                   SysTick_CTRL_TICKINT_Msk   |
	                   SysTick_CTRL_ENABLE_Msk;
}

// Timer Count End Function
// when called the timer stops counting
void Timer_end_func(void){
	 SysTick->CTRL  = 0;
	 i=0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	Timer_start_func();  //
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
  /* USER CODE BEGIN 2 */
  /* TIM1 counter enable */


  HAL_TIM_Base_Start(&htim1);

  /* TIM1 Main Output Enable */
  //HAL_TIM_PWM_Init(&htim1);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_GPIO_WritePin(SENS1_M1_GPIO_Port, SENS1_M1_Pin, 1);
  printf("%d,%d,%d\n", 1, cpt_time, speed);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // reads the current state of PA1
	      	// default function used
	      	if ( HAL_GPIO_ReadPin(SPEED_SENSORM1_GPIO_Port, SPEED_SENSORM1_Pin) == 0){
	      		while(HAL_GPIO_ReadPin(SPEED_SENSORM1_GPIO_Port, SPEED_SENSORM1_Pin) == 0){}

	      		if (res==0){
	      			// res here is a flag to see whether to start counter or to stop it
	      			Timer_start_func();
	      			res=1;
	      		} else if (res==1){

	      			iteration_time = (1 - GETVAL()/64000) + i; //DOWN Counter 168000 to 0 Thats why we subtract

	      			// this formula comes as
	      			// time between on slit = iteration_time
	      			// time for 20 slits  = 20 * iteration_time
	      			// for 1 time = 1/20*iteration time
	      			// for one minute = 60000/20*iteration_time

	      			speed = 3000/iteration_time;


	      			// timer end function as we have seen the second high pulse
	      			Timer_end_func();

	      			// to remove certain high level debouncing values
	      			if (speed < 3000) {
	      				input = speed;
	      			}

	      			//******************************************///
	      			//PId has been implemented here
	      			//PID constants are
	      			// KP =0.1 Kd = 1 KI =10
	      			// input to the pid setup is the current_error
	      			current_error = ref_speed - input;
	      			integration_sum += (current_error * iteration_time);
	      			duty = KP * current_error + KI * integration_sum + KD * 1000 * (current_error -previous_error)/iteration_time;

	      			// directly loaded to the current compare register value instead of using the funtion for
	      			// PWM to speed up the iteration tim intervals
	      			//TIM1->CCR1 = duty;
	      			//if(duty>2000){duty=2000;}
	      			//if(duty<0){duty=0;}
	      			TIM1->CCR1 = duty;



	      			printf("%d,%d,%d\r\n",1,cpt_time ,speed);
	      			// to keep a track of the previous error
	      			previous_error = current_error;

	      			//*******************************************///
	      			res=0;
	      		}
	      	}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
