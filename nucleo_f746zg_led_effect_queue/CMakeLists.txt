# #############################################################################################################################
# # file:  CMakeLists.txt
# # brief: Template "CMakeLists.txt" for building of executables and static libraries.
# #
# # usage: Edit "VARIABLES"-section to suit project requirements.
# #        For debug build:
# #          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
# #          make -C Debug VERBOSE=1 -j
# #        For release build:
# #          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
# #          make -C Release VERBOSE=1 -j
# #############################################################################################################################

# ###################### PROJECT Variables ######################################
cmake_minimum_required(VERSION 3.20)


set (PROJECT_NAME             "nucleo_f746zg_led_effect")
set (PROJECT_TYPE             "exe")
set (CMAKE_EXPORT_COMPILE_COMMANDS ON) # needed by clang-tidy
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES C ASM)
# #######################################################################



# ###################### CMAKE Modules ######################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/) #append dir
#list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)

set(TOOLCHAIN_CMAKE_FILE "GCCToolchain")
set(DOCS_CMAKE_FILE	"Docs")
##include(${TOOLCHAIN_CMAKE_FILE})#include cmake module files

# Set the toolchain file
#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchains/arm-none-eabi-gcc.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/toolchains/arm-none-eabi-gcc.cmake)

include(${DOCS_CMAKE_FILE})
include(Tools) # clang-format cmake-format clang-tidy
# #######################################################################

# ###################### VARIABLES ######################################


set (PROJECT_SOURCES
	# LIST SOURCE FILES HERE
	./Core/Startup/startup_stm32f746zgtx.s
	./Core/Src/main.c
	./Core/Src/syscalls.c
	./Core/Src/sysmem.c
	./Core/Src/commandParser.c
	./Core/Src/ledEffect.c
	./Core/Src/mainMenu.c
	./Core/Src/printManager.c
	./Core/Src/rtcConfig.c
	./Core/Src/stm32f7xx_hal_msp.c
	./Core/Src/stm32f7xx_hal_timebase_tim.c
	./Core/Src/stm32f7xx_it.c
	./Core/Src/system_stm32f7xx.c
	./Core/Src/userCallbacks.c
	./Core/Src/utils.c
	)

set (PROJECT_DEFINES
	# LIST COMPILER DEFINITIONS HERE
	USE_FULL_ASSERT
	USE_HAL_DRIVER
	STM32F746xx
    )

set (PROJECT_INCLUDES
	# LIST INCLUDE DIRECTORIES HERE
	./Core/Inc
	./Drivers/STM32F7xx_HAL_Driver/Inc
	./Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
	./Drivers/CMSIS/Device/ST/STM32F7xx/Include
	./Drivers/CMSIS/Include
	./Core/ThirdParty/FreeRTOS/include
	./Core/ThirdParty/FreeRTOS/portable/GCC/ARM_CM7
	./Core/ThirdParty/SEGGER/Config
	./Core/ThirdParty/SEGGER/SEGGER
	./Core/ThirdParty/SEGGER/OS
    )

# ############ MODIFY ACCORDING TO REQUIREMENTS) ########################

# #######################################################################

# ################## PROJECT SETUP ######################################

# enable_language(ASM)

# if (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_EXECUTABLE})
#   add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
#   add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)
# elseif (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_STATIC_LIBRARY})
#   add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
# endif()

set(CMAKE_C_STANDARD          17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS        ON)


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
add_compile_definitions(${PROJECT_DEFINES})
include_directories(${PROJECT_INCLUDES})


add_cppcheck_to_target(${PROJECT_NAME})
#add_clang_tidy_to_target(${PROJECT_NAME})

add_subdirectory(./Core/ThirdParty)
add_subdirectory(./Drivers)

target_link_libraries(${PROJECT_NAME} STM32F7xx_HAL_Driver FreeRTOS SEGGER)

#cmake -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#cmake --build ./Debug/

#Compiler flags
# set (MCPU                     "-mcpu=Cortex-M7")
# set (MFLOAT_ABI               "-mfloat-abi=hard")
# set (MFPU                 "-mfpu=fpv5-sp-d16")
# set (RUNTIME_LIBRARY          "--specs=nano.specs")
# set(INSTRUCTION_LEVEL "-mthumb")

# set (CMAKE_C_FLAGS "${MCPU} ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} ${INSTRUCTION_LEVEL} -Wall -Werror")
# set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")