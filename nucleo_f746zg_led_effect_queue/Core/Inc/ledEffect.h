
#ifndef __LED_EFFECT_H
#define __LED_EFFECT_H

// #include <FreeRTOS.h>
// #include <SEGGER_SYSVIEW.h>

// #include <task.h>
#include "stm32f7xx_hal.h"
#include <appCommon.h>
#include <main.h>

#include <timers.h>

#define COMMAND_QUEUE_LENGTH (15)


typedef struct {
  TaskHandle_t ledEffectTaskHandle;
  StackType_t ledEffectTaskStack[STACK_SIZE];
  StaticTask_t ledEffectTaskTCB;

  // software timer for LED effect timing
  LedEffectCmd led_mode; // currently commanded LED mode
  TimerHandle_t ledTimer;
  
  //void (*ledTimerCallback)(TimerHandle_t xTimer);
  
  // command queue
  QueueHandle_t commandQueue;

} LEDEffect;

void ledEffectInit(LEDEffect *);

BaseType_t isLedEffectInitialized(LEDEffect *);

#endif //__LED_EFFECT_H