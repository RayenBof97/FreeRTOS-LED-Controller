/**
 * @file task_handlers.c
 * @brief API Documentation for Task used in the project
 *
 * This file contains the task source code for the project.
 * Each task is responsible for handling specific task as detailed below.
 */


/**
 * @brief Menu Task Handler
 *
 * - Displays the main menu to the user.
 * - Processes user input for navigation and triggers corresponding actions.
 *
 * @param a pointer to a parameter if needed
 * @return void
 */

#include "main.h"

void menuTask_Handler(void* parameters) {
	while(1){

	}
}


/**
 * @brief LED Task Handler
 *
 * - Displays the LED control menu.
 * - Handles user input for controlling LEDs (e.g. choosing the mode).
 *
 * @param a pointer to a parameter if needed
 * @return void
 */
void ledTask_Handler(void* parameters){
	while(1){

	}
}


/**
 * @brief RTC Task Handler
 *
 * - Manages the Real-Time Clock (RTC) peripheral.
 * - Handles user commands to configure or update RTC settings.
 *
 * @param a pointer to a parameter if needed
 * @return void
 */
void RTCTask_Handler(void* parameters){
	while(1){

	}
}


/**
 * @brief Print Task Handler
 *
 * - Handles UART communication.
 * - Dequeues messages from the print queue and sends them via UART.
 * - Displays menus and messages on Terminal.
 *
 * @param a pointer to a parameter if needed
 * @return void
 */
void printTask_Handler(void* parameters){
	while(1){

	}
}


/**
 * @brief Command Handling Task
 *
 * - Dequeues input data from the command queue and processes it accordingly.
 *
 * @param a pointer to a parameter if needed
 * @return void
 */
void cmdHandlingTask_Handler(void* parameters){

}


