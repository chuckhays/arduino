#include "MultiChase.h"
#include "Strip.h"

// Define colors for the chases
const uint8_t chaseColors[][4] = {
    {255, 0, 0, 0},    // Red
    {0, 255, 0, 0},    // Green
    {0, 0, 255, 0},    // Blue
    {255, 255, 0, 0},  // Yellow
    {0, 255, 255, 0},  // Cyan
    {255, 0, 255, 0}   // Magenta
};
const int numChases = sizeof(chaseColors) / sizeof(chaseColors[0]);
const int chaseSpacing = 8;

MultiChase::MultiChase(Adafruit_ILI9341 *tft) : Mode(tft), step(0) {}

void MultiChase::load() {
    step = 0;
    for (uint16_t n = 0; n < numPixels(); ++n) {
        setPixelColor(n, 0, 0, 0, 255);
    }
    show();
}

void MultiChase::updateLEDs() {
    for (uint16_t n = 0; n < numPixels(); ++n) {
        setPixelColor(n, 0, 0, 0, 255); // Clear
    }
    for (int c = 0; c < numChases; ++c) {
        int pos = (step + c * chaseSpacing) % numPixels();
        setPixelColor(pos, chaseColors[c][0], chaseColors[c][1], chaseColors[c][2], chaseColors[c][3]);
    }
    show();
    step = (step + 1) % numPixels();
    delay(40);
}

void MultiChase::drawRect(int x, int y, int w, int h) {
    // Draw a simple representation: colored stripes
    int stripeH = h / numChases;
    for (int c = 0; c < numChases; ++c) {
        tft->fillRect(x, y + c * stripeH, w, stripeH, tft->color565(chaseColors[c][0], chaseColors[c][1], chaseColors[c][2]));
    }
}
