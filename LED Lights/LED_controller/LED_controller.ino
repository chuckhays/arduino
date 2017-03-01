#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>

#include "Bars.h"

#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5

#define YPIN A3
#define XPIN A2
#define SWITCH A1

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(444 /*292 + 152*/, PIN, NEO_GRBW + NEO_KHZ800);
#define MODES 1
Mode* modes[MODES];
int currentMode = 0;

int bar = 0;
int red = 0;
int green = 0;
int blue = 0;
int white = 255;

void setup() {
  Bars *b = new Bars(&tft, &strip);
  modes[0] = b;
  Serial.begin(115200);
  strip.setBrightness(255);
  strip.begin();
  strip.show();
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
 
//  fadeIn(red, green, blue, white, 255);
}

void loop(void) {
  Mode *mode = modes[currentMode];

  int s = digitalRead(SWITCH);
  // Take 3 readings and average.
  int x1 = analogRead(XPIN);
  int y1 = analogRead(YPIN);
  delay(10);
  int x2 = analogRead(XPIN);
  int y2 = analogRead(YPIN);
  delay(10);
  int x3 = analogRead(XPIN);
  int y3 = analogRead(YPIN);
  int x = (x1 + x2 + x3) / 3;
  int y = (y1 + y2 + y3) / 3;
  mode->processInput(x, y, s);
  mode->draw();
  mode->updateLEDs();
}

