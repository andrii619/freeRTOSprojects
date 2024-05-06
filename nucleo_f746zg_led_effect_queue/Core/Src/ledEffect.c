
#include "ledEffect.h"

#include <printManager.h>
#include <string.h>

// static char const *const ledMenuMsg = "LED Menu:\r\na:Led OFF\r\nb:LED"
//                                       "Sequential\r\nc : Exit\r\n ";

static char const *const ledMenuMsg = "XXXXXXXXX\r\n";
extern PrintManager printer;

static void LEDEffectAll(void);
static void LEDEffectEven(void);
static void LEDEffectOdd(void);
static void LEDEffectEvenOdd(void);

static void ledTurnOffAll(void);
static void ledTurnOnAll(void);
static void ledTurnOnEven(void);
static void ledTurnOffEven(void);
static void ledTurnOnOdd(void);
static void ledTurnOffOdd(void);

static LEDEffect *localLEDControllerRef;

static void LEDTask(void *argument) {

  LEDEffect *LEDController = (LEDEffect *)argument;
  assert_param(isLedEffectInitialized(LEDController) == pdTRUE);

  size_t msgLen = strlen(ledMenuMsg);
  uint8_t ledItenationNum = 0;

  xTimerStart(LEDController->ledTimer, portMAX_DELAY);

  SEGGER_SYSVIEW_PrintfHost("LED task running");

  while (1) {

    SEGGER_SYSVIEW_PrintfHost("led iter %d", ledItenationNum++);

    // block until the command queue is not empty
    // when reading an empty queue the task will be placed in blocked state
    // we process one command on each iteration of the loop
    command_t cmd = {0, NULL};
    xQueueReceive(LEDController->commandQueue, &cmd, portMAX_DELAY);
    /**
     * @brief wait for a notification to process a command
     *
     */
    // test led handle command
    assert_param(cmd.arg_count > 0 && cmd.args);
    handleCommand(cmd);

    /*
    // testing command handling
    command_t testCmd = {0, NULL};
    testCmd.arg_count = 1;
    testCmd.args = pvPortMalloc(testCmd.arg_count * sizeof(char *));
    if (testCmd.args == NULL) {
      // Handle allocation failure (e.g., error logging and exit)
    }
    for (int i = 0; i < testCmd.arg_count; i++) {
      testCmd.args[i] = pvPortMalloc(10 * sizeof(char));
      if (testCmd.args[i] == NULL) {
        // Handle allocation failure; Free previously allocated memory
        for (int j = 0; j < i; j++) {
          vPortFree(testCmd.args[j]);
          testCmd.args[j] = NULL;
        }
        vPortFree(testCmd.args);
        testCmd.args = NULL;
        // Exit or error handling code
      }
      memset(testCmd.args[i], 0, 10);
    }
    strcpy(testCmd.args[0], "$LEDMODE");
    handleCommand(testCmd);
    */

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void vTimerCallback(TimerHandle_t xTimer) {
  SEGGER_SYSVIEW_PrintfHost("Timer Callback");
  assert_param(localLEDControllerRef);
  switch (localLEDControllerRef->led_mode) {
  case (LED_EFFECT_CMD_ALL): {
    LEDEffectAll();
  }; break;
  case (LED_EFFECT_CMD_EVEN): {
    LEDEffectEven();
  }; break;
  case (LED_EFFECT_CMD_ODD): {
    LEDEffectOdd();
  }; break;
  case (LED_EFFECT_CMD_EVEN_ODD): {
    LEDEffectEvenOdd();
  }; break;
  default:
    ledTurnOffAll();
  };
};

void ledEffectInit(LEDEffect *LEDController) {
  LEDController->ledEffectTaskHandle = xTaskCreateStatic(
      LEDTask, LED_EFFECT_TASK_NAME, STACK_SIZE, (void *)LEDController,
      LED_EFFECT_TASK_PRIORITY, LEDController->ledEffectTaskStack,
      &(LEDController->ledEffectTaskTCB));
  assert_param(LEDController->ledEffectTaskHandle);

  LEDController->led_mode = LED_EFFECT_CMD_OFF;

  // copy for local ref
  localLEDControllerRef = LEDController;

  LEDController->ledTimer =
      xTimerCreate("led", pdMS_TO_TICKS(500), pdTRUE, NULL, vTimerCallback);
  assert_param(LEDController->ledTimer);

  LEDController->commandQueue =
      xQueueCreate(COMMAND_QUEUE_LENGTH, sizeof(command_t));
  assert_param(LEDController->commandQueue);
}

BaseType_t isLedEffectInitialized(LEDEffect *LEDController) {
  if (!LEDController->ledEffectTaskHandle) {
    return pdFALSE;
  }
  return pdTRUE;
}

void handleCommand(command_t cmd) {

  // take the command and put it into a queue

  assert_param(cmd.arg_count > 0);
  for (int i = 0; i < cmd.arg_count; i++) {
    assert_param(
        cmd.args[i]); // check that each command string is not a null pointer
  }

  if (strncmp(cmd.args[0], "$LEDMODE", 8) == 0) {
    // print current LED mode
    char const *msg = "LEDMODE:ALL_ON\r\n";
    // reply
    printMessageBlocking(&printer, (uint8_t *)msg, strlen(msg));
  }

  // free each command string
  for (int i = 0; i < cmd.arg_count; i++) {
    vPortFree(cmd.args[i]);
    cmd.args[i] = NULL;
  }
  vPortFree(cmd.args);
  cmd.args = NULL;
}

static void LEDEffectAll(void) {
  static int flag = 1;
  (flag ^= 1) ? ledTurnOffAll() : ledTurnOnAll();
}

static void LEDEffectEven(void) {
  static int flag = 1;
  ledTurnOffAll();
  (flag ^= 1) ? ledTurnOffEven() : ledTurnOnEven();
}

static void LEDEffectOdd(void) {
  static int flag = 1;
  ledTurnOffAll();
  (flag ^= 1) ? ledTurnOffOdd() : ledTurnOnOdd();
}

static void LEDEffectEvenOdd(void) {
  static int flag = 1;
  ledTurnOffAll();
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