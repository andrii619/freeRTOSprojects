
#include <userCallbacks.h>

#include <commandParser.h>
#include <ledEffect.h>
#include <printManager.h>

//#include <task.h>

#include <ctype.h>
#include <stdint.h>

extern PrintManager printer;
extern LEDEffect ledController;
extern CommandParser parser;

/**
 * @brief  Rx Transfer completed callback.
 * @param  huart UART handle.
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  /* Prevent unused argument(s) compilation warning */
  BaseType_t notifyCommandTask = pdFALSE;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  //  UNUSED(huart);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_RxCpltCallback can be implemented in the user file.
   */
  // our data should hold the recieved byte
  SEGGER_SYSVIEW_PrintfHost("UART RxCplt Callback %d", parser.uartRxData);
  // SEGGER_SYSVIEW_PrintfHost("UART RxCplt Callback %c", (char)uartRxData);

  // put the data into the queue
  // its not the job of ISR to know if this char is valid.
  // we simply pass data to task and let task handle the rest
  SEGGER_SYSVIEW_PrintfHost("Enqueue %c", (char)parser.uartRxData);
  if (xQueueIsQueueFullFromISR(parser.inputDataQueue) == pdFALSE) {
    xQueueSendToBackFromISR(parser.inputDataQueue, &parser.uartRxData, NULL);
    if (parser.uartRxData == '\r') {
      // notify the command parsing task that a command arrived
      SEGGER_SYSVIEW_PrintfHost("Notifycmd %d", parser.uartRxData);
      xTaskNotifyFromISR(parser.commandParserTaskHandle, (uint32_t)0, eNoAction,
                         &notifyCommandTask);
      // Yield if a higher priority task was woken by this ISR
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    // recieve the next data character only if data queue is not full
    // if the data queue is full
    HAL_UART_Receive_IT(huart, &parser.uartRxData, sizeof(uint8_t));
  } else {
    // notify that the data queue needs to be emptied and the UART Rx
    // data reception restarted again
    xTaskNotifyFromISR(parser.commandParserTaskHandle, (uint32_t)0, eNoAction,
                       &notifyCommandTask);
    // Yield if a higher priority task was woken by this ISR
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_ErrorCallback can be implemented in the user file.
   */
  SEGGER_SYSVIEW_PrintfHost("UART Error Callback");
}

/**
 * @brief  EXTI line detection callbacks.
 * @param  GPIO_Pin Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  /* Prevent unused argument(s) compilation warning */
  // UNUSED(GPIO_Pin);
  SEGGER_SYSVIEW_PrintfHost("GPIO_EXTI_Callback %d", GPIO_Pin);

  /**
   * @brief
   * to get a message printed from ISR maybe use a counting semaphore or a flag
   */
  // TODO: Button debouncing
  ledController.led_mode++;
  if (ledController.led_mode > LED_EFFECT_CMD_EVEN_ODD) {
    ledController.led_mode = LED_EFFECT_CMD_OFF;
  }

  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}