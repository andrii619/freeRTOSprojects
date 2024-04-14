
#ifndef __APP_COMMON_H
#define __APP_COMMON_H

#include "stm32f7xx_hal.h"

#include <FreeRTOS.h>
#include <SEGGER_SYSVIEW.h>

#include <queue.h>
#include <semphr.h>
#include <task.h>

#define STACK_SIZE 128

typedef enum
{
  sMainMenu = 0,
  sLedEffect,
  sRtcMenu,
  sRtcTimeConfig,
  sRtcDateConfig,
  sRtcReport,
} app_state_t;

typedef enum
{
  MENU_CMD_ONE = (1 << 0),
  MENU_CMD_TWO = (1 << 1),
  MENU_CMD_THREE = (1 << 2),
  MENU_CMD_FOUR = (1 << 3),
  MENU_CMD_BAD_CMD = (1 << 31),
} mainMenuCmd;

typedef enum
{
  LED_EFFECT_CMD_ONE = (1 << 0),
  LED_EFFECT_CMD_TWO = (1 << 1),
  LED_EFFECT_CMD_THREE = (1 << 2),
  LED_EFFECT_CMD_FOUR = (1 << 3),
  LED_EFFECT_CMD_BAD_CMD = (1 << 31),
} ledEffectCmd;

typedef enum
{
  RTC_MENU_CMD_ONE = (1 << 0),
  RTC_MENU_CMD_TWO = (1 << 1),
  RTC_MENU_CMD_THREE = (1 << 2),
  RTC_MENU_CMD_FOUR = (1 << 3),
  RTC_MENU_CMD_BAD_CMD = (1 << 31),
} rtcMenuCmd;

extern app_state_t appState;

#endif //__APP_COMMON_H