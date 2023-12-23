#include "btstack_run_loop.h"
#include "hog_host_test.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/multicore.h"

#define SIGNAL_PIN 14
#define MUSIC_PIN 28
#define BUTTON_PIN 0

uint8_t ly = 130;
uint8_t buttons = 0;

void driveForward() {
    for(int i=0; i<=3; i++) {
		gpio_put(SIGNAL_PIN, 1);
		sleep_us(1500);
		gpio_put(SIGNAL_PIN, 0);
		sleep_us(500);
	}
		for(int i=0; i<=9; i++) {
		gpio_put(SIGNAL_PIN, 1);
		sleep_us(500);
		gpio_put(SIGNAL_PIN, 0);
		sleep_us(500);
	}
}

void driveBackward() {
    for(int i=0; i<=3; i++) {
        gpio_put(SIGNAL_PIN, 1);
        sleep_us(1500);
        gpio_put(SIGNAL_PIN, 0);
        sleep_us(500);
    }
    for(int i=0; i<=39; i++) {
        gpio_put(SIGNAL_PIN, 1);
        sleep_us(500);
        gpio_put(SIGNAL_PIN, 0);
        sleep_us(500);
    }
}

void second_core_loop() {
    printf("Second core started\n");
    while (1) {
    if (ly < 100) {
        printf("calling Forward\n");
        driveForward();
    } else if (ly > 150) {
        driveBackward();
        printf("Backward\n");
    } else {
        gpio_put(SIGNAL_PIN, 0);
    }

    if (buttons > 0 || !gpio_get(BUTTON_PIN)) {
        gpio_put(MUSIC_PIN, 1);
    } else {
        gpio_put(MUSIC_PIN, 0);
    }


    // Add a delay to avoid excessive loop iteration
    sleep_ms(20);
    }
}

void registerBootKeyboardEventHandler(BootKeyboardEventHandler callback);

// Event handler function
void myBootKeyboardEventHandler(const uint8_t *data, uint16_t size) {
    // Print the data
    uint8_t lx =          data[1];
    ly =          data[3];
    uint8_t rx =          data[5];
    uint8_t ry =          data[7];
    buttons =     data[13]; 
    uint8_t hat =         data[12];

    printf("Left Stick: X: %d, Y: %d - Right Stick: X: %d, Y: %d - Hat: %d - Buttons: %d\n", lx, ly, rx, ry, hat, buttons);
    
    // control the car

}

int main() {
    // Initialize standard Pico SDK functions
    stdio_init_all();

    gpio_init(SIGNAL_PIN);
	gpio_init(MUSIC_PIN);
	gpio_init(BUTTON_PIN);

	gpio_set_dir(SIGNAL_PIN, GPIO_OUT);
	gpio_set_dir(MUSIC_PIN, GPIO_OUT);
	gpio_set_dir(BUTTON_PIN, GPIO_IN);
	gpio_pull_up(BUTTON_PIN);
    
    // Initialize BTstack UART
	if (cyw43_arch_init()) {
		printf("Wi-Fi init failed\n");
		return -1;
	}

    multicore_launch_core1(second_core_loop);
    
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(500);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(500);

    printf ("BTstack on Pico starting...\n");

    // Setup example
    btstack_main(0, NULL);

    registerBootKeyboardEventHandler(myBootKeyboardEventHandler);

    // // Enter run loop (forever)
    btstack_run_loop_execute();

    return 0;
}