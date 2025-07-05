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
    static unsigned long lastUpdate = 0;
    static int offset = 0;
    unsigned long now = millis();
    if (now - lastUpdate < 250) {
        return;
    }
    lastUpdate = now;
    offset = (offset + 1) % w;
    int stripeW = w / numChases;
    for (int c = 0; c < numChases; ++c) {
        int x0 = x + ((c * stripeW - offset + w) % w);
        int actualW = (c == numChases - 1) ? (w - c * stripeW) : stripeW;
        
        uint16_t fill =  chaseColors[c][3] == 255 ? 
        tft->color565(255, 255, 255)
        : tft->color565(chaseColors[c][0], chaseColors[c][1], chaseColors[c][2]);
        if (x0 + actualW <= x + w) {
            tft->fillRect(x0, y, actualW, h, fill);
        } else {
            // Draw wrapped part
            int part1 = (x + w) - x0;
            tft->fillRect(x0, y, part1, h, fill);
            int part2 = actualW - part1;
            if (part2 > 0) {
                tft->fillRect(x, y, part2, h, fill);
            }
        }
    }
}

// Add empty implementations for draw and processInput
void MultiChase::draw() {}

void MultiChase::processInput(int x, int y, int s) {}
