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

//Constants
const char* msg_invalid ="Invalid Choice, Try again";

void menuTask_Handler(void* parameters) {

	uint32_t cmd_addr; //This variable will receive the address of the command as (uint32_t)
	command_t *cmd; //This is a pointer to command_t variable which will store the pointer to our command

	int option;

	const char* msg_menu = "\n========================\n"
								"|         Menu         |\n"
								"========================\n"
									"LED effect    ----> 0\n"
									"Date and time ----> 1\n"
									"Exit          ----> 2\n"
									"Enter your choice here : ";

	while(1){
		xQueueSend(printQueue,&msg_menu,portMAX_DELAY); //Here the queue will actually store msg_menu which is a pointer to the string
		xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
		cmd = (command_t*)cmd_addr;

		if (cmd->len == 1){
			option = cmd->payload[0] - 48 ; //Converting ASCII to numbers
			switch(option) //Depending on the value we are gonna also notify the corresponding task
			{
			case 0:
				curr_state = LedState;
				xTaskNotify(handle_ledTask,0,eNoAction);
				break;
			case 1:
				curr_state = RtcState;
				xTaskNotify(handle_rtcTask,0,eNoAction);
				break;
			case 2: //Exit
				break;
			default:
				xQueueSend(printQueue,&msg_invalid,portMAX_DELAY); //Invalid Char
				continue;
			}
		}else{
			//Invalid Choice (More than 1 char)
			xQueueSend(printQueue,&msg_invalid,portMAX_DELAY);
		}
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
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
			uint32_t cmd_addr;
			command_t *cmd;
			const char* msg_led = "========================\n"
								  "|      LED Effect     |\n"
								  "========================\n"
								  "(none,e1,e2,e3,e4)\n"
								  "Enter your choice here : ";

			while(1){
				xTaskNotifyWait(0,0,NULL,portMAX_DELAY); //Whenever the Task run , it blocks and wait for notifications

				//Print the Menu
				xQueueSend(printQueue,&msg_led,portMAX_DELAY);

				// wait for LED command
				xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
				cmd = (command_t*)cmd_addr;

				if(cmd->len <= 4)
				{
					if(! strcmp((char*)cmd->payload,"none"))
						led_effect_stop();
					else if (! strcmp((char*)cmd->payload,"e1"))
						led_effect(1);
					else if (! strcmp((char*)cmd->payload,"e2"))
						led_effect(2);
					else if (! strcmp((char*)cmd->payload,"e3"))
						led_effect(3);
					else if (! strcmp((char*)cmd->payload,"e4"))
						led_effect(4);
					else
						xQueueSend(printQueue,&msg_invalid,portMAX_DELAY); //Invalid Option
				}else
					xQueueSend(printQueue,&msg_invalid,portMAX_DELAY); //Invalid Option

				//update state variable
				curr_state = MenuState;

				// Notify menu task
				xTaskNotify(handle_menuTask,0,eNoAction);

			}
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

