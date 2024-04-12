#ifndef __MAIN_MENU_H
#define __MAIN_MENU_H

// #include <FreeRTOS.h>
// #include <SEGGER_SYSVIEW.h>

// #include <task.h>

#include <appCommon.h>

#include <string.h>

char const * const mainMenuMsg = "Main Menu:\r\n0:Led Menu\r\n1:Rtc Menu\r\n2:Exit\r\n";
TaskHandle_t menuTaskHandle;



void MenuTask(void *argument);


#endif // __MAIN_MENU_H