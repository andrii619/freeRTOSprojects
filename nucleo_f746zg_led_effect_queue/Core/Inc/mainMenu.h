#ifndef __MAIN_MENU_H
#define __MAIN_MENU_H

// #include <FreeRTOS.h>
// #include <SEGGER_SYSVIEW.h>

// #include <task.h>

#include <appCommon.h>

///#include <string.h>


//TaskHandle_t menuTaskHandle;


typedef struct {
    TaskHandle_t        menuTaskHandle;
    StackType_t         MenuTaskStack[STACK_SIZE];
    StaticTask_t        MenuTaskTCB;
    
} MainMenu;

//static void MenuTask(void *argument);


void mainMenuInit(MainMenu *);

BaseType_t isMainMenuInitialized(MainMenu *);


#endif // __MAIN_MENU_H