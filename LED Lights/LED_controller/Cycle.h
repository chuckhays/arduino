#ifndef _CYCLE_H
#define _CYCLE_H

#include "Mode.h"
#include "Strip.h"

#include <Adafruit_GFX.h>
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

class Cycle : public Mode {
  public:
    Cycle(Adafruit_ILI9341* t, Strip* s);
    void load();
    void processInput(int x, int y, int sw);
    void draw();
    void updateLEDs();

  private:
    void changeBar(int diff);
    void changeBarValue(int diff);
    void drawRedOutline();
    void drawGreenOutline();
    void drawBlueOutline();
    void drawWhiteOutline();
    void drawBarOutline(int bar);
    void drawRedBar(int fill);
    void drawGreenBar(int fill);
    void drawBlueBar(int fill);
    void drawWhiteBar(int fill);
    void drawBar(int16_t x, int fill, uint16_t color);

    uint32_t Wheel(byte WheelPos);

    int bar = 0;
    int red = 0;
    int green = 0;
    int blue = 0;
    int white = 255;
};

Cycle::Cycle(Adafruit_ILI9341* t, Strip* s) : Mode(t, s) {
  
}

void Cycle::load() {
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
  
}

void Cycle::processInput(int x, int y, int sw) {
  if (x < STARTLOWSIGNAL) {
    changeBar(-1);
  } else if (x > STARTHIGHSIGNAL) {
    changeBar(1);
  }
  if (y < MINSIGNAL) {
    changeBarValue(-MAXSTEP);
    x=0;
  } else if (y < STARTLOWSIGNAL) {
    changeBarValue(-(STARTLOWSIGNAL - y) * MAXSTEP / (STARTLOWSIGNAL - MINSIGNAL));
    x=1;
  } else if (y > MAXSIGNAL) {
    changeBarValue(MAXSTEP);
    x=2;
  } else if (y > STARTHIGHSIGNAL) {
    changeBarValue((y - STARTHIGHSIGNAL) * MAXSTEP / (MAXSIGNAL - STARTHIGHSIGNAL));
    x=3;
  } else {
    x = 4;
  }
  x=red;

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

void Cycle::changeBar(int diff) {
  bar = (bar + diff) % 4;
  if (bar < 0) {
    bar = 3;
  }
}

void Cycle::changeBarValue(int diff) {
  switch (bar) {
    case 0:
    Serial.print("diff:");
    Serial.print(diff);
    Serial.print(" red:");
    Serial.print(red);
      red = coerceValue(red + diff);
      Serial.print(" new red:");
      Serial.println(red);
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
 uint16_t i, j;
  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip->numPixels(); i++) {
      strip->setPixelColor(i, Wheel(((i * 256 / strip->numPixels()) + j) & 255));
    }
    strip->show();
    delay(5);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Cycle::Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

void Cycle::drawBarOutline(int bar) {
  tft->drawRect(REDBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 0 ? ILI9341_RED : ILI9341_BLACK);
  tft->drawRect(GREENBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 1 ? ILI9341_GREEN : ILI9341_BLACK);
  tft->drawRect(BLUEBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 2 ? ILI9341_BLUE : ILI9341_BLACK);
  tft->drawRect(WHITEBARX - BAROUTLINE, BARMINY - BAROUTLINE, BARWIDTH + BAROUTLINE + BAROUTLINE + BAROUTLINE + BAROUTLINE, BARMAXY - BARMINY + BAROUTLINE + BAROUTLINE, bar == 3 ? ILI9341_WHITE : ILI9341_BLACK);
}

void Cycle::drawRedBar(int fill) {
  drawBar(REDBARX, fill, ILI9341_BLUE);
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

#endif // _CYCLE_H
