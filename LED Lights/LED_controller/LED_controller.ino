#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>

#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    5

#define YPIN A3
#define XPIN A2
#define SWITCH A1

#define LINE1 0
#define LINE2 20
#define LINE3 40
#define TEXTX 25

#define BARWIDTH 30
#define BAROFFSET 50
#define REDBARX 100
#define GREENBARX (REDBARX + BAROFFSET)
#define BLUEBARX (GREENBARX + BAROFFSET)
#define WHITEBARX (BLUEBARX + BAROFFSET)
#define BAROUTLINE 3
#define BARMAXY 230
#define BARMINY 10

#define STARTLOWSIGNAL 412
#define STARTHIGHSIGNAL 612
#define MAXSIGNAL 924
#define MINSIGNAL 100
#define MAXSTEP 20

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(444 /*292 + 152*/, PIN, NEO_GRBW + NEO_KHZ800);

int bar = 0;
int red = 0;
int green = 0;
int blue = 0;
int white = 255;

void setup() {
  Serial.begin(115200);
  strip.setBrightness(255);
  strip.begin();
  strip.show();
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, LINE1);
  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(2);
  tft.println("X:");
  tft.setCursor(0, LINE2);
  tft.println("Y:");
  tft.setCursor(0, LINE3);
  tft.println("S:");
  drawRedOutline();
  drawGreenOutline();
  drawBlueOutline();
  drawWhiteOutline();
  fadeIn(red, green, blue, white, 255);
}

void loop(void) {
  processInput();
  updateBars();
  updateLEDs();
}

void processInput() {
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
  if (x < STARTLOWSIGNAL) {
    changeBar(-1);
  } else if (x > STARTHIGHSIGNAL) {
    changeBar(1);
  }
  if (y < MINSIGNAL) {
    changeBarValue(-MAXSTEP);
  } else if (y < STARTLOWSIGNAL) {
    changeBarValue(-(STARTLOWSIGNAL - y) * MAXSTEP / (STARTLOWSIGNAL - MINSIGNAL));
  } else if (y > MAXSIGNAL) {
    changeBarValue(MAXSTEP);
  } else if (y > STARTHIGHSIGNAL) {
    changeBarValue((y - STARTHIGHSIGNAL) * MAXSTEP / (MAXSIGNAL - STARTHIGHSIGNAL));
  }

  tft.fillRect(TEXTX, LINE1, 50, 60, ILI9341_BLACK);
  tft.setCursor(TEXTX, LINE1);
  tft.setTextColor(ILI9341_GREEN); tft.setTextSize(2);
  tft.println(x);
  tft.setCursor(TEXTX, LINE2);
  tft.println(y);
  tft.setCursor(TEXTX, LINE3);
  tft.println(s);
}

void changeBar(int diff) {
  bar = (bar + diff) % 4;
  if (bar < 0) {
    bar = 3;
  }
}

void changeBarValue(int diff) {
  switch (bar) {
    case 0:
      red = coerceValue(red + diff);
      break;
    case 1:
      green = coerceValue(green + diff);
      break;
    case 2:
      blue = coerceValue(blue + diff);
      break;
    case 3:
      white = coerceValue(white + diff);
      break;
  }
}

int coerceValue(int value) {
  if (value > 255) {
    return 255;
  }
  if (value < 0) {
    return 0;
  }
}

void updateBars() {
  drawRedBar(red);
  drawGreenBar(green);
  drawBlueBar(blue);
  drawWhiteBar(white);
  drawBarOutline(bar);
}

void drawBarOutline(int bar) {
  tft.drawRect(REDBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 0 ? ILI9341_RED : ILI9341_BLACK);
  tft.drawRect(GREENBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 1 ? ILI9341_GREEN : ILI9341_BLACK);
  tft.drawRect(BLUEBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 2 ? ILI9341_BLUE : ILI9341_BLACK);
  tft.drawRect(WHITEBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 3 ? ILI9341_WHITE : ILI9341_BLACK);
}

void drawRedBar(int fill) {
  drawBar(REDBARX, fill, ILI9341_RED);
}

void drawGreenBar(int fill) {
  drawBar(GREENBARX, fill, ILI9341_GREEN);
}

void drawBlueBar(int fill) {
  drawBar(BLUEBARX, fill, ILI9341_BLUE);
}

void drawWhiteBar(int fill) {
  drawBar(WHITEBARX, fill, ILI9341_WHITE);
}

void drawBar(int16_t x, int fill, uint16_t color) {
  int middleY = (BARMAXY - BAROUTLINE) - ((BARMAXY - BARMINY - BAROUTLINE - BAROUTLINE) * fill / 255.0);
  tft.fillRect(x + BAROUTLINE, BARMINY + BAROUTLINE, BARWIDTH, middleY - BARMINY - BAROUTLINE, ILI9341_BLACK);
  tft.fillRect(x + BAROUTLINE, middleY, BARWIDTH, BARMAXY - BAROUTLINE - middleY, color);
}

void drawRedOutline() {
  tft.drawRect(REDBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_RED);
}

void drawGreenOutline() {
  tft.drawRect(GREENBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_GREEN);
}

void drawBlueOutline() {
  tft.drawRect(BLUEBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_BLUE);
}

void drawWhiteOutline() {
  tft.drawRect(WHITEBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_WHITE);
}

uint8_t scaleColor(uint8_t c, uint8_t brightness) {
  return (c * brightness) >> 8;
}

void updateLEDs() {
  for(uint16_t n=0; n < strip.numPixels(); ++n) {
    strip.setPixelColor(n, red, green, blue, white);
  }
  strip.show();
}

void fadeIn(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t maxBrightness) {
  for(uint16_t i = 0; i < maxBrightness; i = i+5) {
    uint8_t newR = scaleColor(r, i);
    uint8_t newG = scaleColor(g, i);
    uint8_t newB = scaleColor(b, i);
    uint8_t newW = scaleColor(w, i);
    for(uint16_t n=0; n < strip.numPixels(); ++n) {
      strip.setPixelColor(n, newR, newG, newB, newW);
    }
    strip.show();
  }
  for(uint16_t n=0; n < strip.numPixels(); ++n) {
    strip.setPixelColor(n, r, g, b, w);
  }
  strip.show();
}
