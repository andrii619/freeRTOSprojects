#ifndef __RTC_CONFIG_H
#define __RTC_CONFIG_H


#include <appCommon.h>

#include "stm32f7xx_hal.h"

void RTCTask(void *argument);

TaskHandle_t rtcTaskHandle;

#endif // __RTC_CONFIG_H