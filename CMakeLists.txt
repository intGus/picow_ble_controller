cmake_minimum_required(VERSION 3.13)

include(src/pico_sdk_import.cmake)

project(flashprint C CXX ASM)
set(CMAKE_C STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w CACHE STRING "Board type")

pico_sdk_init()

add_executable(printflash
  src/main.c
  src/hog_host_test.c
)

pico_enable_stdio_usb(printflash 1)
pico_enable_stdio_uart(printflash 0)

target_include_directories(printflash PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

target_link_libraries(printflash
  pico_cyw43_arch_none
  pico_stdlib
  pico_btstack_cyw43
  pico_btstack_ble
  hardware_pwm
	hardware_gpio
  pico_multicore
)

pico_add_extra_outputs(printflash)