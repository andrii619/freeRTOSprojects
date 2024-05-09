


set(SEGGER_SOURCES 
    "./Config/SEGGER_SYSVIEW_Config_FreeRTOS.c"
    "./OS/SEGGER_SYSVIEW_FreeRTOS.c"
    "./SEGGER/Syscalls/SEGGER_RTT_Syscalls_GCC.c"
    "./SEGGER/SEGGER_RTT.c"
    "./SEGGER/SEGGER_RTT_ASM_ARMv7M.S"
    "./SEGGER/SEGGER_RTT_printf.c"
    "./SEGGER/SEGGER_SYSVIEW.c"
)

set(SEGGER_HEADERS 
    ./Config
    ./OS
    ./SEGGER
)
#add_compile_definitions (${PROJECT_DEFINES})
##include_directories(${FreeRTOS_HEADERS})

add_library(SEGGER STATIC ${SEGGER_SOURCES})

target_include_directories(SEGGER PUBLIC ${SEGGER_HEADERS})

target_link_libraries(SEGGER FreeRTOS)