cmake_minimum_required(VERSION 3.16)
project(ID_Solution C CXX ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# Target: STM32F103VE (Cortex-M3, 512KB Flash, 64KB RAM)
set(MCU_FLASH_SIZE 512K)
set(MCU_RAM_SIZE 64K)

# Toolchain
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)

set(WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Working)

# Compiler flags (list style - no quoting issues)
set(CPU_FLAGS -mcpu=cortex-m3 -mthumb)
set(CFLAGS_COMMON ${CPU_FLAGS} -fdata-sections -ffunction-sections -Wall
  -Wno-implicit-function-declaration
  -Wno-implicit-int
  -Wno-incompatible-pointer-types
  -Wno-int-conversion
)
set(CFLAGS_DEBUG -g -O0)
set(CFLAGS_RELEASE -O1)

# Defines (Main.h 에서 CH2, NEW_BOARD, SENSOR_PH_EC 사용)
set(C_DEFINES
  USE_STDPERIPH_DRIVER
  STM32F10X_HD
  USE_STM3210B_EVAL
  NEW_BOARD
  SENSOR_PH_EC
  CH2
  # HAL for ethernet
  STM32F103xE
  USE_HAL_DRIVER
)

# STM32F10x device header (stm32f10x.h)
# Keil pack: C:/Keil/ARM/PACK/Keil/STM32F1xx_DFP/2.3.0/Device/Include
# Or copy from STM32F10x_StdPeriph_Lib to Working/CMSIS/Device/ST/STM32F10x/Include/
set(INCLUDE_DIRS_PRE "")
set(STM32F10X_DEVICE_INCLUDE "${WORKING_DIR}/CMSIS/Device/ST/STM32F10x/Include")
if(EXISTS "${STM32F10X_DEVICE_INCLUDE}/stm32f10x.h")
  list(APPEND INCLUDE_DIRS_PRE "${STM32F10X_DEVICE_INCLUDE}")
endif()
# Keil DFP pack (new style)
if(WIN32 AND EXISTS "C:/Keil/ARM/PACK/Keil/STM32F1xx_DFP")
  file(GLOB KEIL_DFP_VERS "C:/Keil/ARM/PACK/Keil/STM32F1xx_DFP/*")
  if(KEIL_DFP_VERS)
    list(GET KEIL_DFP_VERS 0 KEIL_DFP_VER)
    if(EXISTS "${KEIL_DFP_VER}/Device/Include")
      list(APPEND INCLUDE_DIRS_PRE "${KEIL_DFP_VER}/Device/Include")
    endif()
  endif()
endif()
# Keil legacy include (C:\Keil\ARM\INC\ST\STM32F10x)
if(WIN32 AND EXISTS "C:/Keil/ARM/INC/ST/STM32F10x")
  list(APPEND INCLUDE_DIRS_PRE "C:/Keil/ARM/INC/ST/STM32F10x")
endif()

