#ifndef _MODE_H
#define _MODE_H

#include <Adafruit_ILI9341.h>

class Mode {
  public:
    Mode(Adafruit_ILI9341* t);
    virtual void load() = 0;
    virtual void processInput(int x, int y, int sw) = 0;
    virtual void draw() = 0;
    virtual void updateLEDs() = 0;
    int coerceValue(int value);
    uint8_t scaleColor(uint8_t c, uint8_t brightness);
  protected:
    Adafruit_ILI9341* tft;
};

#endif // _MODE_H
