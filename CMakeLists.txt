# #############################################################################
# #################        CMake Template (CUSTOM)       ######################
# #################    Copyright (c) 2023 ZJU   ######################
# #############################################################################

#Windows下构建后面需要添加 -G "Unix Makefiles"

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
#版本号
cmake_minimum_required(VERSION 3.22)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#mcu settings
set(CPU -mcpu=cortex-m7)
set(FPU -mfpu=fpv5-d16)
set(FLOAT-ABI -mfloat-abi=hard)


set(CMAKE_BUILD_TYPE "Debug")

# project settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# ########################## USER CONFIG SECTION ##############################
# set up proj
project(ROV C CXX ASM) # TODO
set(CMSISDSP components/algorithm) # if using DSP, modify your_dsp_path here
                                  # e.g. set(CMSISDSP Drivers/CMSIS/DSP)

# ! rebuild or use command line `cmake .. -D` to switch option
# floating point settings
option(ENABLE_HARD_FP "enable hard floating point" ON) # 开启硬件FPU
option(ENABLE_SOFT_FP "enable soft floating point" OFF) # 软件浮点运算
option(USE_NEW_VERSION_DSP "DSP version >= 1.10.0" OFF) # DSP版本大于等于1.10.0

# add inc and src here  
include_directories(
  #STM32_H7XX
  Drivers/STM32H7xx_HAL_Driver/Inc
  Drivers/BSP/Components/lan8742
  Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
  Drivers/CMSIS/Device/ST/STM32H7xx/Include 
  Drivers/CMSIS/Include 
  Inc
  #LWIP
  LWIP/App
  LWIP/Target
  ######core########
  Middlewares/Third_Party/LwIP/src/include
  Middlewares/Third_Party/LwIP/system

  #FREERTOS
  Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
  Middlewares/Third_Party/FreeRTOS/Source/include
  Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F

  #DSP
  if(ENABLE_HARD_FP)
  if(USE_NEW_VERSION_DSP)
  ${CMSISDSP}/Include/dsp
  ${CMSISDSP}/Include
  ${CMSISDSP}/PrivateInclude
  else()
  ${CMSISDSP}/Include
  endif()
  endif()

  # USER
  application
  application/protocol
  bsp
  components/algorithm
  components/controller
  components/devices
  components/suport

)


# !! Keep only sub folders required to build and use CMSIS-DSP Library.
# !! If DSP version >= 1.10, for all paths including DSP folders, plz add [^a] to filter DSP files.
# !! e.g. your_dsp_path = Drivers/CMSIS/DSP, use "Drivers/[^a]*.*" "${CMSISDSP}/[^a]*.*" 
file(GLOB_RECURSE SOURCES
  "Startup/*.*"
  "Src/*.c"
  "Drivers/*.c"
  "Middlewares/*.c"
  # USER
  "application/*.*"
  "bsp/*.*"
  "components/*.c"
)


# #############################################################################
if(ENABLE_HARD_FP)
  message(STATUS "Use FPU")

  if(USE_NEW_VERSION_DSP)
    message(STATUS "DSP version >= 1.10.0")
    add_compile_definitions(
      ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
  else()
    message(STATUS "DSP version < 1.10.0")
    add_compile_definitions(
      ARM_MATH_CM7;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING;__FPU_PRESENT=1U)
  endif()

  add_compile_options(-mfloat-abi=hard -mfpu=fpv5-d16)
  add_link_options(-mfloat-abi=hard -mfpu=fpv5-d16)
else()
  message(STATUS "Unuse FPU")
endif()

if(ENABLE_SOFT_FP)
  add_compile_options(-mfloat-abi=soft)
endif()

add_compile_options(-mcpu=cortex-m7 -mthumb -mthumb-interwork)
add_compile_options(--specs=nosys.specs -ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  message(STATUS "Maximum optimization for speed")
  add_compile_options(-Ofast)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
  message(STATUS "Maximum optimization for speed, debug info included")
  add_compile_options(-Ofast -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
  message(STATUS "Maximum optimization for size")
  add_compile_options(-Os)
else()
  message(STATUS "Minimal optimization, debug info included")
  add_compile_options(-Og -g)
endif()

add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32H743IITX_FLASH.ld)


add_link_options(
  -Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map
)
add_link_options(-mcpu=cortex-m7 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})


add_executable(${PROJECT_NAME}.elf ${SOURCES})

link_directories(${CMAKE_SOURCE_DIR}/${CMSISDSP})

target_link_libraries(${PROJECT_NAME}.elf  ${CMAKE_SOURCE_DIR}/${CMSISDSP}/arm_cortexM7lfdp_math.lib) #需要使用绝对路径


set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(
  TARGET ${PROJECT_NAME}.elf
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
  COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf>
  ${BIN_FILE}
  COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

