#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN     6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  30

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 255

#define AVE 3
#define MULT 2

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

unsigned long white = 0;
unsigned long red = 0;
unsigned long blue = 0;
unsigned long green = 0;

unsigned long whiteAverage = 0;
unsigned long redAverage = 0;
unsigned long blueAverage = 0;
unsigned long greenAverage = 0;

void setup() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
  white = pulseIn(A3, LOW, 20000);
  if (white == 0 && !digitalRead(A3)) {
    white = 8000;
  }
  red = pulseIn(A1, LOW, 20000);
  if (red == 0 && !digitalRead(A1)) {
    red = 8000;
  }
  blue = pulseIn(A2, LOW, 20000);
  if (blue == 0 && !digitalRead(A2)) {
    blue = 8000;
  }
  green = pulseIn(A0, LOW, 20000);
  if (green == 0 && !digitalRead(A0)) {
    green = 8000;
  }

  whiteAverage = (white + whiteAverage * MULT) / AVE;
  redAverage = (red + redAverage * MULT) / AVE;
  blueAverage = (blue + blueAverage * MULT) / AVE;
  greenAverage = (green + greenAverage * MULT) / AVE;

  // Calculate what colors to show.
  int w = 0;
  int r = 0;
  int g = 0;
  int b = 0;

  if (whiteAverage > 7500) {
    w = 255;
  } else {
    w = whiteAverage / 30;
  }
  if (redAverage > 7500) {
    r = 255;
  } else {
    r = redAverage / 30;
  }
  if (blueAverage > 7500) {
    b = 255;
  } else {
    b = blueAverage / 30;
  }
  if (greenAverage > 7500) {
    g = 255;
  } else {
    g = greenAverage / 30;
  }

  uint32_t color = strip.Color(r, b, g, w);
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();                     
 
}
