#ifndef __PRINT_MANAGER_H
#define __PRINT_MANAGER_H

#include "stm32f7xx_hal.h"

#include <appCommon.h>

#include <math.h>
#include <string.h>

/**
 * @brief Length of the PrinterManager internal queue to hold data.
 * Bigger queue uses more memory but it means that the printer will be able to
 * print more data in one shot.
 */
#define PRINT_QUEUE_LENGTH (50)

/**
 * @brief Base time delay for exponential backoff algorithm. Used in
 * printMessageBlocking to introduce a time delay if the Backend can't keep up
 * with the amount of data that is being printed. Prevents the task using this
 * API from abusing system resources. Also the print task might not even get to
 * print right away because it might get preempted.
 */
#define PRINT_DELAY_BASE (PRINT_QUEUE_LENGTH / 5)

/**
 * @brief The PrinterManager is a Backend resource manager for printing data.
 * Currently only supports UART as a Backend. The PrintManager is a Caller owned
 * instance. Do not access internal members directly. Struct only exposed to
 * reserve memory for the Handle.
 *
 * The internal RTOS Task that actually prints data to Backend is not exposed.
 * The exposed APIs are only to help callers to print data in a way that
 * prevents race conditions or misusing the Backend.
 */
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

/**
 * @brief Initializes the PrinterManager instance.
 *
 * @param printer - pointer to the PrinterManager Handle
 * @param uartHandle - pointer to the UART Backend Handle.
 */
void printManagerInit(PrintManager *printer, UART_HandleTypeDef *uartHandle);

/**
 * @brief
 *
 * @param printer - pointer to an initialized PrintManager Handle
 * @param buffer - pointer to a char buffer
 * @param bufferLength - number of characters to print
 * @return size_t returns the number of characters that were actually printed
 * without blocking. If an error occurrs the functions terminates according to
 * samurai principle.
 * Do not use from an ISR
 */
size_t printMessage(PrintManager const *const printer,
                    uint8_t const *const buffer, size_t bufferLength);

/**
 * @brief
 *
 * @param printer
 * @param buffer
 * @param bufferLength
 * Do not use from an ISR
 */
void printMessageBlocking(PrintManager const *const printer,
                          uint8_t const *const buffer,
                          size_t const bufferLength);

#endif //__PRINT_MANAGER_H