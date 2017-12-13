#ifndef _BARS_H
#define _BARS_H

#include "Mode.h"

#include <Adafruit_ILI9341.h>

class Bars : public Mode {
  public:
    Bars(Adafruit_ILI9341* t);
    void load();
    void processInput(int x, int y, int sw);
    void drawRect(int x, int y, int w, int h);
    void draw();
    void updateLEDs();

  private:
    void changeBar(int diff);
    void changeBarValue(int diff);
    void drawRedOutline();
    void drawGreenOutline();
    void drawBlueOutline();
    void drawWhiteOutline();
    void drawBarOutline(int bar);
    void drawRedBar(int fill);
    void drawGreenBar(int fill);
    void drawBlueBar(int fill);
    void drawWhiteBar(int fill);
    void drawBar(int16_t x, int fill, uint16_t color);

    int bar = 0;
    int red = 0;
    int green = 0;
    int blue = 0;
    int white = 255;
};

#endif // _BARS_H
