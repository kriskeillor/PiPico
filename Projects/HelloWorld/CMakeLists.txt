cmake_minimum_required(VERSION 3.12)

# Pull in SDK (before project)
include(pico_sdk_import.cmake)

project(HelloWorld C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK ver 1.3.0 or later required; you have version ${PICO_SDK_VERSION_STRING}")
endif()

pico_sdk_init()

add_compile_options(-Wall
    -Wno-format                 # int != int32_t in gcc
    -Wno-unused-function
    -Wno-maybe-unitialized)

add_subdirectory(blink_and_speak)
