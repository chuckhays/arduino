#include "Bars.h"

#include "Strip.h"

#include <Adafruit_ILI9341.h>

#define LINE1 3
#define LINE2 23
#define LINE3 43
#define TEXTX 25

#define BARWIDTH 25
#define BAROFFSET 40
#define REDBARX 80
#define GREENBARX (REDBARX + BAROFFSET)
#define BLUEBARX (GREENBARX + BAROFFSET)
#define WHITEBARX (BLUEBARX + BAROFFSET)
#define BAROUTLINE 3
#define BARMAXY 310
#define BARMINY 10

#define STARTLOWSIGNAL 412
#define STARTHIGHSIGNAL 612
#define MAXSIGNAL 924
#define MINSIGNAL 100
#define MAXSTEP 20

Bars::Bars(Adafruit_ILI9341* t) : Mode(t) {}

void Bars::load() {
  /*
  tft->fillScreen(ILI9341_BLACK);
  tft->setCursor(0, LINE1);
  tft->setTextColor(ILI9341_GREEN);
  tft->setTextSize(2);
  tft->println("X:");
  tft->setCursor(0, LINE2);
  tft->println("Y:");
  tft->setCursor(0, LINE3);
  tft->println("S:");
  drawRedOutline();
  drawGreenOutline();
  drawBlueOutline();
  drawWhiteOutline();
  */
}

void Bars::processInput(int x, int y, int sw) {
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

  tft->fillRect(TEXTX, LINE1, 50, 60, ILI9341_BLACK);
  tft->setCursor(TEXTX, LINE1);
  tft->setTextColor(ILI9341_GREEN); 
  tft->setTextSize(2);
  tft->println(x);
  tft->setCursor(TEXTX, LINE2);
  tft->println(y);
  tft->setCursor(TEXTX, LINE3);
  tft->println(sw);
}

void Bars::changeBar(int diff) {
  bar = (bar + diff) % 4;
  if (bar < 0) {
    bar = 3;
  }
}

void Bars::changeBarValue(int diff) {
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


void Bars::drawRect(int x, int y, int w, int h) {
  tft->fillRect(x, y, w, h, ILI9341_RED);
}

void Bars::draw() {
  drawRedBar(red);
  drawGreenBar(green);
  drawBlueBar(blue);
  drawWhiteBar(white);
  drawBarOutline(bar);
}

void Bars::updateLEDs() {
  for(uint16_t n=0; n < numPixels(); ++n) {
    setPixelColor(n, red, green, blue, white);
  }
  show();
}

void Bars::drawBarOutline(int bar) {
  tft->drawRect(REDBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 0 ? ILI9341_RED : ILI9341_BLACK);
  tft->drawRect(GREENBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 1 ? ILI9341_GREEN : ILI9341_BLACK);
  tft->drawRect(BLUEBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 2 ? ILI9341_BLUE : ILI9341_BLACK);
  tft->drawRect(WHITEBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 3 ? ILI9341_WHITE : ILI9341_BLACK);
}

void Bars::drawRedBar(int fill) {
  drawBar(REDBARX, fill, ILI9341_RED);
}

void Bars::drawGreenBar(int fill) {
  drawBar(GREENBARX, fill, ILI9341_GREEN);
}

void Bars::drawBlueBar(int fill) {
  drawBar(BLUEBARX, fill, ILI9341_BLUE);
}

void Bars::drawWhiteBar(int fill) {
  drawBar(WHITEBARX, fill, ILI9341_WHITE);
}

void Bars::drawBar(int16_t x, int fill, uint16_t color) {
  int middleY = (BARMAXY - BAROUTLINE) - ((BARMAXY - BARMINY - BAROUTLINE - BAROUTLINE) * fill / 255.0);
  tft->fillRect(x + BAROUTLINE, BARMINY + BAROUTLINE, BARWIDTH, middleY - BARMINY - BAROUTLINE, ILI9341_BLACK);
  tft->fillRect(x + BAROUTLINE, middleY, BARWIDTH, BARMAXY - BAROUTLINE - middleY, color);
}

void Bars::drawRedOutline() {
  tft->drawRect(REDBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_RED);
}

void Bars::drawGreenOutline() {
  tft->drawRect(GREENBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_GREEN);
}

void Bars::drawBlueOutline() {
  tft->drawRect(BLUEBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_BLUE);
}

void Bars::drawWhiteOutline() {
  tft->drawRect(WHITEBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_WHITE);
}

