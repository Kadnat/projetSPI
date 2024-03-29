/*
 * PID.c
 *
 *  Created on: Feb 18, 2024
 *      Author: Nathanael
 */
#include "stdint.h"
#include "tim.h"
#include "main.h"

uint32_t tickL=0, tickR=0, speedL=0, speedR=0, previous_speedL=0, previous_speedL2=0, previous_speedL3=0;

static int i = 0;int bit =0;int res =0;

static uint32_t cpt_time=0,cpt_start=0,time_elapsed=0;
int finished_measure=1; int enabled_motorR=0;int first_front=0;

// refernce and pid related constants
int ref_speed = 0;float duty =0;int iteration_time =0;
int first_dutyL=1999; first_dutyR=1999; is_firstL=1; is_firstR=1;

//errors variables
float previous_errorL = 0;float current_errorL = 0;
float previous_errorR = 0;float current_errorR = 0;
float error_somme=0, delta_erreur=0;

// variables for the pid function that is actually not a function
float KP = 700;float KI = 70;int KD = 30;

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


	      		if(GPIO_PIN==SPEED_SENSORM1_Pin)
	      		{


	      			if(finished_measure)
	      			{
	      				//iteration_time=time_elapsed/20;
						//speedL = 3000/iteration_time;

	      				speedL=getSpeed();

						// to remove certain high level debouncing values
						if ((speedL) <= 250) {
							input = (speedL+previous_speedL+previous_speedL2+previous_speedL3)/4;
						}
						//else
							//input = previous_speedL;

						if(input==0) input=1;
						iteration_time = 3000/input;

						printf("speed=%d\r\n",speedL);
						printf("previous_speedL=%d\r\n",previous_speedL);
						//******************************************///
						//PId has been implemented here
						//PID constants are
						// KP =0.1 Kd = 1 KI =10
						// input to the pid setup is the current_error
						current_errorL = ref_speed - input;
						error_somme+=current_errorL;
						integration_sum += (current_errorL * iteration_time);
						delta_erreur=current_errorL-previous_errorL;

						duty = KP * current_errorL + KI * error_somme + KD*delta_erreur;//+ KD * 1000 * (current_errorL -previous_errorL)/iteration_time;

						// directly loaded to the current compare register value instead of using the funtion for
						// PWM to speed up the iteration tim intervals
						//TIM1->CCR1 = duty;
						uint32_t duty_int = (uint32_t)duty ;

						//printf("duty=%d\r\n",duty);
						if(duty_int>1999){duty_int=1999;}
						if(duty_int<300){duty_int=300;}

						TIM1->CCR1=duty_int;

						printf("RPM=%d\r\n",input);
						printf("duty=%f\r\n",duty);



						// to keep a track of the previous error
						previous_errorL = current_errorL;
						previous_speedL3 = previous_speedL2;
						previous_speedL2 = previous_speedL;
						previous_speedL = input;
						finished_measure=1;
						//*******************************************///
						res=0;
					}
	      		}

				else if(GPIO_PIN==SPEED_SENSORM2_Pin)
				{
					if(is_firstR)
						{
						TIM1->CCR2=first_dutyR;
						is_firstR=0;
						}

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

						speedR = 3000/iteration_time;


						// timer end function as we have seen the second high pulse
						Timer_end_func();

						// to remove certain high level debouncing values
						if (speedR < 3000) {
							input = speedR;
						}

						//******************************************///
						//PId has been implemented here
						//PID constants are
						// KP =0.1 Kd = 1 KI =10
						// input to the pid setup is the current_error
						current_errorR = ref_speed - input;
						integration_sum += (current_errorR * iteration_time);
						duty = KP * current_errorR + KI * integration_sum + KD * 1000 * (current_errorR -previous_errorR)/iteration_time;

						// directly loaded to the current compare register value instead of using the funtion for
						// PWM to speed up the iteration tim intervals
						//TIM1->CCR1 = duty;
						//if(duty>2000){duty=2000;}
						//if(duty<1200){duty=1200;}
						printf("speedR=%d\r\n",speedR);
						TIM1->CCR2=duty;
						// to keep a track of the previous error
						previous_errorR = current_errorR;

						//*******************************************///
						res=0;
					}
					}
				}
	      	}



