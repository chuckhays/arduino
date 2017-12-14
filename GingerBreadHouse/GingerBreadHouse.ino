#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

#define NUM_LEDS 18

#define BRIGHTNESS 255

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define TIMEPER 10000

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

int gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

uint8_t reds[NUM_LEDS];
uint8_t blues[NUM_LEDS];
uint8_t greens[NUM_LEDS];
uint8_t whites[NUM_LEDS];
unsigned long times[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
}

void loop() {
  uint8_t buttons = lcd.readButtons();

  if (buttons) {
    lcd.clear();
    lcd.setCursor(0,0);
    if (buttons & BUTTON_UP) {
      lcd.print("UP ");
      lcd.setBacklight(RED);
    }
    if (buttons & BUTTON_DOWN) {
      lcd.print("DOWN ");
      lcd.setBacklight(YELLOW);
    }
    if (buttons & BUTTON_LEFT) {
      lcd.print("LEFT ");
      lcd.setBacklight(GREEN);
    }
    if (buttons & BUTTON_RIGHT) {
      lcd.print("RIGHT ");
      lcd.setBacklight(TEAL);
    }
    if (buttons & BUTTON_SELECT) {
      lcd.print("SELECT ");
      lcd.setBacklight(VIOLET);
    }
  }

  // Rainbow cycle.
  rainbowCycle(10);
  // Red/white/green wipe.
  redwhitegreenwipe();
  // Candy cane cycle.
  candyCycle(5);
  // Rainbow.
  rainbow(20);
  // Twinkle.
  twinkle();
  // Chase.
  theaterChase(strip.Color(0,0,0,255), 65);
  // Cylon.
  cylon();
}

// 12 13 14 15 16 17
// 0  1  2  3  4  5
// 11 10 9  8  7  6

void setPixel(uint8_t i, uint32_t c) {
  strip.setPixelColor(i, c);
  strip.setPixelColor(i+12, c);
  strip.setPixelColor(11-i, c);
}

void clearAll() {
  for(uint8_t j = 0; j < 6; ++j) {
    setPixel(j, strip.Color(0,0,0,0));
  }
}

void cylon() {
  unsigned long current = millis();
  unsigned long endTime = current + TIMEPER;
  while((current = millis()) < endTime) {
    for(uint8_t i = 0; i < 10; ++i) {
     uint8_t active = i;
     if (active >= 6) {
       active = 10 - active;
     }
     for(uint8_t j = 0; j < 6; ++j) {
       if (j == active) {
         setPixel(j, strip.Color(255,0,0,0));
       } else {
         setPixel(j, strip.Color(0,0,0,0));
       }
     }
     strip.show();
     delay(100);
    }
  }
}

#define DECAYTIME 200
#define MINTIME 250
#define MAXTIME 1000

void twinkle() {
  for(uint8_t i; i < NUM_LEDS; ++i) {
    reds[i] = 0;
    blues[i] = 0;
    greens[i] = 0;
    whites[i] = 0;
    times[i] = 0;
  }

  unsigned long current = millis();
  unsigned long endTime = current + TIMEPER;
  while ((current = millis()) < endTime) {
    if (random(100) == 1) {
      
      // Add a new color.
      uint8_t p = random(NUM_LEDS);
      if (random(5) == 1) {
        // White.
        reds[p] = blues[p] = greens[p] = 0;
        whites[p] = random(256);
      } else {
        // Color.
        whites[p] = 0;
        uint32_t c = Wheel(random(256));
        reds[p] = red(c);
        greens[p] = green(c);
        blues[p] = blue(c);
      }
      times[p] = random(MINTIME, MAXTIME) + current;
    }
    
    // Update all LEDS.
    for(uint8_t j = 0; j < NUM_LEDS; ++j) {
      long delta = times[j] - current;
      uint8_t r = reds[j];
      uint8_t g = greens[j];
      uint8_t b = blues[j];
      uint8_t w = whites[j];
      if (delta <= 0 ) {
        r = g = b = w = 0;
      } else if (delta < DECAYTIME) {
        // Scale by how close to 0.
        float scale = ((float)delta) / ((float)DECAYTIME);
        r = r * scale;
        g = g * scale;
        b = b * scale;
        w = w * scale;
      }
      strip.setPixelColor(j, strip.Color(r, g, b, w));
    }
    strip.show();
  }
}

void redwhitegreenwipe() {
  clearAll();
  uint8_t wait = 100;
  uint32_t red = strip.Color(255, 0, 0);
  uint32_t green = strip.Color(0, 255, 0);
  uint32_t white = strip.Color(0, 0, 0, 255);
  unsigned long current = millis();
  unsigned long endTime = current + TIMEPER;
  while ((current = millis()) < endTime) {
    // Wipe each set through each color.
    for(uint8_t i = 0; i < 6; ++i) {
      setPixel(i, red);
      strip.show();
      delay(wait);
    }
    for(uint8_t i = 0; i < 6; ++i) {
      setPixel(i, green);
      strip.show();
      delay(wait);
    }
    for(uint8_t i = 0; i < 6; ++i) {
      setPixel(i, white);
      strip.show();
      delay(wait);
    }
  }
}

void theaterChase(uint32_t c, uint8_t wait) {
  unsigned long current = millis();
  unsigned long endTime = current + TIMEPER;
  while ((current = millis()) < endTime) {
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < 6; i=i+3) {
        setPixel(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < 6; i=i+3) {
        setPixel(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< 6; i++) {
      setPixel(i, Wheel(((i * 256 / 6) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void candyCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< 6; i++) {
      setPixel(i, CandyWheel(((i * 256 / 6) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 2; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t CandyWheel(byte WheelPos) {
  // 0 is all red.
  // 64 is all white.
  // 127 is all red.
  // 191 is all white.
  // 255 is all red.

  if (WheelPos < 64) {
    return strip.Color(255 - (WheelPos*4), 0, 0, WheelPos*4);
  } else if (WheelPos < 128) {
    WheelPos -= 64;
    return strip.Color(WheelPos*4, 0, 0, 255 - (WheelPos*4));
  } else if (WheelPos < 192) {
    WheelPos -= 128;
    return strip.Color(255 - (WheelPos*4), 0, 0, WheelPos*4);
  } else {
    WheelPos -= 192;
    return strip.Color(WheelPos*4, 0, 0, 255 - (WheelPos*4));
  }
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
  return (c >> 8);
}
uint8_t green(uint32_t c) {
  return (c >> 16);
}
uint8_t blue(uint32_t c) {
  return (c);
}


