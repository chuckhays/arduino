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
    for (int c = 0; c < numChases; ++c) {
        int base = (step + c * 8) % numPixels();
        for (int w = 0; w < width; ++w) {
            int pos = (base + w) % numPixels();
            setPixelColor(pos, chaseColors[c][0], chaseColors[c][1], chaseColors[c][2], chaseColors[c][3]);
        }
    }
    show();
    step = (step + 1) % numPixels();
}

void MultiChase::drawRect(int x, int y, int w, int h) {
    int stripeH = h / numChases;
    for (int c = 0; c < numChases; ++c) {
        tft->fillRect(x, y + c * stripeH, w, stripeH, tft->color565(chaseColors[c][0], chaseColors[c][1], chaseColors[c][2]));
    }
}
