cmake_minimum_required(VERSION 3.22)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

#
# Project Setup
#
project(micromouse_firmware)

enable_language(C)
enable_language(CXX)
enable_language(ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Build Options
#
option(DESKTOP_TESTING "ON: Build natively for desktop testing, OFF: Build for MicroMouse deployment" OFF)

if(DESKTOP_TESTING)
  message(STATUS "~~~~~~~ Building for desktop testing ~~~~~~~")

  #
  # Google Test
  #
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        f8d7d77
  )

  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  FetchContent_MakeAvailable(googletest)

else()
  message(STATUS "~~~~~~~ Building for mouse ~~~~~~~")

  #
  # Toolchain
  #
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  include(gcc-arm-none-eabi)

endif()

#
# Built Type
#
if(NOT CMAKE_BUILD_TYPE)
  if(DESKTOP_TESTING)
    set(CMAKE_BUILD_TYPE "Debug")
  else()
    set(CMAKE_BUILD_TYPE "Release")
  endif()
endif()

if(DESKTOP_TESTING AND CMAKE_BUILD_TYPE STREQUAL "Release")
  message(WARNING "Building desktop testing in release mode is not allowed. Switching to debug mode.")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Build type: " ${CMAKE_BUILD_TYPE})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()




#
# Build
#
if(DESKTOP_TESTING)
  message(FATAL_ERROR "Desktop testing not implemented yet.")

else()
  add_executable(${CMAKE_PROJECT_NAME})

  target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    USE_HAL_DRIVER 
    STM32WB55xx
      $<$<CONFIG:Debug>:DEBUG>
  )

  target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
      "Core/Inc"
      "STM32_WPAN/App"
      "USB_Device/App"
      "USB_Device/Target"
      "Drivers/STM32WBxx_HAL_Driver/Inc"
      "Drivers/STM32WBxx_HAL_Driver/Inc/Legacy"
      "Utilities/lpm/tiny_lpm"
      "Middlewares/Third_Party/FreeRTOS/Source/include"
      "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2"
      "Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F"
      "Middlewares/ST/STM32_WPAN"
      "Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread"
      "Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl"
      "Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci"
      "Middlewares/ST/STM32_WPAN/utilities"
      "Middlewares/ST/STM32_WPAN/ble/core"
      "Middlewares/ST/STM32_WPAN/ble/core/auto"
      "Middlewares/ST/STM32_WPAN/ble/core/template"
      "Middlewares/ST/STM32_WPAN/ble/svc/Inc"
      "Middlewares/ST/STM32_WPAN/ble/svc/Src"
      "Middlewares/ST/STM32_USB_Device_Library/Core/Inc"
      "Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc"
      "Drivers/CMSIS/Device/ST/STM32WBxx/Include"
      "Middlewares/ST/STM32_WPAN/ble"
      "Drivers/CMSIS/Include"
  )

  file(GLOB_RECURSE SRC
    "Core/*.c"        "Core/*.cpp"
    "STM32_WPAN/*.c"  "STM32_WPAN/*.cpp"
    "USB_Device/*.c"  "USB_Device/*.cpp"
    "Drivers/*.c"     "Drivers/*.cpp"
    "Middlewares/*.c" "Middlewares/*.cpp"
    "Utilities/*.c"   "Utilities/*.cpp"
  )

  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${SRC} "startup_stm32wb55xx_cm4.s")

endif()
