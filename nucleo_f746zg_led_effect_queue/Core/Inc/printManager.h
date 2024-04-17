#ifndef __PRINT_MANAGER_H
#define __PRINT_MANAGER_H

// #include <FreeRTOS.h>
// #include <SEGGER_SYSVIEW.h>

// #include <task.h>
#include <appCommon.h>

#include <math.h>
#include <string.h>
// TaskHandle_t printTaskHandle;

typedef struct {
  TaskHandle_t printTaskHandle;

  StackType_t PrintTaskStack[STACK_SIZE];
  StaticTask_t PrintTaskTCB;

  QueueHandle_t printQueue;
  SemaphoreHandle_t printQueueMutex;
  SemaphoreHandle_t readyForPrintSignal;

  // pointer to uart handle that this printer manages
  UART_HandleTypeDef *huartHandle;
  // uint8_t initialized;
} PrintManager;

void printManagerInit(PrintManager *printer, UART_HandleTypeDef *uartHandle);

size_t printMessage(PrintManager *printer, char *m, size_t numberChars);

void printMessageBlocking(PrintManager *printer, char *m, size_t numberChars);

#endif //__PRINT_MANAGER_H