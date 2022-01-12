/**
 * Modified by KGK 2022
 * Now allows selection of scrolling left, right, or not at all 
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "board/oled_i2c.c"

/* Example code to talk to an SSD1306-based OLED display

   NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico
   GPIO (and therefore I2C) cannot be used at 5v.

   You will need to use a level shifter on the I2C lines if you want to run the
   board at 5v.

   Connections on Raspberry Pi Pico board, other boards may vary.

   GPIO PICO_DEFAULT_I2C_SDA_PIN (on Pico this is GP4 (pin 6)) -> SDA on display
   board
   GPIO PICO_DEFAULT_I2C_SCK_PIN (on Pico this is GP5 (pin 7)) -> SCL on
   display board
   3.3v (pin 36) -> VCC on display board
   GND (pin 38)  -> GND on display board
*/

// Refactored as a function --KGK
void render_multiple() {
    // render 3 cute little raspberries
    struct render_area area = {start_col: 0, end_col : IMG_WIDTH - 1, start_page : 0, end_page : OLED_NUM_PAGES - 1};
    calc_render_area_buflen(&area);
    //render(raspberry26x32, &area); // redundant if, below, i=0
    for (int i = 0; i < 3; i++) {
        uint8_t offset = 5 + IMG_WIDTH; // 5px padding
        area.start_col += offset;
        area.end_col += offset;
        render(raspberry26x32, &area);
    }
}

// Refactored as a function --KGK
void conf_horz_scroll(uint8_t horiz_scroll) {
    // configure horizontal scrolling
    oled_send_cmd(OLED_SET_HORIZ_SCROLL | horiz_scroll);    //0x00);
    oled_send_cmd(0x00); // dummy byte
    oled_send_cmd(0x00); // start page 0
    oled_send_cmd(0x00); // time interval
    oled_send_cmd(0x03); // end page 3
    oled_send_cmd(0x00); // dummy byte
    oled_send_cmd(0xFF); // dummy byte

    // LES GO 
    oled_send_cmd(OLED_SET_SCROLL | 0x01);
}

int main() {
    stdio_init_all();
    
    // Set state to be not scrolling --KGK
    scroll_state = 1;
    scroll_state_prev = 1;

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c / oled_i2d example requires a board with I2C pins
    puts("Default I2C pins were not defined");
#else
    // useful information for picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
    bi_decl(bi_program_description("OLED I2C example for the Raspberry Pi Pico"));

    printf("\nHello, OLED display! Look at my raspberries..\n");

    // I2C is "open drain", pull ups to keep signal high when no data is being
    // sent
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // Initialize GPIO buttons --KGK
    // Note - using Cytron Maker 1.2, buttons are pulled-up in hardware. 
    // Therefore, the gpio_pull_up function is not called in this code.
    gpio_init(GPIO20);
    gpio_init(GPIO21);
    gpio_init(GPIO22);
    gpio_set_dir(GPIO20, GPIO_IN);
    gpio_set_dir(GPIO21, GPIO_IN);
    gpio_set_dir(GPIO22, GPIO_IN);

    // run through the complete initialization process
    oled_init();

    // initialize render area for entire frame (128 pixels by 4 pages)
    struct render_area frame_area = {start_col: 0, end_col : OLED_WIDTH - 1, start_page : 0, end_page : OLED_NUM_PAGES -
                                                                                                        1};
    calc_render_area_buflen(&frame_area);

    // zero the entire display
    uint8_t buf[OLED_BUF_LEN];
    fill(buf, 0x00);
    render(buf, &frame_area);

    // intro sequence: flash the screen 3 times
    for (int i = 0; i < 3; i++) {
        oled_send_cmd(0xA5); // ignore RAM, all pixels on
        sleep_ms(500);
        oled_send_cmd(0xA4); // go back to following RAM
        sleep_ms(500);
    }

    // render 3 cute little raspberries
    struct render_area area = {start_col: 0, end_col : IMG_WIDTH - 1, start_page : 0, end_page : OLED_NUM_PAGES - 1};
    calc_render_area_buflen(&area);
    // Draw icons to screen
    render_multiple();

    // Disable scrolling
    oled_send_cmd(OLED_SET_SCROLL | 0x00);

    // Main loop --KGK
    while(1) {
        // Select Left scroll
        if (!gpio_get(GPIO20)) {                    // active low buttons
            printf("Left button pressed\n");
            if (scroll_state != 0) {
                scroll_state = 0;
                printf("New state: %d\n", scroll_state);
            } else { /* already in state */ }
        }
        // Select No scroll 
        else if (!gpio_get(GPIO21)) {
            printf("Middle button pressed\n");
            if (scroll_state != 1) {
                scroll_state = 1;
                printf("New state: %d\n", scroll_state);
            } else { /* already in state */ }
        }
        // Select Scroll right
        else if (!gpio_get(GPIO22)) {
            printf("Right button pressed\n");
            if (scroll_state != 2) {
                scroll_state = 2;
                printf("New state: %d\n", scroll_state);
            } else { /* already in state */ }
        }
        else { 
            /* no input */ 
        }

        // If scroll state changed, update OLED driver
        if (scroll_state != scroll_state_prev) {
             if (scroll_state == 0) {
                conf_horz_scroll(0x01);
            }
            else if (scroll_state == 1) {
                oled_send_cmd(OLED_SET_SCROLL | 0x00);
            }
            else if (scroll_state == 2) {
                conf_horz_scroll(0x00);
            }
            else { } // not applicable 

            // Track last state
            scroll_state_prev = scroll_state;
        }

        // Rest
        sleep_ms(1000); 
    }

#endif
    return 0;
}
