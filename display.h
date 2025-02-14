#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

const uint WIDTH = 128;
const uint HEIGHT = 64;
const uint ADDRESS = 0x3C;
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
const uint BAUDRATE = 400 * 1000;
const char SQUARE = 0xff;

typedef enum {
    SET_CONTRAST = 0x81,
    SET_ENTIRE_ON = 0xA4,
    SET_NORM_INV = 0xA6,
    SET_DISP = 0xAE,
    SET_MEM_ADDR = 0x20,
    SET_COL_ADDR = 0x21,
    SET_PAGE_ADDR = 0x22,
    SET_DISP_START_LINE = 0x40,
    SET_SEG_REMAP = 0xA0,
    SET_MUX_RATIO = 0xA8,
    SET_COM_OUT_DIR = 0xC0,
    SET_DISP_OFFSET = 0xD3,
    SET_COM_PIN_CFG = 0xDA,
    SET_DISP_CLK_DIV = 0xD5,
    SET_PRECHARGE = 0xD9,
    SET_VCOM_DESEL = 0xDB,
    SET_CHARGE_PUMP = 0x8D
} display_command_t;

struct display_t {
    uint8_t width, height, pages, address;
    i2c_inst_t *i2c_port;
    bool external_vcc;
    uint8_t *ram_buffer;
    size_t bufsize;
    uint8_t port_buffer[2];
} display;

static void display_init_ports() {
    i2c_init(i2c1, BAUDRATE);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

static void display_command(uint8_t command) {
    display.port_buffer[1] = command;
    i2c_write_blocking(display.i2c_port, display.address, display.port_buffer, 2, false);
}

static void display_config() {
    display_command(SET_DISP | 0x00);
    display_command(SET_MEM_ADDR);
    display_command(0x01);
    display_command(SET_DISP_START_LINE | 0x00);
    display_command(SET_SEG_REMAP | 0x01);
    display_command(SET_MUX_RATIO);
    display_command(HEIGHT - 1);
    display_command(SET_COM_OUT_DIR | 0x08);
    display_command(SET_DISP_OFFSET);
    display_command(0x00);
    display_command(SET_COM_PIN_CFG);
    display_command(0x12);
    display_command(SET_DISP_CLK_DIV);
    display_command(0x80);
    display_command(SET_PRECHARGE);
    display_command(0xF1);
    display_command(SET_VCOM_DESEL);
    display_command(0x30);
    display_command(SET_CONTRAST);
    display_command(0xFF);
    display_command(SET_ENTIRE_ON);
    display_command(SET_NORM_INV);
    display_command(SET_CHARGE_PUMP);
    display_command(0x14);
    display_command(SET_DISP | 0x01);
}

static void display_setup() {
    display.width = WIDTH;
    display.height = HEIGHT;
    display.pages = HEIGHT / 8U;
    display.address = ADDRESS;
    display.i2c_port = i2c1;
    display.bufsize = display.pages * display.width + 1;
    display.ram_buffer = calloc(display.bufsize, sizeof(uint8_t));
    display.ram_buffer[0] = 0x40;
    display.port_buffer[0] = 0x80;
}

static void display_pixel(uint8_t x, uint8_t y, bool value) {
  uint16_t index = (y >> 3) + (x << 3) + 1;
  uint8_t pixel = (y & 0b111);
  if (value) display.ram_buffer[index] |= (1 << pixel);
  else display.ram_buffer[index] &= ~(1 << pixel);
}

static void display_clean(bool color) {
    for (uint8_t y = 0; y < display.height; ++y) {
        for (uint8_t x = 0; x < display.width; ++x) {
            display_pixel(x, y, color);
        }
    }
}

static void display_flush() {
    display_command(SET_COL_ADDR);
    display_command(0);
    display_command(display.width - 1);
    display_command(SET_PAGE_ADDR);
    display_command(0);
    display_command(display.pages - 1);
    i2c_write_blocking(display.i2c_port, display.address, display.ram_buffer, display.bufsize, false);
}

void display_init() {
    display_init_ports();
    display_setup();
    display_config();
    display_clean(false);
    display_flush();
}

static void display_rect(uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool thin) {
    display_clean(!thin);
    for (uint8_t x = left; x < left + width; ++x) {
        display_pixel(x, top, thin);
        display_pixel(x, top + height - 1, thin);
    }
    for (uint8_t y = top; y < top + height; ++y) {
        display_pixel(left, y, thin);
        display_pixel(left + width - 1, y, thin);
    }
    if (!thin) {
        for (uint8_t x = left + 1; x < left + width - 1; ++x) {
            for (uint8_t y = top + 1; y < top + height - 1; ++y) {
                display_pixel(x, y, thin);
            }
        }
    }
}

static void display_draw_filled_square(uint8_t x, uint8_t y) {
    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            display_pixel(x + j, y + i, SQUARE & (1 << j));
        }
    }
}

void display_draw_filled_square_and_rect(float x_relative_position, float y_relative_position, bool filled) {
    display_rect(3, 3, 122, 58, filled);
    uint x = 60 + (x_relative_position * 57);
    uint y = 28 + (-y_relative_position * 25);
    display_draw_filled_square(x, y);
    display_flush();
}