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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SENSOR_IP_Pin GPIO_PIN_1
#define SENSOR_IP_GPIO_Port GPIOA
#define BUZZER_IP_Pin GPIO_PIN_2
#define BUZZER_IP_GPIO_Port GPIOA
#define POW_EN_Pin GPIO_PIN_3
#define POW_EN_GPIO_Port GPIOA
#define GP1_Pin GPIO_PIN_13
#define GP1_GPIO_Port GPIOB
#define LED_EXT_Pin GPIO_PIN_14
#define LED_EXT_GPIO_Port GPIOB
#define REDE_Pin GPIO_PIN_8
#define REDE_GPIO_Port GPIOA
#define CAN_STDBY_Pin GPIO_PIN_15
#define CAN_STDBY_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOB
#define SHUT_Pin GPIO_PIN_4
#define SHUT_GPIO_Port GPIOB
#define FETOFF_Pin GPIO_PIN_5
#define FETOFF_GPIO_Port GPIOB
#define MCU_WAKE_Pin GPIO_PIN_9
#define MCU_WAKE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define STATE_CHANGE_TIME 2
#define STATE_CHG_THRESHOLD 2
#define STATE_DSG_THRESHOLD -2
#define STATE_IDLE_TIME_MULTIPLIER 5


#define FULL_CHARGE_CAPACITY 64840
#define MAX_BATTERY_VOLTAGE 58100
#define MIN_BATTERY_VOLTAGE 42000
#define VOLTAGE_DIFFERENCE (58100-42000)

void pressure_value(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
