


set(STM32F7xx_HAL_Driver_SOURCES 
    "./Src/stm32f7xx_hal.c"
    "./Src/stm32f7xx_hal_cortex.c"
    "./Src/stm32f7xx_hal_dma.c"
    "./Src/stm32f7xx_hal_dma_ex.c"
    "./Src/stm32f7xx_hal_exti.c"
    "./Src/stm32f7xx_hal_flash.c"
    "./Src/stm32f7xx_hal_flash_ex.c"
    "./Src/stm32f7xx_hal_gpio.c"
    "./Src/stm32f7xx_hal_i2c.c"
    "./Src/stm32f7xx_hal_i2c_ex.c"
    "./Src/stm32f7xx_hal_pwr.c"
    "./Src/stm32f7xx_hal_pwr_ex.c"
    "./Src/stm32f7xx_hal_rcc.c"
    "./Src/stm32f7xx_hal_rcc_ex.c"
    "./Src/stm32f7xx_hal_tim.c"
    "./Src/stm32f7xx_hal_tim_ex.c"
    "./Src/stm32f7xx_hal_uart.c"
    "./Src/stm32f7xx_hal_uart_ex.c"
)

set(STM32F7xx_HAL_Driver_HEADERS 
    ./Inc
)
#add_compile_definitions (${PROJECT_DEFINES})
##include_directories(${FreeRTOS_HEADERS})

add_library(STM32F7xx_HAL_Driver STATIC ${STM32F7xx_HAL_Driver_SOURCES})

target_include_directories(STM32F7xx_HAL_Driver PUBLIC ${STM32F7xx_HAL_Driver_HEADERS})

##target_link_libraries(SEGGER FreeRTOS)