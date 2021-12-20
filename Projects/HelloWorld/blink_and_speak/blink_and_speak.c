#include <stdio.h>
#include "pico/stdlib.h"

int main(){
    // Initialize I/O
    stdio_init_all();

    // Initialize GPIO pin 25 (green LED)
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    // Main loop
    while(1) {
        gpio_put(25, 1);    // drive LED
        printf("LED On!\n");
        sleep_ms(1000);     // 1 second delay

        gpio_put(25, 0);    // stop driving LED
        printf("LED Off!\n");
        sleep_ms(1000);     // 1 second delay
    }
}