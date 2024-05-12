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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <appCommon.h>

// freertos
///#include <task.h>

// c std
#include <stdint.h>
//#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

// task functions
//  extern void MenuTask(void *argument);
//  extern void LEDTask(void *argument);
//  extern void RTCTask(void *argument);
//  extern void PrintTask(void *argument);
//  extern void CommandParseTask(void *argument);

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
// #define LD1_Pin GPIO_PIN_0
// #define LD1_GPIO_Port GPIOB
// #define LD3_Pin GPIO_PIN_14
// #define LD3_GPIO_Port GPIOB
// #define TMS_Pin GPIO_PIN_13
// #define TMS_GPIO_Port GPIOA
// #define SW0_Pin GPIO_PIN_3
// #define SW0_GPIO_Port GPIOB
// #define LD2_Pin GPIO_PIN_7
// #define LD2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
