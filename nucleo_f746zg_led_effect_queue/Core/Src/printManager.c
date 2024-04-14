#include <printManager.h>

static void PrintTask(void *argument) {
  // the print manager is passed as an argument to the task
  PrintManager *printer = (PrintManager *)argument;

  if (isPrinterInitialized(printer) == pdFALSE) {
    // lock up the task indefinatelly
    SEGGER_SYSVIEW_PrintfHost("Print task uninitialized");
    vTaskSuspend(printer->printTaskHandle);
  }

  TickType_t xLastWakeTime;
  const TickType_t xFrequency = pdMS_TO_TICKS(200);

  SEGGER_SYSVIEW_PrintfHost("Print task running");
  // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount();
  while (1) {
    if (uxQueueMessagesWaiting(printer->printQueue) != 0) {

      char charsToPrint[50];
      int i = 0;
      SEGGER_SYSVIEW_PrintfHost("Printing queue");
      xSemaphoreTake(printer->printMutex, portMAX_DELAY);

      // copy over string pointer to print from the queue
      while (xQueueReceive(printer->printQueue, &charsToPrint[i++], 0) ==
             pdPASS)
        ;
      xQueueReset(printer->printQueue);

      xSemaphoreGive(printer->printMutex);
      // make sure to end the command with null char
      charsToPrint[i] = '\0';
      SEGGER_SYSVIEW_PrintfHost("Printing %d chars", i);
      HAL_UART_Transmit_IT(printer->huartHandle, (void *)charsToPrint, i);
      // for(int j=0; j < i;j++){
      //   HAL_UART_Transmit_IT(&huart2, (void*)stringsToPrint[j],
      //   strnlen(stringsToPrint[j], (size_t)100));
      // }
    } else {
      // vTaskDelay(pdMS_TO_TICKS(200));
      //  Wait for the next cycle.
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
  }
}

size_t printMessage(PrintManager *printer, char *m, size_t numberChars) {

  if (!printer || !m || !printer->printMutex || !printer->printQueue) {
    // error, printer not intialized
    return 0;
  }

  size_t printedChars = 0;

  xSemaphoreTake(printer->printMutex, portMAX_DELAY);
  for (; printedChars < numberChars; printedChars++) {
    if (uxQueueSpacesAvailable(printer->printQueue) != 0) {
      xQueueSendToBack(printer->printQueue, &m[printedChars], portMAX_DELAY);
    } else {
      // reset queue for now
      /// xQueueReset(printer->printQueue);
      break;
    }
  }
  // xQueueSendToBack(printQueue, mainMenuMsg, portMAX_DELAY);
  xSemaphoreGive(printer->printMutex);

  return printedChars;
}

void printMessageBlocking(PrintManager *printer, char *m, size_t numberChars) {
  if (!printer || !m || !printer->printMutex || !printer->printQueue) {
    // error, printer not intialized
    return;
  }

  size_t printedChars = 0;

  xSemaphoreTake(printer->printMutex, portMAX_DELAY);
  while (printedChars < numberChars) {
    if (uxQueueSpacesAvailable(printer->printQueue) != 0) {
      xQueueSendToBack(printer->printQueue, &m[printedChars], portMAX_DELAY);
      printedChars++;
    } else {
      SEGGER_SYSVIEW_PrintfHost("Print queue overrun. Blocking");
    }
  }
  xSemaphoreGive(printer->printMutex);
}

void printManagerInit(PrintManager *printer, UART_HandleTypeDef *huartHandle) {
  printer->printQueue = xQueueCreate(100, sizeof(uint8_t));

  printer->printMutex = xSemaphoreCreateMutex();

  printer->printTaskHandle = xTaskCreateStatic(
      PrintTask, "PrintTask", STACK_SIZE, (void *)printer, tskIDLE_PRIORITY + 1,
      printer->PrintTaskStack, &(printer->PrintTaskTCB));

  printer->huartHandle = huartHandle;
}

BaseType_t isPrinterInitialized(PrintManager *printer) {
  if (!printer || !printer->printMutex || !printer->printQueue ||
      !printer->printTaskHandle || !printer->huartHandle) {
    return pdFALSE;
  }

  return pdTRUE;
}