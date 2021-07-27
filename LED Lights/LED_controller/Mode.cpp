#include "Mode.h"

#include <Adafruit_ILI9341.h>

#include "Strip.h"

Mode::Mode(Adafruit_ILI9341 *t)
{
  tft = t;
}

int Mode::coerceValue(int value)
{
  if (value > 255)
  {
    return 255;
  }
  if (value < 0)
  {
    return 0;
  }
  return value;
}

uint8_t Mode::scaleColor(uint8_t c, uint8_t brightness)
{
  return (c * brightness) >> 8;
}
