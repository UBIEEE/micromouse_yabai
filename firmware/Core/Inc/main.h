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
#include "stm32wbxx_hal.h"
#include "app_conf.h"
#include "app_entry.h"
#include "app_common.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IN_BUTTON_1_Pin GPIO_PIN_3
#define IN_BUTTON_1_GPIO_Port GPIOH
#define TIM2_ENC_A_Pin GPIO_PIN_0
#define TIM2_ENC_A_GPIO_Port GPIOA
#define TIM2_ENC_B_Pin GPIO_PIN_1
#define TIM2_ENC_B_GPIO_Port GPIOA
#define ADC1_IR_FAR_R_Pin GPIO_PIN_2
#define ADC1_IR_FAR_R_GPIO_Port GPIOA
#define ADC1_IR_MID_R_Pin GPIO_PIN_3
#define ADC1_IR_MID_R_GPIO_Port GPIOA
#define ADC1_IR_MID_L_Pin GPIO_PIN_4
#define ADC1_IR_MID_L_GPIO_Port GPIOA
#define ADC1_IR_FAR_L_Pin GPIO_PIN_5
#define ADC1_IR_FAR_L_GPIO_Port GPIOA
#define TIM16_BUZZER_Pin GPIO_PIN_6
#define TIM16_BUZZER_GPIO_Port GPIOA
#define OUT_M1_DIR_Pin GPIO_PIN_7
#define OUT_M1_DIR_GPIO_Port GPIOA
#define TIM1_M1_PWM_Pin GPIO_PIN_8
#define TIM1_M1_PWM_GPIO_Port GPIOA
#define TIM1_M2_PWM_Pin GPIO_PIN_9
#define TIM1_M2_PWM_GPIO_Port GPIOA
#define OUT_M2_DIR_Pin GPIO_PIN_2
#define OUT_M2_DIR_GPIO_Port GPIOB
#define OUT_IR_FAR_L_Pin GPIO_PIN_0
#define OUT_IR_FAR_L_GPIO_Port GPIOB
#define OUT_IR_MID_L_Pin GPIO_PIN_1
#define OUT_IR_MID_L_GPIO_Port GPIOB
#define OUT_IR_MID_R_Pin GPIO_PIN_4
#define OUT_IR_MID_R_GPIO_Port GPIOE
#define IN_BUTTON_2_Pin GPIO_PIN_15
#define IN_BUTTON_2_GPIO_Port GPIOA
#define OUT_IR_FAR_R_Pin GPIO_PIN_4
#define OUT_IR_FAR_R_GPIO_Port GPIOB
#define LPTIM1_ENC_B_Pin GPIO_PIN_5
#define LPTIM1_ENC_B_GPIO_Port GPIOB
#define LPTIM1_ENC_A_Pin GPIO_PIN_7
#define LPTIM1_ENC_A_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
