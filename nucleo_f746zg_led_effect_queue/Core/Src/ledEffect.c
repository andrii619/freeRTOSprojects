
#include "ledEffect.h"

#include <printManager.h>
#include <string.h>

// static char const *const ledMenuMsg = "LED Menu:\r\na:Led OFF\r\nb:LED"
//                                       "Sequential\r\nc : Exit\r\n ";

static char const *const ledMenuMsg = "XXXXXXXXX\r\n";
extern PrintManager printer;

static void LEDEffect1(void);
static void LEDEffect2(void);

static void ledTurnOffAll(void);
static void ledTurnOnAll(void);
static void ledTurnOnEven(void);
static void ledTurnOffEven(void);
static void ledTurnOnOdd(void);
static void ledTurnOffOdd(void);

static void LEDTask(void *argument) {

  LEDEffect *LEDController = (LEDEffect *)argument;
  assert_param(isLedEffectInitialized(LEDController) == pdTRUE);

  size_t msgLen = strlen(ledMenuMsg);
  uint8_t ledItenationNum = 0;
  SEGGER_SYSVIEW_PrintfHost("LED task running");
  while (1) {

    SEGGER_SYSVIEW_PrintfHost("led iter %d", ledItenationNum++);
    printMessageBlocking(&printer, (uint8_t *)ledMenuMsg, msgLen);
    LEDEffect2();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void ledEffectInit(LEDEffect *LEDController) {
  LEDController->ledEffectTaskHandle = xTaskCreateStatic(
      LEDTask, LED_EFFECT_TASK_NAME, STACK_SIZE, (void *)LEDController,
      LED_EFFECT_TASK_PRIORITY, LEDController->ledEffectTaskStack,
      &(LEDController->ledEffectTaskTCB));
  assert_param(LEDController->ledEffectTaskHandle != NULL);
}

BaseType_t isLedEffectInitialized(LEDEffect *LEDController) {
  if (!LEDController->ledEffectTaskHandle) {
    return pdFALSE;
  }
  return pdTRUE;
}

static void LEDEffect1(void) {
  static int flag = 1;
  (flag ^= 1) ? ledTurnOffAll() : ledTurnOnAll();
}

int moew1(void) { return 1; }
int meow2(void) { return 2; }

static void LEDEffect2(void) {
  static int flag = 1;
  flag ^= 1;
  if (flag) {
    ledTurnOnOdd();
    ledTurnOffEven();
  } else {
    ledTurnOffOdd();
    ledTurnOnEven();
  }
};

static void ledTurnOffAll(void) {
  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
};

static void ledTurnOnAll(void) {
  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
};

static void ledTurnOnEven(void) {
  // HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
};

static void ledTurnOffEven(void) {
  // HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  // HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
};

static void ledTurnOnOdd(void) {
  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
};

static void ledTurnOffOdd(void) {
  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
  // HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
};