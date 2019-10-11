#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <SparkFun_VEML6075_Arduino_Library.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
VEML6075 uv;

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

void setup() {
  uv.begin();
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
  float uva = uv.uva();
  float uvb = uv.uvb();
  float uvi = uv.index();
  lcd.setCursor(2,0);
  lcd.print(uva, 2);
  lcd.setCursor(10,0);
  lcd.print(uvb, 2);
  lcd.setCursor(2,1);
  lcd.print(uvi, 2);
}
