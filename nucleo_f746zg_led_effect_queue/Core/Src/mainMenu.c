
#include <mainMenu.h>


void MenuTask(void *argument) {
	SEGGER_SYSVIEW_PrintfHost("Menu task running");
  
  //print our menu
  if(appState == sMainMenu){
    xSemaphoreTake(printMutex, portMAX_DELAY);
    for(size_t i =0; i<strlen(mainMenuMsg);i++){
      if(uxQueueSpacesAvailable(printQueue)!=0){
        xQueueSendToBack(printQueue, &mainMenuMsg[i], portMAX_DELAY);
      }
      else{
        break;
      }
    }
    //xQueueSendToBack(printQueue, mainMenuMsg, portMAX_DELAY);
    xSemaphoreGive(printMutex);
  }
  
	while(1){

	}
}