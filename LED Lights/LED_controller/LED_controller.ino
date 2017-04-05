#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "Strip.h"
#include "Bars.h"
#include "Cycle.h"

#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5

#define YPIN A3
#define XPIN A2
#define SWITCH A1

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define ENABLE_PIN 11

#define MODES 2
Mode* modes[MODES];
int currentMode = 0;

int bar = 0;
int red = 0;
int green = 0;
int blue = 0;
int white = 255;

void setup() {
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH);
  setupStrip();
  for(uint16_t n=0; n < numPixels(); ++n) {
    setPixelColor(n, 0, 0, 0, 255);
  }
  show();
  
  Cycle *c = new Cycle(&tft);
  modes[0] = c;
  
  Bars *b = new Bars(&tft);
  modes[1] = b;

  Serial.begin(115200);

  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(ILI9341_BLACK);
  modes[currentMode]->load();
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
  if (s == 0) {
    currentMode = (currentMode + 1) % MODES;
    mode = modes[currentMode];
    mode->load();
  }
}

