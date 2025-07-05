#include "Chase.h"

#include "Strip.h"

#include <Adafruit_ILI9341.h>

#define LINE1 3
#define LINE2 23
#define LINE3 43
#define TEXTX 25

// #define BARWIDTH 25
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

#define BARWIDTH 3
#define SKIP 5

Chase::Chase(Adafruit_ILI9341 *t, uint8_t r, uint8_t g, uint8_t b, uint8_t w) : Mode(t)
{
  red = r;
  green = g;
  blue = b;
  white = w;
  if (r != 0 || g != 0 || b != 0)
  {
    uint16_t tempWhite = w / 2;
    uint16_t tempRed = r + tempWhite;
    uint16_t tempGreen = g + tempWhite;
    uint16_t tempBlue = b + tempWhite;
    fill = t->color565(coerceValue(tempRed), coerceValue(tempGreen), coerceValue(tempBlue));
  }
  else
  {
    fill = t->color565(w, w, w);
  }
}

void Chase::load()
{
}

void Chase::processInput(int x, int y, int sw)
{
}

void Chase::draw() {}

void Chase::drawRect(int x, int y, int w, int h)
{
  unsigned long m = millis();
  unsigned long elapsed = m - lastTime;
  //Serial.print("m:");
  //Serial.print(m);
  //Serial.print("e:");
  //Serial.print(elapsed);
  //Serial.print("l:");
  //Serial.println(lastTime);
  if (elapsed > 250)
  {
    lastTime = m;

    c = (c + 1) % SKIP;
  }
  Serial.println(c);

  //tft->fillRect(x, y, w, h, ILI9341_BLACK);

  for (uint16_t i = 0; i < w - (BARWIDTH - 1); i = i + BARWIDTH)
  {
    if ((i / BARWIDTH) % SKIP == c)
    {
      tft->fillRect(x + i, y, BARWIDTH, h, fill);
    }
    else
    {
      tft->fillRect(x + i, y, BARWIDTH, h, ILI9341_BLACK);
    }
  }
}

void Chase::updateLEDs()
{
  for (uint16_t i = 0; i < numPixels(); i++)
  {
    if (i % SKIP == c)
    {
      // Set to color.
      setPixelColor(i, red, green, blue, white);
    }
    else
    {
      // Set to off.
      setPixelColor(i, 0, 0, 0, 0);
    }
  }
  show();
}
