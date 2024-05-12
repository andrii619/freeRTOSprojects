#include <commandParser.h>

#include <string.h>
#include <utils.h>
#include <ledEffect.h>

static BaseType_t commandParserStart(CommandParser *const parser);

/**
 * @brief Construct a new parse Command object
 * 
 * @param cmd - pointer to a command_t data structure
 * @param buffer - pointer to a string
 */
static void parseCommand(command_t *cmd, char * buffer);

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

extern LEDEffect ledController;

static void CommandParseTask(void *argument) {
  SEGGER_SYSVIEW_PrintfHost("Command task running");
  uint8_t userCommand[INPUT_DATA_QUEUE_LENGTH + 1];
  userCommand[INPUT_DATA_QUEUE_LENGTH] =
      '\0'; // safeguard for some string methods like strlen
  size_t currentBufferFillLevel = 0;
  //char *token;

  CommandParser *parser = (CommandParser *)argument;
  BaseType_t notificationValue = pdFALSE;
  
  /**
   * @brief Construct a new command Parser Start object
   * NOTE: Cannot start the uart receive IT inside of main 
   * probably because main ends its thread of execution when the schedulers starts so the
   * interrupt cant really be invoked from main anymore
   * so we need to start the uart IT RX process from here
   */
  commandParserStart(parser);

  BaseType_t reEnableUartNeeded = pdFALSE;

  while (1) {
    SEGGER_SYSVIEW_PrintfHost("Command task running");
    
    reEnableUartNeeded = pdFALSE;
    
    notificationValue = xTaskNotifyWait(0, 0, NULL,
                                        portMAX_DELAY); // wait indefinitely
    if (notificationValue == pdFALSE) {
      continue;
    }
    
    if(uxQueueSpacesAvailable(parser->inputDataQueue) == 0){
     //
     reEnableUartNeeded = pdTRUE;
    }
    

    // if(uxQueueSpacesAvailable()==0){
    //   SEGGER_SYSVIEW_PrintfHost("");
    //   ///continue;
    // }

    // got notification value
    // either the queue is full or we got a command
    // int i = 0;
    // xSemaphoreTake(parser->inputDataMutex, portMAX_DELAY);

    // we might have multiple finished commands and an unfinished command in the
    // queue
    while (xQueueReceive(parser->inputDataQueue,
                         &userCommand[currentBufferFillLevel], 0) == pdPASS) {
      currentBufferFillLevel++;
      // if we get interrupted during emptying the queue and
      // the ISR puts even more chars into the queue
      if (currentBufferFillLevel > INPUT_DATA_QUEUE_LENGTH) {
        break;
      }
    }
    //string guard
    userCommand[currentBufferFillLevel] = '\0';
    
    // create a command struct
    command_t cmd = {0, NULL};
    
    // parse the input string into a command
    parseCommand(&cmd, (char*)userCommand);
    
    // for now just pass the command as is
    cmd.arg_count = 1;
    cmd.args = pvPortMalloc(cmd.arg_count * sizeof(char *));
    cmd.args[0] = pvPortMalloc(currentBufferFillLevel * sizeof(char));
    strncpy(cmd.args[0], (char*)userCommand, currentBufferFillLevel);
    //size_t cmdSize = strlen();
    
    //put the command into the led command queue
    // item is queued by copy
    xQueueSendToBack(ledController.commandQueue, &cmd, portMAX_DELAY);
    

    // tokenize the commands by \r\n
    // trim the tokens of any whitespace characters
    // for (int i = 0;; i++) {
    //   token = strtok((char *)&userCommand, "\r\n");

    //   // trim whitespace
    //   token = trimnwhitespace(token, INPUT_DATA_QUEUE_LENGTH);
    //   //int commandLength = 
      
    //   //strcpy();
    //   // we can tokenize the command here into the command part and the arguments part
      
    // }

    currentBufferFillLevel = 0;
    
    if(reEnableUartNeeded){
      // uart reception was disabled because the parser input queue was full
      // re-enable the uart RX process
      HAL_UART_Receive_IT(parser->huartHandle, &parser->uartRxData, sizeof(uint8_t));
    }

    // send them one by one to the relevant task

    SEGGER_SYSVIEW_PrintfHost("Command task NOTIFIED");
  }
}

void commandParserInit(CommandParser *parser, UART_HandleTypeDef *uartHandle) {

  assert_param(parser);
  assert_param(uartHandle);

  parser->inputDataQueue =
      xQueueCreate(INPUT_DATA_QUEUE_LENGTH, sizeof(uint8_t));
  assert_param(parser->inputDataQueue);

  parser->commandParserTaskHandle = xTaskCreateStatic(
      CommandParseTask, COMMAND_PARSER_TASK_NAME, STACK_SIZE, (void *)parser,
      COMMAND_PARSER_TASK_PRIORITY, parser->CommandParseTaskStack,
      &(parser->CommandParseTaskTCB));
  assert_param(parser->commandParserTaskHandle);

  parser->huartHandle = uartHandle;
};

BaseType_t isCommandParserInitialized(CommandParser *parser) { 
  UNUSED(parser);
  return pdFALSE;
  };

BaseType_t commandParserStart(CommandParser *const parser) {
  assert_param(parser);
  HAL_StatusTypeDef pass = HAL_UART_Receive_IT(
      parser->huartHandle, &parser->uartRxData, sizeof(uint8_t));
  if (pass != HAL_OK) {
    SEGGER_SYSVIEW_PrintfHost("Cmd parser failed to start uart");
    return pdFAIL;
  }
  return pdPASS;
}




void parseCommand(command_t *cmd, char * buffer) {
  assert_param(cmd && buffer);
  
  /**
   * @brief 
   * 1) tokenize the buffer based on space characters
   * 2) check of we have at least one token
   * 3) assume that the command is valid. its not our job to check validity
   * 4) put the command and arguments into the struct
   */
  //char *token;
  
  
}

void freeCommandResources(command_t *cmd) {
  assert_param(cmd && cmd->arg_count > 0 && cmd->args);
    // free each command string
  for (int i = 0; i < cmd->arg_count; i++) {
    if(cmd->args[i]){
      vPortFree(cmd->args[i]);
      cmd->args[i] = NULL;
    }
  }
  if(cmd->args){
    vPortFree(cmd->args);
    cmd->args = NULL;
  }
}