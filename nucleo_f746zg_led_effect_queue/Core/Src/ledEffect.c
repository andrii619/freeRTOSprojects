
#include "ledEffect.h"

static char const *const ledMenuMsg = "LED Menu:\r\n0:Led OFF\r\n1:LED"
                                      "Sequential\r\n2 : Exit\r\n ";

static void LEDTask(void *argument);

static void LEDTask(void *argument) {
  SEGGER_SYSVIEW_PrintfHost("LED task running");
  while (1) {
  }
}

void ledEffectInit(LEDEffect *LEDController) {
  LEDController->ledEffectTaskHandle = xTaskCreateStatic(
      LEDTask, LED_EFFECT_TASK_NAME, STACK_SIZE, (void *)LEDController,
      LED_EFFECT_TASK_PRIORITY, LEDController->ledEffectTaskStack,
      &(LEDController->ledEffectTaskTCB));
}

BaseType_t isLedEffectInitialized(LEDEffect *LEDController) {
  if (!LEDController->ledEffectTaskHandle) {
    return pdFALSE;
  }
  return pdTRUE;
}