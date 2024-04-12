/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  * Use JP3 to control power input on the Nucleo:
  * JP3 at UV5 is set to use USB 5v as input
  * JP3 at EV5 is set to use external 5V as input
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
///#define DWT_CTRL (*(volatile uint32_t*)0xE0001000)

//the address of Task2Handle is passed to xTaskCreate.
//this global variable will be used by Task3 to delete BlueTask.






app_state_t appState = sMainMenu;

/**
 * holds UART data. The UART interrupt gets data and puts it into this queue.
 * When the interrupt recieves the \n character it notifies
 * the commandParsingTask. Then the commandParsingTask empties and process the command.
 */
QueueHandle_t inputDataQueue; // holds UART input data

QueueHandle_t printQueue;

SemaphoreHandle_t printMutex;

// some common variables to use for each task
// 128 * 4 = 512 bytes
//(recommended min stack size per task)
#define STACK_SIZE 128

//define stack and task control block (TCB) for the red task

static StackType_t MenuTaskStack[STACK_SIZE];
static StaticTask_t MenuTaskTCB;

static StackType_t LEDTaskStack[STACK_SIZE];
static StaticTask_t LEDTaskTCB;

static StackType_t RTCTaskStack[STACK_SIZE];
static StaticTask_t RTCTaskTCB;

static StackType_t PrintTaskStack[STACK_SIZE];
static StaticTask_t PrintTaskTCB;

static StackType_t CommandParseTaskStack[STACK_SIZE];
static StaticTask_t CommandParseTaskTCB;


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  ///DWT_CTRL |=1;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  //HAL_UART_

  SEGGER_SYSVIEW_Conf();

  // call sysview start to start manual rtt recordings
  SEGGER_SYSVIEW_Start();
  //using an inlined if statement with an infinite while loop to stop in case
  //the task wasn't created successfully

  //
  //printQueue = xQueueCreate(100, sizeof(char const *));
  printQueue = xQueueCreate(100, sizeof(uint8_t));

  inputDataQueue = xQueueCreate(20, sizeof(uint8_t));
  
  printMutex = xSemaphoreCreateMutex();

  //xTaskCreateStatic returns task hanlde
  //always passes since memory was statically allocated
  menuTaskHandle = xTaskCreateStatic(MenuTask, "MenuTask", STACK_SIZE, NULL,
    					tskIDLE_PRIORITY + 1,
    					MenuTaskStack, &MenuTaskTCB);

  ledTaskHandle = xTaskCreateStatic(LEDTask, "LEDTask", STACK_SIZE, NULL,
      					tskIDLE_PRIORITY + 1,
      					LEDTaskStack, &LEDTaskTCB);

  rtcTaskHandle = xTaskCreateStatic(RTCTask, "RTCTask", STACK_SIZE, NULL,
        					tskIDLE_PRIORITY + 1,
        					RTCTaskStack, &RTCTaskTCB);

  printTaskHandle = xTaskCreateStatic(PrintTask, "PrintTask", STACK_SIZE, NULL,
        					tskIDLE_PRIORITY + 1,
        					PrintTaskStack, &PrintTaskTCB);

  commandParseTaskHandle = xTaskCreateStatic(CommandParseTask, "CommandParseTask",
		  	  STACK_SIZE, NULL,
			  tskIDLE_PRIORITY + 1,
			  CommandParseTaskStack, &CommandParseTaskTCB);
  
  //set initial app state to main menu
  appState = sMainMenu;

  // start the uart RX interrupt
  HAL_UART_Receive_IT(&huart2, &uartRxData, sizeof(uint8_t));

  //start the scheduler - shouldn't return unless there's a problem
  vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  //HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);//turn led on

	  //HAL_Delay(500);

	  //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

	  //HAL_Delay(500);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
	BaseType_t static notifyCommandTask = pdFALSE;
  UNUSED(huart);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_RxCpltCallback can be implemented in the user file.
   */

  //immediatelly send the character to echo back to the user bypassing the task print queue
  //enqueue the char into the print queue instead of sending it directly
  // if(uartRxData!=13){
  //   HAL_UART_Transmit_IT(huart, &uartRxData, sizeof(uint8_t));
  // }
  // else{
  //   HAL_UART_Transmit_IT(huart, (uint8_t*)"\r\n", 2);
  // }
  xQueueSendToBackFromISR(printQueue, &uartRxData, NULL);
  
  //our data should hold the recieved byte
  SEGGER_SYSVIEW_PrintfHost("UART RxCplt Callback %d", uartRxData);
  SEGGER_SYSVIEW_PrintfHost("UART RxCplt Callback %c", (char)uartRxData);

  // put the data into the queue
  // suspend scheduler to make sure that no task tries to empty the queue
  // while we write to it
  if(uartRxData != 13){
    SEGGER_SYSVIEW_PrintfHost("Enqueue %c", (char)uartRxData);
    if(xQueueIsQueueFullFromISR(inputDataQueue)==pdFALSE){
      xQueueSendToBackFromISR(inputDataQueue, &uartRxData, NULL);
    }
    else{
      //queue is full and we still dont have \n to end a command
      //empty the queue
      xQueueReset(inputDataQueue);
    }
	  
  }
  else {
	  //notify the command parsing task that a command arrived
	  // dont need to put the \n into the queue
    // xTaskNotifyFromISR( TaskHandle_t xTaskToNotify,
    //                             uint32_t ulValue,
    //                             eNotifyAction eAction,
    //                             BaseType_t *pxHigherPriorityTaskWoken );
    SEGGER_SYSVIEW_PrintfHost("Notifycmd %c", (char)uartRxData);
	  xTaskNotifyFromISR( commandParseTaskHandle,
                                (uint32_t) 0,
                                eNoAction,
                                &notifyCommandTask );
  }

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_ErrorCallback can be implemented in the user file.
   */
  SEGGER_SYSVIEW_PrintfHost("UART Error Callback");
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	SEGGER_SYSVIEW_PrintfHost("Assert Failed");
	while(1){

	}
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
