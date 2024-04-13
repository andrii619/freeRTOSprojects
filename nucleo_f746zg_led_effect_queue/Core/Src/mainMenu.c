
#include <mainMenu.h>

#include <printManager.h>


char * mainMenuMsg = "Main Menu:\r\n0:Led Menu\r\n1:Rtc Menu\r\n2:Exit\r\n";
extern PrintManager printer;

static void MenuTask(void *argument) {
	SEGGER_SYSVIEW_PrintfHost("Menu task running");
  
  MainMenu *menu = (MainMenu*)argument;
  
  if(isMainMenuInitialized(menu)==pdFALSE){
    SEGGER_SYSVIEW_PrintfHost("Menu not initialized");
    vTaskSuspend(menu->menuTaskHandle);
  }
  
  //print our menu
  // if(appState == sMainMenu){
  //   xSemaphoreTake(printMutex, portMAX_DELAY);
  //   for(size_t i =0; i<strlen(mainMenuMsg);i++){
  //     if(uxQueueSpacesAvailable(printQueue)!=0){
  //       xQueueSendToBack(printQueue, &mainMenuMsg[i], portMAX_DELAY);
  //     }
  //     else{
  //       break;
  //     }
  //   }
  //   //xQueueSendToBack(printQueue, mainMenuMsg, portMAX_DELAY);
  //   xSemaphoreGive(printMutex);
  // }
  
  size_t msgLen = strlen(mainMenuMsg);
  
	while(1){
    
    //printMessage(&printer, mainMenuMsg, msgLen);
    
    printMessageBlocking(&printer, mainMenuMsg, msgLen);
    
    vTaskDelay(pdMS_TO_TICKS(500));
	}
}


void mainMenuInit(MainMenu *menu){
  menu->menuTaskHandle = xTaskCreateStatic(MenuTask, "MenuTask", STACK_SIZE, NULL,
     					tskIDLE_PRIORITY + 1,
     					menu->MenuTaskStack, &(menu->MenuTaskTCB));
}



BaseType_t isMainMenuInitialized(MainMenu *menu){
  if(!menu->menuTaskHandle){
    return pdFALSE;
  }
  return pdTRUE;
}