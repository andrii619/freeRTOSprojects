
#include <userCallbacks.h>

#include <ledEffect.h>
#include <printManager.h>

#include <ctype.h>
#include <stdint.h>

uint8_t uartRxData;

extern PrintManager printer;
extern LEDEffect ledController;

/**
 * @brief  Rx Transfer completed callback.
 * @param  huart UART handle.
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  /* Prevent unused argument(s) compilation warning */
  // BaseType_t static notifyCommandTask = pdFALSE;
  // UNUSED(huart);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_RxCpltCallback can be implemented in the user file.
   */

  // immediatelly send the character to echo back to the user bypassing the task
  // print queue enqueue the char into the print queue instead of sending it
  // directly
  // if data is a char echo it back to the user
  if (uartRxData != 13) {
    HAL_UART_Transmit_IT(huart, &uartRxData, sizeof(uint8_t));
  } else {
    HAL_UART_Transmit_IT(huart, (uint8_t *)"\r\n", 2);
  }

  // xQueueSendToBackFromISR(printQueue, &uartRxData, NULL);

  // our data should hold the recieved byte
  SEGGER_SYSVIEW_PrintfHost("UART RxCplt Callback %d", uartRxData);
  // SEGGER_SYSVIEW_PrintfHost("UART RxCplt Callback %c", (char)uartRxData);

  // put the data into the queue
  // suspend scheduler to make sure that no task tries to empty the queue
  // while we write to it
  if (uartRxData != 13) {
    SEGGER_SYSVIEW_PrintfHost("Enqueue %c", (char)uartRxData);
    // if(xQueueIsQueueFullFromISR(inputDataQueue)==pdFALSE){
    //   xQueueSendToBackFromISR(inputDataQueue, &uartRxData, NULL);
    // }
    // else{
    //   //queue is full and we still dont have \n to end a command
    //   //empty the queue
    //   xQueueReset(inputDataQueue);
    // }

    /// printMessage(&printer, (char *)&uartRxData, 1);
  } else {
    // notify the command parsing task that a command arrived
    //  dont need to put the \n into the queue
    // xTaskNotifyFromISR( TaskHandle_t xTaskToNotify,
    //                             uint32_t ulValue,
    //                             eNotifyAction eAction,
    //                             BaseType_t *pxHigherPriorityTaskWoken );
    SEGGER_SYSVIEW_PrintfHost("Notifycmd %c", (char)uartRxData);
    // xTaskNotifyFromISR( commandParseTaskHandle,
    //                             (uint32_t) 0,
    //                             eNoAction,
    //                             &notifyCommandTask );
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
  printMessageFromISR(&printer, (uint8_t *)"BUTTON!", 7);
  ledController.led_mode++;
  if (ledController.led_mode > LED_EFFECT_CMD_EVEN_ODD) {
    ledController.led_mode = LED_EFFECT_CMD_OFF;
  }

  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}