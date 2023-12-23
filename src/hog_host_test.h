// hog_host_test.h

#ifndef HOG_HOST_TEST_H
#define HOG_HOST_TEST_H

// Include necessary dependencies for the Pico

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// Declare functions in hog_host_test.c
typedef void (*BootKeyboardEventHandler)(const uint8_t *data, uint16_t size);

int btstack_main(int argc, const char *argv[]);

#endif // HOG_HOST_TEST_H