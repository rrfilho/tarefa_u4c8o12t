#include <stdio.h>
#include "leds.h"
#include "buttons.h"
#include "joystick.h"
#include "display.h"

bool blue_led_status = false;
int leds_active = 1;

void on_button_press() {
    leds_active = !leds_active;
}

void on_joystick_button_press() {
    blue_led_status = !blue_led_status;
    led_set_blue_brightness(blue_led_status ? 1.0f : 0.0f);
}

void init() {
    stdio_init_all(); 
    leds_init();
    buttons_init(on_button_press, on_joystick_button_press);
    joystick_init();
    display_init();
}

int main() {
    init(); 
    while (true) {
        float x_relative_position = joystick_read_x_axis();
        led_set_green_brightness(x_relative_position * leds_active);
        float y_relative_position = joystick_read_y_axis();
        led_set_red_brightness(y_relative_position * leds_active);
        display_draw_filled_square_and_rect(x_relative_position, y_relative_position, blue_led_status);
        sleep_ms(100);
    }
}
