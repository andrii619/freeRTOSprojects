
#include "ledEffect.h"

#include <printManager.h>
#include <string.h>

// static char const *const ledMenuMsg = "LED Menu:\r\na:Led OFF\r\nb:LED"
//                                       "Sequential\r\nc : Exit\r\n ";

static char const *const ledMenuMsg = "XXXXXXXXX\r\n";

extern PrintManager printer;

static void LEDTask(void *argument) {

  LEDEffect *LEDController = (LEDEffect *)argument;
  assert_param(isLedEffectInitialized(LEDController) == pdTRUE);

  size_t msgLen = strlen(ledMenuMsg);
  uint8_t ledItenationNum = 0;
  SEGGER_SYSVIEW_PrintfHost("LED task running");
  while (1) {

    SEGGER_SYSVIEW_PrintfHost("led iter %d", ledItenationNum++);
    printMessageBlocking(&printer, (uint8_t *)ledMenuMsg, msgLen);

    vTaskDelay(pdMS_TO_TICKS(100));
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