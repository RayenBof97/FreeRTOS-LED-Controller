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
uint8_t getnumber(uint8_t *p , int len);

//Constants
const char* msg_invalid ="Invalid Choice, Try again\n";

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
			continue;
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




/**
 * @brief RTC Task Handler
 *
 * - Manages the Real-Time Clock (RTC) peripheral.
 * - Handles user commands to configure or update RTC settings.
 *
 * @param a pointer to a parameter if needed
 * @return void
 */
void RTCTask_Handler(void *param)
{
	const char* msg_rtc1 = "========================\n"
							"|         RTC          |\n"
							"========================\n";

	const char* msg_rtc2 = "Configure Time            ----> 0\n"
							"Configure Date            ----> 1\n"
							"Enable reporting          ----> 2\n"
							"Exit                      ----> 3\n"
							"Enter your choice here : ";


	const char *msg_rtc_hh = "Enter hour(1-12):";
	const char *msg_rtc_mm = "Enter minutes(0-59):";
	const char *msg_rtc_ss = "Enter seconds(0-59):";

	const char *msg_rtc_dd  = "Enter date(1-31):";
	const char *msg_rtc_mo  ="Enter month(1-12):";
	const char *msg_rtc_dow  = "Enter day(1-7 sun:1):";
	const char *msg_rtc_yr  = "Enter year(0-99):";

	const char *msg_conf = "Configuration successful\n";
	const char *msg_rtc_report = "Enable time&date reporting(y/n)?: ";


	uint32_t cmd_addr;
	command_t *cmd;

	static int rtc_state = 0;
	int menu_code;

	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;


#define HH_CONFIG 		0
#define MM_CONFIG 		1
#define SS_CONFIG 		2

#define DATE_CONFIG 	0
#define MONTH_CONFIG 	1
#define YEAR_CONFIG 	2
#define DAY_CONFIG 		3


	while(1){
		/*Notify wait (wait till someone notifies) */
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);

		/*Print the menu and show current date and time information */
		xQueueSend(printQueue,&msg_rtc1,portMAX_DELAY);
		show_time_date();
		xQueueSend(printQueue,&msg_rtc2,portMAX_DELAY);


		while(curr_state != MenuState){

			/*Wait for command notification (Notify wait) */
			xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
			cmd = (command_t*)cmd_addr;  //Store the pointer to the command

			switch(curr_state)
			{
				case RtcState:{
					/*process RTC menu commands */
					if(cmd->len == 1) //Checking if we only got 1 char as input
					{
						menu_code = cmd->payload[0] - 48; //Convert from ASCII to Numbers
						switch(menu_code)
						{
						case 0:
							curr_state = RtcTimeConfigState;
							xQueueSend(printQueue,&msg_rtc_hh,portMAX_DELAY);
							break;
						case 1:
							curr_state = RtcDateConfigState;
							xQueueSend(printQueue,&msg_rtc_dd,portMAX_DELAY);
							break;
						case 2 :
							curr_state = RtcReport;
							xQueueSend(printQueue,&msg_rtc_report,portMAX_DELAY);
							break;
						case 3 :
							curr_state = MenuState;
							break;
						default:
							curr_state = MenuState;
							xQueueSend(printQueue,&msg_invalid,portMAX_DELAY);
						}

					}else{
						curr_state = MenuState;
						xQueueSend(printQueue,&msg_invalid,portMAX_DELAY);
					}
					break;}

				case RtcTimeConfigState:{
					/*get hh, mm, ss infor and configure RTC */
					/*take care of invalid entries */
					switch(rtc_state)
						{
							case HH_CONFIG:{
								uint8_t hour = getnumber(cmd->payload , cmd->len);
								time.Hours = hour;
								rtc_state = MM_CONFIG;
								xQueueSend(printQueue,&msg_rtc_mm,portMAX_DELAY);
								break;}
							case MM_CONFIG:{
								uint8_t min = getnumber(cmd->payload , cmd->len);
								time.Minutes = min;
								rtc_state = SS_CONFIG;
								xQueueSend(printQueue,&msg_rtc_ss,portMAX_DELAY);
								break;}
							case SS_CONFIG:{
								uint8_t sec = getnumber(cmd->payload , cmd->len);
								time.Seconds = sec;
								if(!validate_rtc_information(&time,NULL))
								{
									rtc_configure_time(&time);
									xQueueSend(printQueue,&msg_conf,portMAX_DELAY);
									show_time_date();
								}else
									xQueueSend(printQueue,&msg_invalid,portMAX_DELAY);

								curr_state = MenuState;
								rtc_state = 0;
								break;}
						}

					break;}

				case RtcDateConfigState:{

					/*get date, month, day , year info and configure RTC */

					/*take care of invalid entries */
					switch(rtc_state)
						{
							case DATE_CONFIG:{
								uint8_t d = getnumber(cmd->payload , cmd->len);
								date.Date = d;
								rtc_state = MONTH_CONFIG;
								xQueueSend(printQueue,&msg_rtc_mo,portMAX_DELAY);
								break;}
							case MONTH_CONFIG:{
								uint8_t month = getnumber(cmd->payload , cmd->len);
								date.Month = month;
								rtc_state = DAY_CONFIG;
								xQueueSend(printQueue,&msg_rtc_dow,portMAX_DELAY);
								break;}
							case DAY_CONFIG:{
								uint8_t day = getnumber(cmd->payload , cmd->len);
								date.WeekDay = day;
								rtc_state = YEAR_CONFIG;
								xQueueSend(printQueue,&msg_rtc_yr,portMAX_DELAY);
								break;}
							case YEAR_CONFIG:{
								uint8_t year = getnumber(cmd->payload , cmd->len);
								date.Year = year;

								if(!validate_rtc_information(NULL,&date))
								{
									rtc_configure_date(&date);
									xQueueSend(printQueue,&msg_conf,portMAX_DELAY);
									show_time_date();
								}else
									xQueueSend(printQueue,&msg_invalid,portMAX_DELAY);

								curr_state = MenuState;
								rtc_state = 0;
								break;}
						}


					break;}

				case RtcReport:{
					/*enable or disable RTC current time reporting over ITM printf */
					if(cmd->len == 1)
					{
						if(cmd->payload[0] == 'y'){
							if(xTimerIsTimerActive(rtc_timer) == pdFALSE)
								xTimerStart(rtc_timer,portMAX_DELAY);
						}else if (cmd->payload[0] == 'n'){
							xTimerStop(rtc_timer,portMAX_DELAY);
						}else{
							xQueueSend(printQueue,&msg_invalid,portMAX_DELAY);
						}

					}else
					    xQueueSend(printQueue,&msg_invalid,portMAX_DELAY);

					curr_state = MenuState;
					break;}

			}// switch end

		} //while end


		/*Notify menu task */
		xTaskNotify(handle_menuTask,0,eNoAction);

		}//while super loop end
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
	uint32_t *msg;
	while(1){
		xQueueReceive(printQueue,&msg, portMAX_DELAY);
		HAL_UART_Transmit(&huart2,(uint8_t*)msg, strlen((char*)msg), HAL_MAX_DELAY);
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

/*
 * Converting from ASCII string into an integer Example : "10" -> 10
 */
uint8_t getnumber(uint8_t *p , int len)
{

	int value ;

	if(len > 1)
	   value =  ( ((p[0]-48) * 10) + (p[1] - 48) );
	else
		value = p[0] - 48;

	return value;

}

