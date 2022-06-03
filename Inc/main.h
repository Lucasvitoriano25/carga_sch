/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
 typedef enum {
  CURRENT = 0,
  POTENCY,
  RESISTANCE,
  ALTERATING_TIME_ON,
  IDLE
} E_Carga_State;

 typedef enum {
  OFF = 0,
  ON = 1
} Security_Time_State;

typedef struct {
  E_Carga_State state_load;
  float value_load;
  uint16_t time_load_on;
  Security_Time_State security_time_state;
} Load;

typedef enum
{
  OK = 0,
  ERRO_Length,
  ERRO_CHECKSUM,
  ERRO_EMERGENCY,
  BUSY,
  TIMEOUT,
  OUTRANGE_VALUE,
  INCONSISTENCY_IN_SETED_MODE
} StatusMessageTypeDef;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADC_CURRENT_Pin GPIO_PIN_0
#define ADC_CURRENT_GPIO_Port GPIOA
#define ADC_VOLTAGE_Pin GPIO_PIN_1
#define ADC_VOLTAGE_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
#define AD_To_mA 0.008058608

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
