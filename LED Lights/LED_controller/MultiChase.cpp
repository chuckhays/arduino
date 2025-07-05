#include "MultiChase.h"
#include "Strip.h"

MultiChase::MultiChase(Adafruit_ILI9341 *tft, const uint8_t (*colors)[4], int colorCount, int width)
    : Mode(tft), step(0), chaseColors(colors), numChases(colorCount), width(width) {}

void MultiChase::load() {
    step = 0;
    for (uint16_t n = 0; n < numPixels(); ++n) {
        setPixelColor(n, 0, 0, 0, 255);
    }
    show();
}

void MultiChase::updateLEDs() {
    int total = numChases * width;
    for (uint16_t n = 0; n < numPixels(); ++n) {
        int idx = (n + step) % total;
        int colorIdx = idx / width;
        setPixelColor(n, chaseColors[colorIdx][0], chaseColors[colorIdx][1], chaseColors[colorIdx][2], chaseColors[colorIdx][3]);
    }
    show();
    step = (step + 1) % total;
}

void MultiChase::drawRect(int x, int y, int w, int h) {
    int stripeH = h / numChases;
    for (int c = 0; c < numChases; ++c) {
        tft->fillRect(x, y + c * stripeH, w, stripeH, tft->color565(chaseColors[c][0], chaseColors[c][1], chaseColors[c][2]));
    }
}
