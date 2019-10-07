#include <Adafruit_NeoPixel.h>

#include <Wire.h>

#include <EEPROM.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
# define LED_PIN 6

// How many NeoPixels are attached to the Arduino?
# define LED_COUNT 30

// NeoPixel brightness, 0 (min) to 255 (max)
# define BRIGHTNESS 255

#define NONE 0
#define CLOUDY 1
#define STORM 2
#define RAINBOW 3

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

uint8_t w = 0;
uint8_t r = 0;
uint8_t b = 0;
uint8_t g = 0;
bool wEnabled = false;
bool cEnabled = false;
uint8_t cBright = 0;
uint8_t specialMode = 0;

uint8_t data[4];

void setup() {
  w = EEPROM.read(0);
  r = EEPROM.read(1);
  g = EEPROM.read(2);
  b = EEPROM.read(3);
  cBright = EEPROM.read(4);
  wEnabled = EEPROM.read(5);
  cEnabled = EEPROM.read(6);
  specialMode = EEPROM.read(7);

  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  //Serial.begin(9600);

  strip.begin();
  strip.show();
  strip.setBrightness(255);
}

void receiveEvent(int howMany) {
  // We need to receive 4 bytes and convert to an unsigned long.
  data[0] = 0;
  data[1] = 0;
  data[2] = 0;
  data[3] = 0;
  int index = 0;
  while (Wire.available()) {
    uint8_t d = Wire.read();
    if (index < 4) {
      data[index] = d;
    }
    index++;
  }
  if (index == 4) {
    // Convert to unsigned long.
    unsigned long output = data[0];
    output = (output << 8) | data[1];
    output = (output << 8) | data[2];
    output = (output << 8) | data[3];
    command(output);
  }
}

void command(unsigned long command) {
  switch (command) {
  case 0xF700FF: // Color +
    cBright = cBright + min(25, 255 - cBright);
    break;
  case 0xF720DF: // R
    r = 255;
    g = 0;
    b = 0;
    break;
  case 0xF710EF:
    r = 255;
    g = 20;
    b = 0;
    break;
  case 0xF730CF:
    r = 255;
    g = 40;
    b = 0;
    break;
  case 0xF708F7:
    r = 255;
    g = 65;
    b = 0;
    break;
  case 0xF728D7:
    r = 255;
    g = 128;
    b = 0;
    break;

  case 0xF7807F: // Color -
    cBright = cBright - min(25, cBright);
    break;
  case 0xF7A05F: // G
    r = 0;
    g = 255;
    b = 0;
    break;
  case 0xF7906F:
    r = 150;
    g = 255;
    b = 150;
    break;
  case 0xF7B04F:
    r = 138;
    g = 240;
    b = 255;
    break;
  case 0xF78877:
    r = 75;
    g = 185;
    b = 180;
    break;
  case 0xF7A857:
    r = 20;
    g = 195;
    b = 230;
    break;

  case 0xF740BF: // Color on/off
    cEnabled = !cEnabled;
    specialMode = NONE;
    break;
  case 0xF7609F: // B
    r = 0;
    g = 0;
    b = 255;
    break;
  case 0xF750AF:
    r = 10;
    g = 175;
    b = 245;
    break;
  case 0xF7708F:
    r = 105;
    g = 80;
    b = 175;
    break;
  case 0xF748B7:
    r = 175;
    g = 90;
    b = 210;
    break;
  case 0xF76897:
    r = 255;
    g = 51;
    b = 153;
    break;

  case 0xF7C03F: // W on/off
    wEnabled = !wEnabled;
    specialMode = NONE;
    break;
  case 0xF7E01F: // W +
    w = w + min(25, 255 - w);
    break;
  case 0xF7D02F: // W -
    w = w - min(25, w);
    break;
  case 0xF7F00F: // Cloudy
  specialMode = CLOUDY;
    break;
  case 0xF7C837: // Thunderstorm
  specialMode = STORM;
    break;
  case 0xF7E817: // Rainbow
  specialMode = RAINBOW;
    break;
  }

  // Store values.
  EEPROM.update(0, w);
  EEPROM.update(1, r);
  EEPROM.update(2, g);
  EEPROM.update(3, b);
  EEPROM.update(4, cBright);
  EEPROM.update(5, wEnabled);
  EEPROM.update(6, cEnabled);
  EEPROM.update(7, specialMode);
}

void loop() {
  float colorScale = ((float) cBright / 255.0);

  uint8_t red = ((float) r * colorScale);
  uint8_t blue = ((float) b * colorScale);
  uint8_t green = ((float) g * colorScale);
  uint32_t color = strip.Color(cEnabled ? red : 0, cEnabled ? green : 0, cEnabled ? blue : 0, wEnabled ? w : 0);
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
