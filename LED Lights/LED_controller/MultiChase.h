#ifndef MULTICHASE_H
#define MULTICHASE_H

#include "Mode.h"
#include <Adafruit_ILI9341.h>

class MultiChase : public Mode {
public:
    MultiChase(Adafruit_ILI9341 *tft, const uint8_t (*colors)[4], int colorCount, int width);
    void load() override;
    void updateLEDs() override;
    void drawRect(int x, int y, int w, int h) override;
    void draw() override;
    void processInput(int x, int y, int s) override;
private:
    int step;
    const uint8_t (*chaseColors)[4];
    int numChases;
    int width;
    unsigned long lastUpdate;
    int offset;
};

#endif // MULTICHASE_H
