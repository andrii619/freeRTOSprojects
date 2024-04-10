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
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <string.h>
#include <FreeRTOS.h>
#include <SEGGER_SYSVIEW.h>
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
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define SW0_Pin GPIO_PIN_3
#define SW0_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
typedef enum {
  sMainMenu = 0,
  sLedEffect,
  sRtcMenu,
  sRtcTimeConfig,
  sRtcDateConfig,
  sRtcReport,
} app_state_t;


typedef enum {
  MENU_CMD_ONE=      (1<<0),
  MENU_CMD_TWO=      (1<<1),
  MENU_CMD_THREE=    (1<<2),
  MENU_CMD_FOUR=     (1<<3),
  MENU_CMD_BAD_CMD=   (1<<31),
} mainMenuCmd;


typedef enum {
  LED_EFFECT_CMD_ONE=      (1<<0),
  LED_EFFECT_CMD_TWO=      (1<<1),
  LED_EFFECT_CMD_THREE=    (1<<2),
  LED_EFFECT_CMD_FOUR=     (1<<3),
  LED_EFFECT_CMD_BAD_CMD=   (1<<31),
} ledEffectCmd;


typedef enum {
  RTC_MENU_CMD_ONE=      (1<<0),
  RTC_MENU_CMD_TWO=      (1<<1),
  RTC_MENU_CMD_THREE=    (1<<2),
  RTC_MENU_CMD_FOUR=     (1<<3),
  RTC_MENU_CMD_BAD_CMD=   (1<<31),
} rtcMenuCmd;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
