#include "Strip.h"

#include <Adafruit_NeoPixel.h>

#define NONE 1000
#define NUM_PIXELS 137
#define PHYSICAL_NUM_PIXELS 276
#define PIN 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PHYSICAL_NUM_PIXELS, PIN, NEO_GRBW + NEO_KHZ800);

uint16_t numPixels()
{
  return NUM_PIXELS;
}

void show()
{
  strip.show();
}

uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  return strip.Color(r, g, b, w);
}

void setupStrip()
{
  strip.setBrightness(255);
  strip.begin();
}

void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  uint16_t p1 = NONE;
  uint16_t p2 = NONE;
  if (n < 47)
  {
    p1 = 182 + n;
    p2 = 275 - n;
  }
  else if (n < 95)
  {
    p1 = 86 + n;
    p2 = 179 - n;
  }
  else if (n < 137)
  {
    p1 = n - 95;
    p2 = 178 - n;
  }
  if (p1 < PHYSICAL_NUM_PIXELS)
  {
    strip.setPixelColor(p1, r, g, b, w);
  }
  if (p2 < PHYSICAL_NUM_PIXELS)
  {
    strip.setPixelColor(p2, r, g, b, w);
  }
}

void setPixelColor(uint16_t n, uint32_t c)
{
  uint8_t r = (uint8_t)(c >> 16);
  uint8_t g = (uint8_t)(c >> 8);
  uint8_t b = (uint8_t)c;
  uint8_t w = (uint8_t)(c >> 24);
  setPixelColor(n, r, g, b, w);
}
