
#ifndef __LED_EFFECT_H
#define __LED_EFFECT_H

// #include <FreeRTOS.h>
// #include <SEGGER_SYSVIEW.h>

// #include <task.h>

#include <appCommon.h>

char const * const ledMenuMsg = "LED Menu:\r\n0:Led OFF\r\n1:LED Sequential\r\n2:Exit\r\n";


TaskHandle_t ledTaskHandle;
void LEDTask(void *argument);

#endif //__LED_EFFECT_H