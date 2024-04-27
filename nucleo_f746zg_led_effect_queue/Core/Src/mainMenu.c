
#include <mainMenu.h>

#include <printManager.h>

static char *mainMenuMsg =
    "Main Menu:\r\n0:Led Menu\r\n1:Rtc Menu\r\n2:Exit\r\n";
extern PrintManager printer;

static void MenuTask(void *argument) {
  SEGGER_SYSVIEW_PrintfHost("Menu task running");

  MainMenu *menu = (MainMenu *)argument;

  assert_param(isMainMenuInitialized(menu) == pdTRUE);

  size_t msgLen = strlen(mainMenuMsg);
  uint8_t menuItenationNum = 0;
  while (1) {

    // printMessage(&printer, mainMenuMsg, msgLen);
    SEGGER_SYSVIEW_PrintfHost("Print main menu %d", menuItenationNum++);
    //printMessageBlocking(&printer, (uint8_t *)mainMenuMsg, msgLen);
    //HAL_Delay(100); // wait 10ms
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void mainMenuInit(MainMenu *menu) {
  menu->menuTaskHandle = xTaskCreateStatic(
      MenuTask, MAIN_MENU_TASK_NAME, STACK_SIZE, (void *)menu,
      MAIN_MENU_TASK_PRIORITY, menu->MenuTaskStack, &(menu->MenuTaskTCB));
}

BaseType_t isMainMenuInitialized(MainMenu *menu) {
  if (!menu->menuTaskHandle) {
    return pdFALSE;
  }
  return pdTRUE;
}