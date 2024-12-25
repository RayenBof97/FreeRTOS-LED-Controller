/*
 * led_effect.c
 *
 *  Created on: Dec 24, 2024
 *      Author: Rayen
 */


#include "main.h"

//Basic Functions
void turn_on_all(void){   //Turn on all LEDs
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
}

void turn_off_all(void){	//Turn off all LEDs
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
}

void turn_on_even(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
}

void turn_on_odd(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
}

void led_control(int led_mask){
	//Control PB8
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, (led_mask & 0x1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	// Control PB9
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, (led_mask & 0x2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	// Control PB10
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (led_mask & 0x4) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


void led_effect_stop(void){		//Turn off the currect led effect by turning off the timer
	for(int i = 0; i < 4 ; i++){
		xTimerStop(handle_led_timers[i],portMAX_DELAY);
	}
}

void led_effect(int option){		//start the effect by starting the right timer inserted by the user through the command
	led_effect_stop();
	xTimerStart(handle_led_timers[option-1],portMAX_DELAY);
}

//The LED Effects

void led_effect1(void){    //Toggle ON and OFF all LEDs
	static int flag = 1;
	(flag^=1) ? turn_off_all() : turn_on_all() ;
}

void led_effect2(void){    //Toggle ON and OFF all LEDs
	static int flag = 1;
	(flag^=1) ? turn_on_odd() : turn_on_even() ;
}

void led_effect3(void){    //Cyclic Toggling <-
	static int i = 0;
	led_control(0x1 << (i++ % 3));
}

void led_effect4(void){    //Cyclic Toggling ->
	static int i = 0;
	led_control(0x4 >> (i++ % 3));
}



