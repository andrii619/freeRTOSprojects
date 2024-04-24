#ifndef __COMMAND_PARSER_H
#define __COMMAND_PARSER_H

#include "stm32f7xx_hal.h"
#include <appCommon.h>

#define INPUT_DATA_QUEUE_LENGTH (50)

typedef struct
{
  TaskHandle_t commandParserTaskHandle;

  StackType_t CommandParseTaskStack[STACK_SIZE];
  StaticTask_t CommandParseTaskTCB;
  
   QueueHandle_t inputDataQueue;
  
} CommandParser;

void commandParserInit (CommandParser *parser);

BaseType_t isCommandParserInitialized (CommandParser *);

#endif // __COMMAND_PARSER_H