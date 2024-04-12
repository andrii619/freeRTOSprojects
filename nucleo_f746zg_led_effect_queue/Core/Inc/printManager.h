#ifndef __PRINT_MANAGER_H
#define __PRINT_MANAGER_H


// #include <FreeRTOS.h>
// #include <SEGGER_SYSVIEW.h>

// #include <task.h>
#include <appCommon.h>

TaskHandle_t printTaskHandle;
void PrintTask(void *argument);

#endif //__PRINT_MANAGER_H