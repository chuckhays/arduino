#include "Cycle.h"

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

Cycle::Cycle(Adafruit_ILI9341* t) : Mode(t) {}

void Cycle::load() {
  tft->fillScreen(ILI9341_BLACK);
  tft->setCursor(0, LINE1);
  tft->setTextColor(ILI9341_GREEN);
  tft->setTextSize(2);
  tft->println("X:");
  tft->setCursor(0, LINE2);
  tft->println("Y:");
  tft->setCursor(0, LINE3);
  tft->println("C:");
  drawRedOutline();
  drawGreenOutline();
  drawBlueOutline();
  drawWhiteOutline();
  
}

void Cycle::processInput(int x, int y, int sw) {
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
  tft->println(c);
}

void Cycle::changeBar(int diff) {
  bar = (bar + diff) % 4;
  if (bar < 0) {
    bar = 3;
  }
}

void Cycle::changeBarValue(int diff) {
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

void Cycle::draw() {
  drawRedBar(red);
  drawGreenBar(green);
  drawBlueBar(blue);
  drawWhiteBar(white);
  drawBarOutline(bar);
}

void Cycle::updateLEDs() {
if (false) {
  c = (c + 1) % numPixels();
  //Serial.print("C:"); Serial.println(c);
  for(uint16_t n=0; n < numPixels(); ++n) {
    if (n == c) {
      setPixelColor(n, 0, 0, 0, 255);
    } else {
      setPixelColor(n, 0, 0, 0, 0);
    }
  }
} else {
  c = (c + 1) % 256;
  //for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(uint16_t i=0; i< numPixels(); i++) {
      setPixelColor(i, Wheel(((i * 256 / numPixels()) + c) & 255));
    }
    //delay(5);
  //}
}
  show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Cycle::Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

void Cycle::drawBarOutline(int bar) {
  tft->drawRect(REDBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 0 ? ILI9341_RED : ILI9341_BLACK);
  tft->drawRect(GREENBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 1 ? ILI9341_GREEN : ILI9341_BLACK);
  tft->drawRect(BLUEBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 2 ? ILI9341_BLUE : ILI9341_BLACK);
  tft->drawRect(WHITEBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 3 ? ILI9341_WHITE : ILI9341_BLACK);
}

void Cycle::drawRedBar(int fill) {
  drawBar(REDBARX, fill, ILI9341_PINK);
}

void Cycle::drawGreenBar(int fill) {
  drawBar(GREENBARX, fill, ILI9341_GREEN);
}

void Cycle::drawBlueBar(int fill) {
  drawBar(BLUEBARX, fill, ILI9341_BLUE);
}

void Cycle::drawWhiteBar(int fill) {
  drawBar(WHITEBARX, fill, ILI9341_WHITE);
}

void Cycle::drawBar(int16_t x, int fill, uint16_t color) {
  int middleY = (BARMAXY - BAROUTLINE) - ((BARMAXY - BARMINY - BAROUTLINE - BAROUTLINE) * fill / 255.0);
  tft->fillRect(x + BAROUTLINE, BARMINY + BAROUTLINE, BARWIDTH, middleY - BARMINY - BAROUTLINE, ILI9341_BLACK);
  tft->fillRect(x + BAROUTLINE, middleY, BARWIDTH, BARMAXY - BAROUTLINE - middleY, color);
}

void Cycle::drawRedOutline() {
  tft->drawRect(REDBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_RED);
}

void Cycle::drawGreenOutline() {
  tft->drawRect(GREENBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_GREEN);
}

void Cycle::drawBlueOutline() {
  tft->drawRect(BLUEBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_BLUE);
}

void Cycle::drawWhiteOutline() {
  tft->drawRect(WHITEBARX, BARMINY, BARWIDTH + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY, ILI9341_WHITE);
}

