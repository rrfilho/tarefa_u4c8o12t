#include "pico/stdlib.h"
#include "hardware/pwm.h"

static const uint GREEN_PWM_LED = 11;
static const uint BLUE_PWM_LED = 12;
static const uint RED_PWM_LED = 13;
static const float CLOCK_DIVIDER = 2.0;
static const uint WRAP = 2048;
static const float MIN_BRIGHTNESS = 0.0f;
static const float MAX_BRIGHTNESS = 100.0f;

static void led_init_pin(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice, CLOCK_DIVIDER);
    pwm_set_wrap(slice, WRAP);
    pwm_set_gpio_level(pin, MIN_BRIGHTNESS);
    pwm_set_enabled(slice, true);
}

void leds_init() {
    led_init_pin(GREEN_PWM_LED);
    led_init_pin(BLUE_PWM_LED);
    led_init_pin(RED_PWM_LED);
}

static void led_set_level(uint led_pin, float brightness) {
    float _brightness = brightness >= 0 ? brightness : -1 * brightness;
    if (_brightness > MAX_BRIGHTNESS) brightness = MAX_BRIGHTNESS;
    if (_brightness < MIN_BRIGHTNESS) brightness = MIN_BRIGHTNESS;
    uint level = WRAP * _brightness;
    pwm_set_gpio_level(led_pin, level);
}

void led_set_green_brightness(float brightness) {
    led_set_level(GREEN_PWM_LED, brightness);
}

void led_set_blue_brightness(float brightness) {
    led_set_level(BLUE_PWM_LED, brightness);
}

void led_set_red_brightness(float brightness) {
    led_set_level(RED_PWM_LED, brightness);
}