#ifndef _SOLID_H
#define _SOLID_H

#include "Mode.h"

#include <Adafruit_ILI9341.h>

class Solid : public Mode {
  public:
    Solid(Adafruit_ILI9341* t, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    void load();
    void processInput(int x, int y, int sw);
    void drawRect(int x, int y, int w, int h);
    void draw();
    void updateLEDs();

  private:
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t white = 255;
    uint16_t fill = 0;
};

#endif // _SOLID_H
