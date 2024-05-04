
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
#define LED_EFFECT_TASK_NAME ("LEDEffect")

#define MAIN_MENU_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define MAIN_MENU_TASK_NAME ("Menu")

#define PRINT_MANAGER_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define PRINT_MANAGER_TASK_NAME ("Printer")

#define COMMAND_PARSER_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define COMMAND_PARSER_TASK_NAME ("CommandParser")

#define RTC_CONFIG_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define RTC_CONFIG_TASK_NAME ("RTCConfig")

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
  LED_EFFECT_CMD_OFF = 0,
  LED_EFFECT_CMD_ALL,
  LED_EFFECT_CMD_EVEN,
  LED_EFFECT_CMD_ODD,
  LED_EFFECT_CMD_EVEN_ODD,
} LedEffectCmd;

typedef enum {
  RTC_MENU_CMD_ONE = (1 << 0),
  RTC_MENU_CMD_TWO = (1 << 1),
  RTC_MENU_CMD_THREE = (1 << 2),
  RTC_MENU_CMD_FOUR = (1 << 3),
  RTC_MENU_CMD_BAD_CMD = (1 << 31),
} rtcMenuCmd;

// create command data structure
typedef struct {
  
  // to be allocated and freed by the user
  char *userCommand;
  
  // array of command arguments. also to be freed by the user
  char **arguments;
  int argumentCount;
  
} user_command_s;


typedef struct {
  int arg_count;
  char** args;
} command_t;

extern app_state_t appState;

#endif //__APP_COMMON_H