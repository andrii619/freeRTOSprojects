#include <printManager.h>


void PrintTask(void *argument) {
//   TickType_t xLastWakeTime;
//   const TickType_t xFrequency = pdMS_TO_TICKS(200);
  
 	SEGGER_SYSVIEW_PrintfHost("Print task running");
  
//   // Initialise the xLastWakeTime variable with the current time.
//   xLastWakeTime = xTaskGetTickCount();
  
// 	while(1){
//     if(uxQueueMessagesWaiting(printQueue)!=0){
      
//       char charsToPrint[50];
//       // array of pointers to constant char
//       ///char const * stringsToPrint[20];
//       int i =0;
//       SEGGER_SYSVIEW_PrintfHost("Printing queue");
//       xSemaphoreTake(printMutex, portMAX_DELAY);
      
//       // copy over string pointer to print from the queue
//       while(xQueueReceive(printQueue, &charsToPrint[i++], 0) == pdPASS);
//       xQueueReset(printQueue);
      
//       xSemaphoreGive(printMutex);
//       //make sure to end the command with null char
//       charsToPrint[i] = '\0';
//       HAL_UART_Transmit_IT(&huart2, (void*)charsToPrint, strnlen(charsToPrint, (size_t)50));
//       // for(int j=0; j < i;j++){
//       //   HAL_UART_Transmit_IT(&huart2, (void*)stringsToPrint[j], strnlen(stringsToPrint[j], (size_t)100));
//       // }
      
//     }
//     else{
//       //vTaskDelay(pdMS_TO_TICKS(200));
//       // Wait for the next cycle.
//       vTaskDelayUntil( &xLastWakeTime, xFrequency );
//     }
// 	}

while(1){
    
}
}