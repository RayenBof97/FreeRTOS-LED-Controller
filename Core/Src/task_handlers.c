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

//Prototypes
void process_cmd(command_t *cmd);
int extract_cmd(command_t *cmd);


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
	BaseType_t ret;
	command_t cmd;  //Create a command variable

	while (1){
		ret = xTaskNotifyWait(0,0,NULL,portMAX_DELAY);  //Wait in the blocked state until its notified
		if(ret == pdTRUE){
			process_cmd(&cmd); //Process the command
		}

	}
}


void process_cmd(command_t *cmd){

	int Status = extract_cmd(cmd);
	configASSERT(Status != -1);; //Extracting the command from the queue into the command variable

	/*
	 * Depending on the application state, we are going to notify the right Task
	 * With the command by overwriting its notification value with the command adresse
	 */
	switch(curr_state){
	case MenuState:
			xTaskNotify(handle_menuTask,(uint32_t)cmd,eSetValueWithOverwrite);
		break;

	case LedState:
			xTaskNotify(handle_ledTask,(uint32_t)cmd,eSetValueWithOverwrite);
		break;

	case RtcState:
	case RtcTimeConfigState:
	case RtcDateConfigState:
	case RtcReport:
			xTaskNotify(handle_rtcTask,(uint32_t)cmd,eSetValueWithOverwrite);
		break;
	}
}


int extract_cmd(command_t *cmd){
	uint8_t temp; //We will dequeue char by char inside this temp variable
	BaseType_t status; //

	status = uxQueueMessagesWaiting(inputQueue); //Return number of Messages inside the queue
	if(!status) return -1; //if Queue is empty (Status = 0) then return -1

	uint8_t i = 0; //Counter

	do{
		status = xQueueReceive(inputQueue,&temp,0); //Fetch Q item in the temp variable
		if (status == pdTRUE) //If the data is fetched
			cmd->payload[i++] = temp ; //We store in the cmd variable
	}while(temp != '\n');

	cmd->payload[i-1] = '\0'; //Change the \n character to end of string char
	cmd->len = i-1 ; //get the length of the string

	return 0;

}

