#include "ws2812.pio.h"
#include "hardware/pio.h"
#include <Arduino.h>

#define IS_RGBW false

PIO pio = pio0;
int sm = 0;
int running = false;
int init_pin;

int neopixel_init(int pin)
{
  if (running) return 0;
  init_pin = pin;
  sm = pio_claim_unused_sm(pio, false);  // Find a free SM
  if (sm < 0) {
    pio = pio1;  // Try pio1 if SM not found
    sm = pio_claim_unused_sm(pio, false);
  }
  if (sm < 0) return -1;  // Return error if SM not found
  uint offset = pio_add_program(pio, &ws2812_program);
  ws2812_program_init(pio, sm, offset, init_pin, 800000, IS_RGBW);
  running = true;
  return 0;
}

void neopixel_set_pin(int pin)
{
  if (!running) return;
  pio_gpio_init(pio, pin);
  pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
  pio_sm_set_sideset_pins(pio, sm, pin);
}

static void put_pixel(uint32_t pixel_grb) {
  pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

void neopixel_set_pixels(uint32_t* buf, int size)
{
  if (!running) return;
  for (int i = 0; i < size; i++)
    put_pixel(buf[i]);
}

uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    uint32_t new_r = r;
    uint32_t new_g = g;
    uint32_t new_b = b;
    new_r = new_r * w / 255;
    new_g = new_g * w / 255;
    new_b = new_b * w / 255;
    return (new_r << 8) | (new_g << 16) | new_b;
}