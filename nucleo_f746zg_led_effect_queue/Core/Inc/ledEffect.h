
#ifndef __LED_EFFECT_H
#define __LED_EFFECT_H

// #include <FreeRTOS.h>
// #include <SEGGER_SYSVIEW.h>

// #include <task.h>

#include <appCommon.h>

typedef struct {
  TaskHandle_t ledEffectTaskHandle;
  StackType_t ledEffectTaskStack[STACK_SIZE];
  StaticTask_t ledEffectTaskTCB;

} LEDEffect;

void ledEffectInit(LEDEffect *);

BaseType_t isLedEffectInitialized(LEDEffect *);

#endif //__LED_EFFECT_H