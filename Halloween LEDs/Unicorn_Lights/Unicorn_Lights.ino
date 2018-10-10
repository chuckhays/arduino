#include <Adafruit_NeoPixel.h>

#define PIN 6

#define NUM_LEDS 22

#define BRIGHTNESS 50

#define POTPIN A5
#define LEDPIN 5
#define SWITCHPIN 9

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  pinMode(LEDPIN, OUTPUT); // Set switch LED pin to output.
  pinMode(SWITCHPIN, INPUT_PULLUP); // Set switch pin to input.
  strip.setBrightness(BRIGHTNESS); // Set max brightness.
  strip.begin(); // Initialize the neopixel strip object.
  strip.show(); // Output current LED colors (all off currently).
}

void loop() {
  int switchVal = digitalRead(SWITCHPIN);
  int sensorValue = analogRead(POTPIN) / 4;
  analogWrite(LEDPIN, sensorValue);
  if (switchVal) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(sensorValue));
    }
  } else {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0,0,0,255));
    }
  }
  strip.show();
}

// Top
//   21           43
// 6  7 20     28 29 42
// 5  8 19     27 30 41
// 4  9 18     26 31 40
// 3 10 17     25 32 39
// 2 11 16     24 33 38
// 1 12 15     23 34 37
// 0 13 14     22 35 36

// Tower 0 : Row [0-6] = Index, 13 - Index, Index + 14
// Tower 1 : Row [0-6] = 22+ Index, 22 + 13 - Index, 22 + Index + 14
// Tower 0: Row 7 = Index + 14 (or just 21)
// Tower 0: Row 7 = 22 + Index + 14 (or just 43)

// 0::6 means iterate in a loop for each number 0 through 6
// Tower 0: Column [0-2] = Column * 7 + (0::6)
// Tower 1: Colun [0-2] = 22 + Column * 7 + (0::6)

// Set color for a logical row (0-7), will set all 3 LEDs (or single for top).
// Tower should be 0 or 1.
void setRowColor(uint8_t row, uint8_t tower, uint32_t color) {

}

// Set color for a logical row(0-7) on both towers at once.
void setRowColor(uint8_t row, uint32_t color) {

}

// Set color for a column with column (0-2), will set the entire column.
// Tower should be 0 or 1.
void setColumnColor(uint8_t column, uint8_t tower, uint32_t color) {

}

// Set color for a column with column (0-2) on both towers at once.
void setColumnColor(uint8_t column, uint32_t color) {

}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}
