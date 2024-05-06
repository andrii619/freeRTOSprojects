#ifndef __COMMAND_PARSER_H
#define __COMMAND_PARSER_H

#include "stm32f7xx_hal.h"
#include <appCommon.h>

#define INPUT_DATA_QUEUE_LENGTH (50)

typedef struct {
  TaskHandle_t commandParserTaskHandle;

  StackType_t CommandParseTaskStack[STACK_SIZE];
  StaticTask_t CommandParseTaskTCB;

  QueueHandle_t inputDataQueue;

  uint8_t uartRxData;
  UART_HandleTypeDef *huartHandle;

  // SemaphoreHandle_t inputDataMutex;

} CommandParser;

// typedef enum {
//   sMainMenu = 0,
//   sLedEffect,
//   sRtcMenu,
//   sRtcTimeConfig,
//   sRtcDateConfig,
//   sRtcReport,
// } parser_queue_status_t;

/**
 * @brief Command Parser monitors input data over an interface and parses out
 * user commands. WHen a command arrives on an interface the current main task
 * gets notified of the command(s)
 *
 * @param parser
 * @param uartHandle
 */
void commandParserInit(CommandParser *parser, UART_HandleTypeDef *uartHandle);

BaseType_t isCommandParserInitialized(CommandParser *);

///BaseType_t commandParserStart(CommandParser *const parser);



// void commandParserStop();

#endif // __COMMAND_PARSER_H