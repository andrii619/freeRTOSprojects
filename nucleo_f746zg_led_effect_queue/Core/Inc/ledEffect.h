
#ifndef __LED_EFFECT_H
#define __LED_EFFECT_H

// #include <FreeRTOS.h>
// #include <SEGGER_SYSVIEW.h>

// #include <task.h>
#include "stm32f7xx_hal.h"
#include <appCommon.h>
#include <main.h>

#include <timers.h>

typedef struct {
  TaskHandle_t ledEffectTaskHandle;
  StackType_t ledEffectTaskStack[STACK_SIZE];
  StaticTask_t ledEffectTaskTCB;

  // software timer for LED effect timing
  LedEffectCmd led_mode;
  TimerHandle_t ledTimer;

} LEDEffect;

void ledEffectInit(LEDEffect *);

BaseType_t isLedEffectInitialized(LEDEffect *);

#endif //__LED_EFFECT_H