


set(FreeRTOS_SOURCES 
    "timers.c"
    "tasks.c"
    "stream_buffer.c"
    "queue.c"
    "list.c"
    "event_groups.c"
    "croutine.c"
    "cmsis_os2.c"
    "./portable/MemMang/heap_4.c"
    "./portable/GCC/ARM_CM7/port.c")

set(FreeRTOS_HEADERS 
    ./include
    ./portable/GCC/ARM_CM7
#    "atomic.h"
#    "croutine.h"
#    "deprecated_definitions.h"
#    "event_groups.h"
#    "FreeRTOS.h"
#    "FreeRTOSConfig.h"
#    "list.h"
#    "message_buffer.h"
#    "mpu_prototypes.h"
#    "mpu_syscall_numbers.h"
#    "mpu_wrappers.h"
#    "newlib-freertos.h"
#    "picolibc-freertos.h"
#    "portable.h"
#    "projdefs.h"
#    "queue.h"
#    "semphr.h"
#    "stack_macros.h"
#    "StackMacros.h"
#    "stream_buffer.h"
#    "task.h"
#    "timers.h"
#    "portmacro.h"
)
#add_compile_definitions (${PROJECT_DEFINES})
##include_directories(${FreeRTOS_HEADERS})

add_library(FreeRTOS STATIC ${FreeRTOS_SOURCES})

target_include_directories(FreeRTOS PUBLIC ./include ./portable/GCC/ARM_CM7)

target_link_libraries(FreeRTOS SEGGER)