#ifndef MULTICHASE_H
#define MULTICHASE_H

#include "Mode.h"
#include <Adafruit_ILI9341.h>

class MultiChase : public Mode {
public:
    MultiChase(Adafruit_ILI9341 *tft);
    void load() override;
    void updateLEDs() override;
    void drawRect(int x, int y, int w, int h) override;
private:
    int step;
};

#endif // MULTICHASE_H
