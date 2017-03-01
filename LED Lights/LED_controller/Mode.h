#ifndef _MODE_H
#define _MODE_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>

class Mode {
  public:
    Mode(Adafruit_ILI9341* t, Adafruit_NeoPixel* s);
    virtual void load() = 0;
    virtual void processInput(int x, int y, int sw) = 0;
    virtual void draw() = 0;
    virtual void updateLEDs() = 0;
    int coerceValue(int value);
    uint8_t scaleColor(uint8_t c, uint8_t brightness);
  protected:
    Adafruit_ILI9341* tft;
    Adafruit_NeoPixel* strip;
};

Mode::Mode(Adafruit_ILI9341* t, Adafruit_NeoPixel* s) {
  tft = t;
  strip = s;
}

int Mode::coerceValue(int value) {
  if (value > 255) {
    return 255;
  }
  if (value < 0) {
    return 0;
  }
  return value;
}

uint8_t Mode::scaleColor(uint8_t c, uint8_t brightness) {
  return (c * brightness) >> 8;
}

#endif // _MODE_H
