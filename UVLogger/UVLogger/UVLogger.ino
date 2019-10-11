#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include "Adafruit_VEML6075.h"

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
Adafruit_VEML6075 uv = Adafruit_VEML6075();

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

void setup() {
  uv.begin();
   uv.setIntegrationTime(VEML6075_100MS);
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);

  lcd.setCursor(0,0);
  lcd.print("A:");
  lcd.setCursor(8,0);
  lcd.print("B:");
  lcd.setCursor(0,1);
  lcd.print("I:");
}

void loop() {
  float uva = max(0, uv.readUVA());
  float uvb = max(0, uv.readUVB());
  float uvi = max(0, uv.readUVI());

  lcd.setCursor(0,0);
  lcd.print("A:      ");
  lcd.setCursor(2,0);
  lcd.print(uva, 0);

  lcd.setCursor(8,0);
  lcd.print("B:      ");
  lcd.setCursor(10,0);
  lcd.print(uvb, 0);

  lcd.setCursor(0,1);
  lcd.print("I:              ");
  lcd.setCursor(2,1);
  lcd.print(uvi, 1);
}
