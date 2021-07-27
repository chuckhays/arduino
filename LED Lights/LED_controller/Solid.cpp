#include "Solid.h"

#include "Strip.h"

#include <Adafruit_ILI9341.h>

Solid::Solid(Adafruit_ILI9341 *t, uint8_t r, uint8_t g, uint8_t b, uint8_t w) : Mode(t)
{
  red = r;
  green = g;
  blue = b;
  white = w;
  if (r != 0 || g != 0 || b != 0)
  {
    uint16_t tempWhite = w / 2;
    uint16_t tempRed = r + tempWhite;
    uint16_t tempGreen = g + tempWhite;
    uint16_t tempBlue = b + tempWhite;
    fill = t->color565(coerceValue(tempRed), coerceValue(tempGreen), coerceValue(tempBlue));
  }
  else
  {
    fill = t->color565(w, w, w);
  }
}

void Solid::load()
{
}

void Solid::processInput(int x, int y, int sw)
{
}

void Solid::drawRect(int x, int y, int w, int h)
{
  tft->fillRect(x, y, w, h, fill);
}

void Solid::draw()
{
}

void Solid::updateLEDs()
{
  for (uint16_t n = 0; n < numPixels(); ++n)
  {
    setPixelColor(n, red, green, blue, white);
  }
  show();
}
