#include <commandParser.h>

static void CommandParseTask(void *argument) {
  SEGGER_SYSVIEW_PrintfHost("Command task running");
  //   uint8_t userCommand[30];
  //   BaseType_t notificationValue = pdFALSE;
  // 	while(1){

  // 		//wait to get notified that the uart queue holds a command to
  // be proccessed 		notificationValue =  xTaskNotifyWait(0,
  // 0, 		                             NULL,
  // 		                             portMAX_DELAY);//wait indefinitely
  //     if(notificationValue==pdTRUE){
  //       SEGGER_SYSVIEW_PrintfHost("CMD got notified");
  //       memset(userCommand, 0, sizeof(uint8_t)*30);
  //       ///BaseType_t gotChar = pdFALSE;
  //       int i=0;
  //       taskENTER_CRITICAL();
  //       //empty queue data by copying it into a local char array
  //       // read from the queue and return immediatelly if no data is
  //       available
  //       //while(xQueueReceive(inputDataQueue, (userCommand+i), 0) ==
  //       pdPASS){
  //       //  i++;
  //       //}
  //       while(xQueueReceive(inputDataQueue, &userCommand[i++], 0) ==
  //       pdPASS); xQueueReset(inputDataQueue);
  //       /**
  //        * @brief
  //        * uxQueueMessagesWaiting - number of items in queue
  //        */
  //       taskEXIT_CRITICAL();
  //       //print the data we got to SEGGER
  //       SEGGER_SYSVIEW_PrintfHost("Got cmd:%s", (char*)userCommand);

  //       switch(appState){
  //         case(sMainMenu): {
  //           if(strncmp((char*)userCommand, "one", (size_t)3)==0){
  //             xTaskNotifyIndexed(menuTaskHandle, 0, MENU_CMD_ONE,
  //             eSetValueWithOverwrite);
  //           }
  //           else if(strncmp((char*)userCommand, "two", (size_t)3)==0){
  //             xTaskNotifyIndexed(menuTaskHandle, 0, MENU_CMD_TWO,
  //             eSetValueWithOverwrite);
  //           }
  //           else if(strncmp((char*)userCommand, "three", (size_t)5)==0){
  //             xTaskNotifyIndexed(menuTaskHandle, 0, MENU_CMD_THREE,
  //             eSetValueWithOverwrite);
  //           }
  //           else {
  //             xTaskNotifyIndexed(menuTaskHandle, 0, MENU_CMD_BAD_CMD,
  //             eSetValueWithOverwrite);
  //           }
  //         }; break;
  //         case(sLedEffect):{
  //           if(strncmp((char*)userCommand, "one", (size_t)3)==0){
  //             xTaskNotifyIndexed(ledTaskHandle, 0, LED_EFFECT_CMD_ONE,
  //             eSetValueWithOverwrite);
  //           }
  //           else if(strncmp((char*)userCommand, "two", (size_t)3)==0){
  //             xTaskNotifyIndexed(ledTaskHandle, 0, LED_EFFECT_CMD_TWO,
  //             eSetValueWithOverwrite);
  //           }
  //           else if(strncmp((char*)userCommand, "three", (size_t)5)==0){
  //             xTaskNotifyIndexed(ledTaskHandle, 0, LED_EFFECT_CMD_THREE,
  //             eSetValueWithOverwrite);
  //           }
  //           else {
  //             xTaskNotifyIndexed(ledTaskHandle, 0, LED_EFFECT_CMD_BAD_CMD,
  //             eSetValueWithOverwrite);
  //           }
  //         }; break;
  //         case(sRtcMenu):
  //         case(sRtcTimeConfig):
  //         case(sRtcDateConfig):
  //         case(sRtcReport):{
  //           if(strncmp((char*)userCommand, "one", (size_t)3)==0){
  //             xTaskNotifyIndexed(rtcTaskHandle, 0, RTC_MENU_CMD_ONE,
  //             eSetValueWithOverwrite);
  //           }
  //           else if(strncmp((char*)userCommand, "two", (size_t)3)==0){
  //             xTaskNotifyIndexed(rtcTaskHandle, 0, RTC_MENU_CMD_TWO,
  //             eSetValueWithOverwrite);
  //           }
  //           else if(strncmp((char*)userCommand, "three", (size_t)5)==0){
  //             xTaskNotifyIndexed(rtcTaskHandle, 0, RTC_MENU_CMD_THREE,
  //             eSetValueWithOverwrite);
  //           }
  //           else {
  //             xTaskNotifyIndexed(rtcTaskHandle, 0, RTC_MENU_CMD_BAD_CMD,
  //             eSetValueWithOverwrite);
  //           }
  //         }; break;
  //         default:{

  //         }
  //       }
  //     }
  //     //else case should not happen
  // 	}
  while (1) {
    SEGGER_SYSVIEW_PrintfHost("Command task running");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void commandParserInit(CommandParser *parser) {

  assert_param(parser);

  parser->inputDataQueue =
      xQueueCreate(INPUT_DATA_QUEUE_LENGTH, sizeof(uint8_t));
  assert_param(parser->inputDataQueue);

  parser->commandParserTaskHandle = xTaskCreateStatic(
      CommandParseTask, COMMAND_PARSER_TASK_NAME, STACK_SIZE, (void *)parser,
      COMMAND_PARSER_TASK_PRIORITY, parser->CommandParseTaskStack,
      &(parser->CommandParseTaskTCB));
  assert_param(parser->commandParserTaskHandle);
};

BaseType_t isCommandParserInitialized(CommandParser *) { return pdFALSE; };