# Include paths (uvproj 기준)
set(INCLUDE_DIRS
  ${INCLUDE_DIRS_PRE}
  ${WORKING_DIR}/Include_Files
  ${WORKING_DIR}/StdPeriph_Driver/inc
  ${WORKING_DIR}/CMSIS
  ${WORKING_DIR}/FATFS_V0.08A/src
  ${WORKING_DIR}/Image
  ${WORKING_DIR}/LwIP/system
  ${WORKING_DIR}/LwIP/system/arch
  ${WORKING_DIR}/LwIP/src/include
  ${WORKING_DIR}/LwIP/src/include/ipv4
  ${WORKING_DIR}/LwIP/src/include/lwip
  ${WORKING_DIR}/LwIP/src/include/netif
  ${WORKING_DIR}/LwIP/src/include/posix
  ${WORKING_DIR}/LwIP/src/include/posix/sys
  ${WORKING_DIR}/LwIP/src/core
  ${WORKING_DIR}/Inc_ethernet
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
  ${WORKING_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Include
  ${WORKING_DIR}/Drivers/CMSIS/Include
  ${WORKING_DIR}
)

# Source files - Application
# Note: ReDisplay_ch*.c are #included directly by main.c via #ifdef CH2/CH4/else
# Do NOT add them here as separate compilation units to avoid duplicate symbols.
set(APP_SOURCES
  ${WORKING_DIR}/Source_Files/Display.c
  ${WORKING_DIR}/Source_Files/Initialize.c
  ${WORKING_DIR}/Source_Files/Interrupt.c
  ${WORKING_DIR}/Source_Files/main.c
  ${WORKING_DIR}/Source_Files/NandInterface.c
  ${WORKING_DIR}/Source_Files/NAND.c
  ${WORKING_DIR}/Source_Files/Tft.c
  ${WORKING_DIR}/Source_Files/RTC_Time.c
  ${WORKING_DIR}/Source_Files/serial.c
  ${WORKING_DIR}/Source_Files/Flash.c
  ${WORKING_DIR}/Source_Files/Watchdog.c
  ${WORKING_DIR}/Source_Files/DAC7512Interface.c
  ${WORKING_DIR}/Source_Files/Communication.c
  ${WORKING_DIR}/Source_Files/Sensor_Manager.c
  ${WORKING_DIR}/Source_Files/sensor_ph.c
  ${WORKING_DIR}/Source_Files/sensor_ec.c
  ${WORKING_DIR}/Source_Files/sensor_autodetect.c
  ${WORKING_DIR}/Source_Files/sensor_adc.c
  ${WORKING_DIR}/Source_Files/Adc_Read.c
  ${WORKING_DIR}/Source_Files/Button.c
  ${WORKING_DIR}/Source_Files/NANDDisplay.c
  ${WORKING_DIR}/Source_Files/CurrentSorceTrans.c
  ${WORKING_DIR}/Source_Files/MicroSd.c
  ${WORKING_DIR}/Source_Files/sdio_sd.c
  ${WORKING_DIR}/Source_Files/eeprom.c
  ${WORKING_DIR}/Source_Files/display_icon.c
  ${WORKING_DIR}/Source_Files/icon_ch2_ch4.c
  ${WORKING_DIR}/Source_Files/display_update.c
  ${WORKING_DIR}/Source_Files/ui_mode.c
  ${WORKING_DIR}/Source_Files/ReDisplay_ch1.c
  ${WORKING_DIR}/Source_Files/ReDisplay_ch4.c
)

# StdPeriph Driver
set(STDPERIPH_SOURCES
  ${WORKING_DIR}/StdPeriph_Driver/src/misc.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_adc.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_bkp.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_flash.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_fsmc.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_gpio.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_iwdg.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_pwr.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_rcc.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_rtc.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_sdio.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_spi.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_tim.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_usart.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_dma.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_dac.c
  ${WORKING_DIR}/StdPeriph_Driver/src/stm32f10x_i2c.c
)

# CMSIS (StdPeriph용)
set(CMSIS_SOURCES
  ${WORKING_DIR}/CMSIS/core_cm3.c
  ${WORKING_DIR}/CMSIS/system_stm32f10x.c
)

# Startup (GCC용)
set(STARTUP_SOURCE
  ${WORKING_DIR}/startup_stm32f10x_hd_gcc.s
)

# FATFS
set(FATFS_SOURCES
  ${WORKING_DIR}/FATFS_V0.08A/src/diskio.c
  ${WORKING_DIR}/FATFS_V0.08A/src/ff.c
)

# LwIP core
set(LWIP_SOURCES
  ${WORKING_DIR}/LwIP/src/core/def.c
  ${WORKING_DIR}/LwIP/src/core/dhcp.c
  ${WORKING_DIR}/LwIP/src/core/dns.c
  ${WORKING_DIR}/LwIP/src/core/init.c
  ${WORKING_DIR}/LwIP/src/core/mem.c
  ${WORKING_DIR}/LwIP/src/core/memp.c
  ${WORKING_DIR}/LwIP/src/core/netif.c
  ${WORKING_DIR}/LwIP/src/core/pbuf.c
  ${WORKING_DIR}/LwIP/src/core/raw.c
  ${WORKING_DIR}/LwIP/src/core/stats.c
  ${WORKING_DIR}/LwIP/src/core/sys.c
  ${WORKING_DIR}/LwIP/src/core/tcp.c
  ${WORKING_DIR}/LwIP/src/core/tcp_in.c
  ${WORKING_DIR}/LwIP/src/core/tcp_out.c
  ${WORKING_DIR}/LwIP/src/core/timers.c
  ${WORKING_DIR}/LwIP/src/core/udp.c
  ${WORKING_DIR}/LwIP/src/core/ipv4/autoip.c
  ${WORKING_DIR}/LwIP/src/core/ipv4/icmp.c
  ${WORKING_DIR}/LwIP/src/core/ipv4/igmp.c
  ${WORKING_DIR}/LwIP/src/core/ipv4/inet.c
  ${WORKING_DIR}/LwIP/src/core/ipv4/inet_chksum.c
  ${WORKING_DIR}/LwIP/src/core/ipv4/ip.c
  ${WORKING_DIR}/LwIP/src/core/ipv4/ip_addr.c
  ${WORKING_DIR}/LwIP/src/core/ipv4/ip_frag.c
  ${WORKING_DIR}/LwIP/src/netif/etharp.c
  ${WORKING_DIR}/LwIP/src/netif/ethernetif.c
  ${WORKING_DIR}/LwIP/src/netif/slipif.c
)

# Ethernet application (ENC28J60, Modbus)
set(ETHERNET_SOURCES
  ${WORKING_DIR}/source_ethernet/app_ethernet.c
  ${WORKING_DIR}/source_ethernet/enc28j60.c
  ${WORKING_DIR}/source_ethernet/main_ethernet.c
  ${WORKING_DIR}/source_ethernet/modbus_client.c
  ${WORKING_DIR}/source_ethernet/ethernetif_1.c
  ${WORKING_DIR}/source_ethernet/system_stm32f1xx.c
  ${WORKING_DIR}/source_ethernet/stm32f1xx_it.c
)

# HAL Driver (Ethernet용)
set(HAL_SOURCES
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac_ex.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_dma.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
  ${WORKING_DIR}/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
)

# All sources
set(ALL_SOURCES
  ${APP_SOURCES}
  ${STDPERIPH_SOURCES}
  ${CMSIS_SOURCES}
  ${STARTUP_SOURCE}
  ${FATFS_SOURCES}
  ${LWIP_SOURCES}
  ${ETHERNET_SOURCES}
  ${HAL_SOURCES}
)

# Create executable
add_executable(${PROJECT_NAME}.elf ${ALL_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${INCLUDE_DIRS})

# Compile definitions
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE ${C_DEFINES})

# C flags
target_compile_options(${PROJECT_NAME}.elf PRIVATE
  $<$<CONFIG:Debug>:${CFLAGS_DEBUG}>
  $<$<CONFIG:Release>:${CFLAGS_RELEASE}>
  ${CFLAGS_COMMON}
)

# Linker script
set(LINKER_SCRIPT ${WORKING_DIR}/STM32F103VETx_FLASH.ld)

# Linker flags
target_link_options(${PROJECT_NAME}.elf PRIVATE
  -T${LINKER_SCRIPT}
  ${CPU_FLAGS}
  -specs=nano.specs
  -specs=nosys.specs
  -Wl,-Map=${PROJECT_NAME}.map,--cref
  -Wl,--gc-sections
)

# Generate HEX and BIN
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
  COMMENT "Generating HEX and BIN"
)

# Output directory
set_target_properties(${PROJECT_NAME}.elf PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
