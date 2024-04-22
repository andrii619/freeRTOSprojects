
#ifndef __APP_COMMON_H
#define __APP_COMMON_H

#include <FreeRTOS.h>
#include <SEGGER_SYSVIEW.h>

#include <queue.h>
#include <semphr.h>
#include <task.h>

#define STACK_SIZE (128)

#define MAX_RETRIES (10)

#define LED_EFFECT_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define LED_EFFECT_TASK_NAME ("LEDTask")

#define MAIN_MENU_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define MAIN_MENU_TASK_NAME ("MenuTask")

#define PRINT_MANAGER_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define PRINT_MANAGER_TASK_NAME ("PrinterTask")

#define COMMAND_PARSER_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define COMMAND_PARSER_TASK_NAME ("CommandParserTask")

#define RTC_CONFIG_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define RTC_CONFIG_TASK_NAME ("RTCConfigTask")

typedef enum {
  sMainMenu = 0,
  sLedEffect,
  sRtcMenu,
  sRtcTimeConfig,
  sRtcDateConfig,
  sRtcReport,
} app_state_t;

typedef enum {
  MENU_CMD_ONE = (1 << 0),
  MENU_CMD_TWO = (1 << 1),
  MENU_CMD_THREE = (1 << 2),
  MENU_CMD_FOUR = (1 << 3),
  MENU_CMD_BAD_CMD = (1 << 31),
} mainMenuCmd;

typedef enum {
  LED_EFFECT_CMD_ONE = (1 << 0),
  LED_EFFECT_CMD_TWO = (1 << 1),
  LED_EFFECT_CMD_THREE = (1 << 2),
  LED_EFFECT_CMD_FOUR = (1 << 3),
  LED_EFFECT_CMD_BAD_CMD = (1 << 31),
} ledEffectCmd;

typedef enum {
  RTC_MENU_CMD_ONE = (1 << 0),
  RTC_MENU_CMD_TWO = (1 << 1),
  RTC_MENU_CMD_THREE = (1 << 2),
  RTC_MENU_CMD_FOUR = (1 << 3),
  RTC_MENU_CMD_BAD_CMD = (1 << 31),
} rtcMenuCmd;

extern app_state_t appState;

#endif //__APP_COMMON_H