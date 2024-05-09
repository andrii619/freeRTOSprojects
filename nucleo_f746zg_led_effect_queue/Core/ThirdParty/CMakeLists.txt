#cmake_minimum_required(VERSION 3.20)

#project(ThirdPartyLibraries VERSION 1.0.0 LANGUAGES C ASM)

# set(CMAKE_C_STANDARD          17)
# set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS        ON)




# set (PROJECT_TYPE_EXECUTABLE          "exe")
# set (PROJECT_TYPE_STATIC_LIBRARY      "static-lib")
# set (MCPU_CORTEX_M0				      "-mcpu=cortex-m0")
# set (MCPU_CORTEX_M0PLUS				  "-mcpu=cortex-m0plus")
# set (MCPU_CORTEX_M3				      "-mcpu=cortex-m3")
# set (MCPU_CORTEX_M4				      "-mcpu=cortex-m4")
# set (MCPU_CORTEX_M7				      "-mcpu=cortex-m7")
# set (MCPU_CORTEX_M33				  "-mcpu=cortex-m33")
# set (MCPU_CORTEX_M55				  "-mcpu=cortex-m55")
# set (MCPU_CORTEX_M85				  "-mcpu=cortex-m85")
# set (MFPU_FPV4_SP_D16                 "-mfpu=fpv4-sp-d16")
# set (MFPU_FPV5_D16                    "-mfpu=fpv5-d16")
# set (RUNTIME_LIBRARY_REDUCED_C        "--specs=nano.specs")
# set (RUNTIME_LIBRARY_STD_C            "")
# set (RUNTIME_LIBRARY_SYSCALLS_MINIMAL "--specs=nosys.specs")
# set (RUNTIME_LIBRARY_SYSCALLS_NONE    "")
# set (MFLOAT_ABI_SOFTWARE              "-mfloat-abi=soft")
# set (MFLOAT_ABI_HARDWARE              "-mfloat-abi=hard")
# set (MFLOAT_ABI_MIX                   "-mfloat-abi=softfp")




#set (RUNTIME_LIBRARY_SYSCALLS "--specs=nosys.specs")


##set (CMAKE_EXE_LINKER_FLAGS "${RUNTIME_LIBRARY_SYSCALLS} -Wl,-Map=test.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")

add_subdirectory(FreeRTOS)
add_subdirectory(SEGGER)

#cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#cmake --build ./Debug/