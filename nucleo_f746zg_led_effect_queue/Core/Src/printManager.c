#include <printManager.h>

static BaseType_t isPrinterInitialized(PrintManager const *);
static char charsToPrint[PRINT_QUEUE_LENGTH];

static void PrintTask(void *argument) {
  // the print manager is passed as an argument to the task
  PrintManager *printer = (PrintManager *)argument;
  assert_param(isPrinterInitialized(printer) == pdTRUE);

  SEGGER_SYSVIEW_PrintfHost("Print task running");
  while (1) {

    if (xSemaphoreTake(printer->readyForPrintSignal, portMAX_DELAY) == pdTRUE) {
      if (uxQueueMessagesWaiting(printer->printQueue) == 0) {
        SEGGER_SYSVIEW_PrintfHost("PRINT TSK: EMPTY QUEUE SIGNAL");
        continue;
      }
      int i = 0;
      SEGGER_SYSVIEW_PrintfHost("COPY QUEUE");
      //  copy over string pointer to print from the queue
      taskENTER_CRITICAL();
      while (xQueueReceive(printer->printQueue, &charsToPrint[i], 0) ==
             pdPASS) { // &&
                       // i <= PRINT_QUEUE_LENGTH) {
        i++;
        // this can happen if we get interrupted in the while loop and theres no
        // lock on the queue so any task can keep writing while we are emptying
        // at the same time.
      };
      taskEXIT_CRITICAL();
      SEGGER_SYSVIEW_PrintfHost("DONE COPY QUEUE");
      // make sure to end the command with null char
      charsToPrint[i] = '\0';
      SEGGER_SYSVIEW_PrintfHost("Printing %d chars", i);
      HAL_UART_Transmit_IT(printer->huartHandle, (void *)charsToPrint, i);
    }
  }
}

size_t printMessage(PrintManager const *const printer,
                    uint8_t const *const buffer, size_t const bufferLength) {

  assert_param(isPrinterInitialized(printer) == pdTRUE);

  size_t printedChars = 0;

  xSemaphoreTake(printer->printQueueMutex, portMAX_DELAY);
  for (; printedChars < bufferLength; printedChars++) {
    if (uxQueueSpacesAvailable(printer->printQueue) != 0) {
      xQueueSendToBack(printer->printQueue, &buffer[printedChars],
                       portMAX_DELAY);
    } else {
      break;
    }
  }
  xSemaphoreGive(printer->printQueueMutex);

  // give signal that there is something for the print task to print
  if (printedChars > 0) {
    xSemaphoreGive(printer->readyForPrintSignal);
  }

  return printedChars;
}

void printMessageBlocking(PrintManager const *const printer,
                          uint8_t const *const buffer,
                          size_t const bufferLength) {

  assert_param(isPrinterInitialized(printer) == pdTRUE);

  size_t printedChars = 0;
  // base number has to be proportional to the size of the queue
  BaseType_t expBackoffDelay = 0;
  BaseType_t delayBase = PRINT_DELAY_BASE;
  uint8_t numberOfSendRetries = 0;

  assert_param(xSemaphoreTake(printer->printQueueMutex, portMAX_DELAY) ==
               pdTRUE);

  while (printedChars < bufferLength) {
    if (uxQueueSpacesAvailable(printer->printQueue) > 0) {
      xQueueSendToBack(printer->printQueue, &buffer[printedChars],
                       portMAX_DELAY);
      printedChars++;
    } else {
      SEGGER_SYSVIEW_PrintfHost("Print queue overrun. Blocking");
      // enter a deleyed loop until the queue gets emptied
      // unclock the queue and wait for empty
      /// xSemaphoreGive(printer->printQueueMutex);
      xSemaphoreGive(printer->readyForPrintSignal);
      // wait untill the queue is empty
      while (uxQueueSpacesAvailable(printer->printQueue) !=
             PRINT_QUEUE_LENGTH) {

        expBackoffDelay =
            delayBase * ((BaseType_t)powf(2.0f, numberOfSendRetries));
        //  HAL_Delay(expBackoffDelay); // wait 10ms
        SEGGER_SYSVIEW_PrintfHost("Backoff: %d", expBackoffDelay);
        vTaskDelay(pdMS_TO_TICKS(expBackoffDelay));
        /// SEGGER_SYSVIEW_PrintfHost("Sleeping");
        // expBackoffDelay += 20;
        numberOfSendRetries++;
      }
      // take the mutex again to keep printing
      // xSemaphoreTake(printer->printQueueMutex, portMAX_DELAY);
    }
  }
  xSemaphoreGive(printer->printQueueMutex);
  // give signal that there is something for the print task to
  // print
  if (uxQueueMessagesWaiting(printer->printQueue) > 0) {
    xSemaphoreGive(printer->readyForPrintSignal);
  }
}

void printManagerInit(PrintManager *printer, UART_HandleTypeDef *huartHandle) {
  assert_param(huartHandle != NULL);
  printer->huartHandle = huartHandle;

  printer->printQueue = xQueueCreate(PRINT_QUEUE_LENGTH, sizeof(uint8_t));
  assert_param(printer->printQueue != NULL);
  configASSERT(printer->printQueue != NULL);

  printer->printQueueMutex = xSemaphoreCreateMutex();
  assert_param(printer->printQueueMutex != NULL);

  printer->readyForPrintSignal = xSemaphoreCreateBinary();
  assert_param(printer->readyForPrintSignal != NULL);

  printer->printTaskHandle =
      xTaskCreateStatic(PrintTask, PRINT_MANAGER_TASK_NAME, STACK_SIZE,
                        (void *)printer, PRINT_MANAGER_TASK_PRIORITY,
                        printer->PrintTaskStack, &(printer->PrintTaskTCB));
  assert_param(printer->printTaskHandle != NULL);
}

BaseType_t isPrinterInitialized(PrintManager const *const printer) {
  if (!printer || !printer->printQueueMutex || !printer->readyForPrintSignal ||
      !printer->printQueue || !printer->printTaskHandle ||
      !printer->huartHandle) {
    return pdFALSE;
  }
  return pdTRUE;
}
