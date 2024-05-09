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
cmake_minimum_required(VERSION 3.20)


set (PROJECT_NAME             "nucleo_f746zg_led_effect")
set (PROJECT_TYPE             "exe")

project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES C ASM)

set(CMAKE_TOOLCHAIN_FILE "./cubeide-gcc.cmake")
include(${CMAKE_TOOLCHAIN_FILE})

# ###################### CONSTANTS ######################################
set (PROJECT_TYPE_EXECUTABLE          "exe")
set (PROJECT_TYPE_STATIC_LIBRARY      "static-lib")
set (MCPU_CORTEX_M0				      "-mcpu=cortex-m0")
set (MCPU_CORTEX_M0PLUS				  "-mcpu=cortex-m0plus")
set (MCPU_CORTEX_M3				      "-mcpu=cortex-m3")
set (MCPU_CORTEX_M4				      "-mcpu=cortex-m4")
set (MCPU_CORTEX_M7				      "-mcpu=cortex-m7")
set (MCPU_CORTEX_M33				  "-mcpu=cortex-m33")
set (MCPU_CORTEX_M55				  "-mcpu=cortex-m55")
set (MCPU_CORTEX_M85				  "-mcpu=cortex-m85")
set (MFPU_FPV4_SP_D16                 "-mfpu=fpv4-sp-d16")
set (MFPU_FPV5_D16                    "-mfpu=fpv5-d16")
set (RUNTIME_LIBRARY_REDUCED_C        "--specs=nano.specs")
set (RUNTIME_LIBRARY_STD_C            "")
set (RUNTIME_LIBRARY_SYSCALLS_MINIMAL "--specs=nosys.specs")
set (RUNTIME_LIBRARY_SYSCALLS_NONE    "")
set (MFLOAT_ABI_SOFTWARE              "-mfloat-abi=soft")
set (MFLOAT_ABI_HARDWARE              "-mfloat-abi=hard")
set (MFLOAT_ABI_MIX                   "-mfloat-abi=softfp")

# #######################################################################

# ###################### VARIABLES ######################################

set (LINKER_SCRIPT            ${PROJECT_SOURCE_DIR}/STM32F746ZGTX_FLASH.ld)
set (MCPU                     "-mcpu=Cortex-M7")
set (MFPU                 "-mfpu=fpv5-sp-d16")
set (MFLOAT_ABI               ${MFLOAT_ABI_HARDWARE})
set (RUNTIME_LIBRARY          "--specs=nano.specs")
set (RUNTIME_LIBRARY_SYSCALLS "--specs=nosys.specs")




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

set (CMAKE_EXECUTABLE_SUFFIX ".elf")
set (CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set (CMAKE_C_FLAGS "${MCPU} ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror")
set (CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} ${RUNTIME_LIBRARY_SYSCALLS} -Wl,-Map=test.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")
set (CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
add_compile_definitions(${PROJECT_DEFINES})
include_directories(${PROJECT_INCLUDES})

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