#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define LED_PIN     2
#define LED_COUNT   8
#define IS_RGBW     false
#define BRIGHTNESS  80  // adjust brightness (0-255)

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(g) << 24) |
           ((uint32_t)(r) << 16) |
           ((uint32_t)(b) << 8);
}

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb >> 8);
}

static inline uint32_t scale_brightness(uint32_t color) {
    uint8_t g = (color >> 24) & 0xFF;
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t b = (color >> 8) & 0xFF;

    r = (r * BRIGHTNESS) / 255;
    g = (g * BRIGHTNESS) / 255;
    b = (b * BRIGHTNESS) / 255;

    return urgb_u32(r, g, b);
}

int main() {
    stdio_init_all();

    // Init WS2812 PIO
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, LED_PIN, 800000, IS_RGBW);

    // Pick LED color (white)
    uint32_t color = scale_brightness(urgb_u32(255, 255, 255));

    // Turn ON first 8 LEDs
    for (int i = 0; i < LED_COUNT; i++) {
        put_pixel(color);
    }

    // Keep running forever (do nothing)
    while (true) {
        sleep_ms(1000);
    }
}

