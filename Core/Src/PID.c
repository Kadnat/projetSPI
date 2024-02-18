/*
 * PID.c
 *
 *  Created on: Feb 18, 2024
 *      Author: Nathanael
 */
#include "stdint.h"
#include "tim.h"

static int i = 0;int bit =0;int speed =0;int res =0;
static uint32_t cpt_time=0;

// refernce and pid related constants
int ref_speed = 0;float duty =0;int iteration_time =0;

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

void pid_calculation(int r_speed, GPIO_TypeDef * GPIO_PORT, uint16_t GPIO_PIN)
{
		ref_speed = r_speed;
	  // reads the current state of PA1
	      	// default function used
	      	if ( HAL_GPIO_ReadPin(GPIO_PORT, GPIO_PIN) == 0){
	      		while(HAL_GPIO_ReadPin(GPIO_PORT, GPIO_PIN) == 0){}

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

	      			TIM1->CCR1=duty;
	      			// to keep a track of the previous error
	      			previous_error = current_error;

	      			//*******************************************///
	      			res=0;

	      		}
	      	}
}

