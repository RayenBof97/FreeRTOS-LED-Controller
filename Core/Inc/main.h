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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */


//Command structure
typedef struct {
	uint8_t payload[10];
	uint32_t len;
}command_t;

//States of the program
typedef enum{
	MenuState = 0,
	LedState,
	RtcState,
	RtcTimeConfigState,
	RtcDateConfigState,
	RtcReport,
}state_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern TaskHandle_t handle_menuTask;
extern TaskHandle_t handle_ledTask;
extern TaskHandle_t handle_rtcTask;
extern TaskHandle_t handle_printTask;
extern TaskHandle_t handle_cmdHandlingTask;

extern QueueHandle_t inputQueue;
extern QueueHandle_t printQueue;

extern state_t curr_state;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern void menuTask_Handler(void* parameters);
extern void ledTask_Handler(void* parameters);
extern void RTCTask_Handler(void* parameters);
extern void printTask_Handler(void* parameters);
extern void cmdHandlingTask_Handler(void* parameters);

extern void led_effect_stop(void);
extern void led_effect(int option);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define MAX_PAYLOAD_SIZE 10
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
