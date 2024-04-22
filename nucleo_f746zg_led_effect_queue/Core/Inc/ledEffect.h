
#ifndef __LED_EFFECT_H
#define __LED_EFFECT_H

// #include <FreeRTOS.h>
// #include <SEGGER_SYSVIEW.h>

// #include <task.h>

#include <appCommon.h>

#include <timers.h>

typedef struct {
  TaskHandle_t ledEffectTaskHandle;
  StackType_t ledEffectTaskStack[STACK_SIZE];
  StaticTask_t ledEffectTaskTCB;

  // software timer for LED effect timing
  TimerHandle_t ledTimer;

} LEDEffect;

void ledEffectInit(LEDEffect *);

BaseType_t isLedEffectInitialized(LEDEffect *);

#endif //__LED_EFFECT_H