#ifndef _STRIP_H
#define _STRIP_H

#include <Adafruit_NeoPixel.h>

#define NONE 1000
#define NUM_PIXELS 308
#define PHYSICAL_NUM_PIXELS 444
#define PIN 12

    //void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    //void setPixelColor(uint16_t n, uint32_t c);
    uint16_t numPixels(void) { return NUM_PIXELS; }
    

    Adafruit_NeoPixel strip = Adafruit_NeoPixel(PHYSICAL_NUM_PIXELS, PIN, NEO_GRBW + NEO_KHZ800);
void show() {
      strip.show();/*
      for(uint16_t i = 0; i < NUM_PIXELS; ++i) {
        uint32_t c = strip.getPixelColor(i);
        uint8_t 
        w = (uint8_t)(c >> 24),
      r = (uint8_t)(c >> 16),
      g = (uint8_t)(c >>  8),
      b = (uint8_t)c;
      Serial.print("n:");Serial.print(i);
      Serial.print("r:");Serial.print(r);
      Serial.print("g:");Serial.print(g);
      Serial.print("b:");Serial.print(b);
      Serial.print("w:");Serial.println(w);
      }*/
    }
    uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
      return strip.Color(r,g,b,w);
    }
  void setupStrip() {
    strip.setBrightness(255);
    strip.begin();
  }

void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  /*
A 48  403 - n n + 399 for 4<=n<=45
B 110 403 - n X
C 136 n - 110 406 - n for 114<=n<=136
D 159 n - 110 406 - n
E 210 n - 110 X
F 234 n - 110 457 - n
G 259 n - 110 457 - n for 235<=n<=258
H 308 n - 110 X
   */
   uint16_t upper = NONE;
   uint16_t lower = NONE;
   if (n < 48) {
     upper = 401 - n;
     if (3 <= n && n <= 44) {
       lower = 399 + n;
     }
   } else if (n < 110) {
     upper = 401 - n;
   } else if (n < 136) {
     upper = n - 110;
     if (113 <= n && n <= 135) {
       lower = 404 - n;
     }
   } else if (n < 159) {
     upper = n - 110;
     lower = 404 - n;
   } else if (n < 210) {
     upper = n - 110;
   } else if (n < 234) {
     upper = n - 110;
     lower = 455 - n;
   } else if (n < 259) {
     upper = n - 110;
     if (234 <= n && n <= 257) {
       lower = 455 - n;
     }
   } else if (n < 308) {
     upper = n - 110;
   }
   if (upper < 444) {
   //if (upper < 436 && PHYSICAL_NUM_PIXELS) {
    strip.setPixelColor(upper, r, g, b, w);
   }
   if (lower < 444) {
   //if (lower < 436 && PHYSICAL_NUM_PIXELS) {
    strip.setPixelColor(lower, r, g, b, w);
   }
}
/*
void setPixelColor(uint16_t n, uint32_t c) {
  uint8_t r = (uint8_t)(c >> 16);
  uint8_t g = (uint8_t)(c >>  8);
  uint8_t b = (uint8_t)c;
  uint8_t w = (uint8_t)(c >> 24);
  setPixelColor(n, r, g, b, w);
}
*/
#endif //_STRIP_H
