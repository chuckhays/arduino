#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include "Strip.h"
#include "Bars.h"
#include "Cycle.h"
#include "Solid.h"
#include "Chase.h"
#include "MultiChase.h"

#define STMPE_CS 6
#define TFT_CS 9
#define TFT_DC 10
#define SD_CS 5

#define YPIN A3
#define XPIN A2
#define SWITCH A1

#define RECTLEFT 130
#define FIRSTTOP 30
#define MIDDLETOP 120
#define BOTTOMTOP 210
#define MOREBOTTOMTOP 300
#define MOREHEIGHT 20
#define RECTWIDTH 100
#define RECTHEIGHT 80
#define RECTBORDER 1
#define RECTPADDING 1

#define STARTLOWSIGNAL 412
#define STARTHIGHSIGNAL 612
#define MAXSIGNAL 924
#define MINSIGNAL 100

#define MODEDELAYTIME 500

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define ENABLE_PIN 11

#define MODES 12
Mode *modes[MODES];
int currentMode = 2;
unsigned long lastModeSwitchTime = 0;

void setup()
{
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH);
  setupStrip();
  for (uint16_t n = 0; n < numPixels(); ++n)
  {
    setPixelColor(n, 0, 0, 0, 255);
  }
  show();

  modes[0] = new Chase(&tft, 0, 0, 0, 255);
  Cycle *c = new Cycle(&tft);
  modes[1] = c;

  //Bars *b = new Bars(&tft);
  //modes[1] = b;
  modes[2] = new Solid(&tft, 0, 0, 0, 255);
  modes[3] = new Solid(&tft, 255, 0, 0, 0);
  modes[4] = new Solid(&tft, 0, 255, 0, 0);
  modes[5] = new Solid(&tft, 0, 0, 255, 0);
  modes[6] = new Solid(&tft, 0, 0, 0, 128);
  modes[7] = new Solid(&tft, 255, 0, 0, 255);
  modes[8] = new Solid(&tft, 0, 0, 255, 255);
  modes[9] = new Solid(&tft, 255, 0, 255, 0);
  modes[10] = new Chase(&tft, 255, 0, 0, 0);
  modes[11] = new MultiChase(&tft);

  Serial.begin(115200);

  tft.begin();
  tft.setRotation(2);
  tft.fillScreen(ILI9341_BLACK);
  modes[currentMode]->load();
}

void loop(void)
{
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

  if (y < MINSIGNAL)
  {
    changeMode(1);
  }
  else if (y < STARTLOWSIGNAL)
  {
  }
  else if (y > MAXSIGNAL)
  {
    changeMode(-1);
  }
  else if (y > STARTHIGHSIGNAL)
  {
  }
  else
  {
    lastModeSwitchTime = 0;
  }
  //mode->processInput(x, y, s);
  mode->updateLEDs();

  // Draw top more.
  if (currentMode > 1)
  {
    Mode *topMoreMode = modes[currentMode - 2];
    topMoreMode->drawRect(RECTLEFT, 0, RECTWIDTH, MOREHEIGHT);
    drawOutline(RECTLEFT, 0, RECTWIDTH, MOREHEIGHT);
  }
  else
  {
    clearRect(RECTLEFT, 0, RECTWIDTH, MOREHEIGHT);
  }

  // Draw first mode rect.
  if (currentMode > 0)
  {
    Mode *topMode = modes[currentMode - 1];
    topMode->drawRect(RECTLEFT, FIRSTTOP, RECTWIDTH, RECTHEIGHT);
    drawOutline(RECTLEFT, FIRSTTOP);
  }
  else
  {
    clearRect(RECTLEFT, FIRSTTOP);
  }
  // Draw selected mode rect.
  mode->drawRect(RECTLEFT, MIDDLETOP, RECTWIDTH, RECTHEIGHT);
  drawOutline(RECTLEFT, MIDDLETOP, ILI9341_WHITE);

  // Draw last mode rect.
  if (currentMode < MODES - 1)
  {
    Mode *bottomMode = modes[currentMode + 1];
    bottomMode->drawRect(RECTLEFT, BOTTOMTOP, RECTWIDTH, RECTHEIGHT);
    drawOutline(RECTLEFT, BOTTOMTOP);
  }
  else
  {
    clearRect(RECTLEFT, BOTTOMTOP);
  }

  // Draw bottom more.
  if (currentMode < MODES - 2)
  {
    Mode *bottomMoreMode = modes[currentMode + 2];
    bottomMoreMode->drawRect(RECTLEFT, MOREBOTTOMTOP, RECTWIDTH, MOREHEIGHT);
    drawOutline(RECTLEFT, MOREBOTTOMTOP, RECTWIDTH, MOREHEIGHT);
  }
  else
  {
    clearRect(RECTLEFT, MOREBOTTOMTOP, RECTWIDTH, MOREHEIGHT);
  }
}

void changeMode(int delta)
{
  if (millis() - lastModeSwitchTime < MODEDELAYTIME)
  {
    return;
  }
  lastModeSwitchTime = millis();
  if (delta > 0 && currentMode < MODES - 1)
  {
    currentMode++;
    Mode *mode = modes[currentMode];
    mode->load();
  }
  else if (delta < 0 && currentMode > 0)
  {
    currentMode--;
    Mode *mode = modes[currentMode];
    mode->load();
  }
}

void drawOutline(int x, int y) { drawOutline(x, y, RECTWIDTH, RECTHEIGHT, ILI9341_WHITE); }
void drawOutline(int x, int y, uint16_t color) { drawOutline(x, y, RECTWIDTH, RECTHEIGHT, color); }
void drawOutline(int x, int y, int w, int h) { drawOutline(x, y, w, h, ILI9341_WHITE); }
void drawOutline(int x, int y, int w, int h, uint16_t color)
{
  tft.drawRect(x - RECTBORDER - RECTPADDING,
               y - RECTBORDER - RECTPADDING,
               w + 2 * RECTBORDER + 2 * RECTPADDING,
               h + 2 * RECTBORDER + 2 * RECTPADDING, color);
}

void clearRect(int x, int y) { clearRect(x, y, RECTWIDTH, RECTHEIGHT); }
void clearRect(int x, int y, int w, int h)
{
  tft.fillRect(x - RECTBORDER - RECTPADDING,
               y - RECTBORDER - RECTPADDING,
               w + 2 * RECTBORDER + 2 * RECTPADDING,
               h + 2 * RECTBORDER + 2 * RECTPADDING, ILI9341_BLACK);
}
