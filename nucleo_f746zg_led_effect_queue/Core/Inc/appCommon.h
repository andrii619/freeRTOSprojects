
#ifndef __APP_COMMON_H
#define __APP_COMMON_H

#include <FreeRTOS.h>
#include <SEGGER_SYSVIEW.h>

#include <task.h>
#include <queue.h>
#include <semphr.h>


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


//extern app_state_t appState;
// task handles
extern TaskHandle_t menuTaskHandle;
extern TaskHandle_t rtcTaskHandle;
extern TaskHandle_t ledTaskHandle;
extern TaskHandle_t printTaskHandle;
extern TaskHandle_t commandParseTaskHandle;


extern QueueHandle_t printQueue;
extern SemaphoreHandle_t printMutex;

extern app_state_t appState;


#endif //__APP_COMMON_H