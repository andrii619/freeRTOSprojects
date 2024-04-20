#include <printManager.h>

//static BaseType_t isPrinterInitialized(PrintManager const *);

static void PrintTask(void *argument) {
  // the print manager is passed as an argument to the task
  PrintManager *printer = (PrintManager *)argument;
  // assert_param(isPrinterInitialized(printer) == pdTRUE);

  // TickType_t xLastWakeTime;
  // const TickType_t xFrequency = pdMS_TO_TICKS(200);

  SEGGER_SYSVIEW_PrintfHost("Print task running");
  // Initialise the xLastWakeTime variable with the current time.
  // xLastWakeTime = xTaskGetTickCount();
  while (1) {

    if (xSemaphoreTake(printer->readyForPrintSignal, portMAX_DELAY) == pdTRUE) {
      // the printing queue is not empty
      // print the entire queue and go to blocking state untill theres more to
      // print in the queue
      // assert that if we are here then the queue is not empty
      // if it is empty then someone generated signal by error so we just lock
      // up
      // assert_param(uxQueueMessagesWaiting(printer->printQueue) != 0);
      if (uxQueueMessagesWaiting(printer->printQueue) == 0) {
        SEGGER_SYSVIEW_PrintfHost("PRINT TSK: EMPTY QUEUE SIGNAL");
        continue;
      }

      char charsToPrint[50];
      int i = 0;
      SEGGER_SYSVIEW_PrintfHost("Printing queue");

      // copy over string pointer to print from the queue
      // xSemaphoreTake(printer->printQueueMutex, portMAX_DELAY);
      while (xQueueReceive(printer->printQueue, &charsToPrint[i], 0) ==
             pdPASS) {
        i++;
      };
      // make sure to end the command with null char
      charsToPrint[i] = '\0';
      SEGGER_SYSVIEW_PrintfHost("Printing %d chars", i);
      HAL_UART_Transmit_IT(printer->huartHandle, (void *)charsToPrint, i);
    }
  }
}

size_t printMessage(PrintManager const *const printer,
                    uint8_t const *const buffer, size_t const bufferLength) {

  /// assert_param(isPrinterInitialized(printer) == pdTRUE);

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

  /// assert_param(isPrinterInitialized(printer) == pdTRUE);

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

  // TODO: Increase queue size at the end.
  // TODO: Made queue really small (20 chars)to test race conditions for tight
  // TODO: resources
  printer->printQueue = xQueueCreate(PRINT_QUEUE_LENGTH, sizeof(uint8_t));
  assert_param(printer->printQueue != NULL);

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

// BaseType_t isPrinterInitialized(PrintManager const *const printer) {
//   if (!printer || !printer->printQueueMutex || !printer->readyForPrintSignal ||
//       !printer->printQueue || !printer->printTaskHandle ||
//       !printer->huartHandle) {
//     return pdFALSE;
//   }
//   return pdTRUE;
// }
