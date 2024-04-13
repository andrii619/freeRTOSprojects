#ifndef __PRINT_MANAGER_H
#define __PRINT_MANAGER_H


// #include <FreeRTOS.h>
// #include <SEGGER_SYSVIEW.h>

// #include <task.h>
#include <appCommon.h>

#include <string.h>
//TaskHandle_t printTaskHandle;


typedef struct {
    TaskHandle_t printTaskHandle;
    
    StackType_t PrintTaskStack[STACK_SIZE];
    StaticTask_t PrintTaskTCB;
    
    QueueHandle_t printQueue;
    SemaphoreHandle_t printMutex;
    
    //pointer to uart handle that this printer manages
    UART_HandleTypeDef *huartHandle;
    //uint8_t initialized;
} PrintManager;

//private task function
//void PrintTask(void *argument);

void printManagerInit(PrintManager *printer, UART_HandleTypeDef *uartHandle);


size_t printMessage(PrintManager *printer,char *m, size_t numberChars);


void printMessageBlocking(PrintManager *printer,char *m, size_t numberChars);



BaseType_t isPrinterInitialized(PrintManager *);

#endif //__PRINT_MANAGER_H