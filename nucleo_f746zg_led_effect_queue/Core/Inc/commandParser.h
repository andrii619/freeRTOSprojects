#ifndef __COMMAND_PARSER_H
#define __COMMAND_PARSER_H

#include <appCommon.h>

typedef struct
{
  TaskHandle_t commandParserTaskHandle;

  StackType_t CommandParseTaskStack[STACK_SIZE];
  StaticTask_t CommandParseTaskTCB;
} CommandParser;

void commandParserInit (CommandParser *parser);

BaseType_t isCommandParserInitialized (CommandParser *);

#endif // __COMMAND_PARSER_H