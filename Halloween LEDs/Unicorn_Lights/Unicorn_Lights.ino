#include <Adafruit_NeoPixel.h>

#define PIN 6

#define NUM_LEDS 22

#define BRIGHTNESS 255

#define POTPIN A5
#define LEDPIN 5
#define SWITCHPIN 9

#define PATTERNS_COUNT 9

#define BUTTON_PRESS_DELAY 150

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

uint8_t current_pattern = 0;
unsigned long last_press_start_time = 0;
bool switch_pressed = false;
bool switch_press_recorded = false;

void setup()
{
  pinMode(LEDPIN, OUTPUT);          // Set switch LED pin to output.
  pinMode(SWITCHPIN, INPUT_PULLUP); // Set switch pin to input.
  strip.setBrightness(BRIGHTNESS);  // Set max brightness.
  strip.begin();                    // Initialize the neopixel strip object.
  strip.show();                     // Output current LED colors (all off currently).

  Serial.begin(9600);
}

void loop()
{
  // First, check if the switch is pressed.
  checkForSwitchPress();

  int sensorValue = analogRead(POTPIN) / 4;
  uint16_t pause = 0;
  // Next, decide which pattern we are currently showing and run it.
  switch (current_pattern)
  {
  case 0:
    pause = rainbow();
    break;
  case 1:
    pause = alternateRainbow();
    break;
  case 2:
    pause = purple();
    break;
  case 3:
    pause = orange();
    break;
  case 4:
    pause = solid(sensorValue);
    break;
  case 5:
    pause = strobe(sensorValue);
    break;
  case 6:
    pause = strobe(0);
    break;
  case 7:
    pause = fire();
    break;
  case 8:
    pause = police();
    break;
  }
  strip.show();

  delay(pause);
  analogWrite(LEDPIN, sensorValue);
}

byte currentRainbowColor = 0;
uint16_t rainbow()
{
  for (uint16_t i = 0; i <= 7; i++)
  {
    uint32_t color = Wheel(((i * 36) + currentRainbowColor) & 255);
    setRowColor(i, color);
  }
  ++currentRainbowColor;
  return 2;
}

byte currentAlternateRainbowColor = 0;
uint16_t alternateRainbow()
{
  for (uint16_t i = 0; i <= 2; i++)
  {
    uint32_t color = Wheel(((i * 85) + currentAlternateRainbowColor) & 255);
    setColumnColor(i, color);
    setRowColor(7, strip.Color(0, 0, 0, 0));
  }
  ++currentAlternateRainbowColor;
  return 2;
}

byte currentPoliceColor = 0;
uint16_t police() {
  for (uint16_t i = 0; i <= 2; i++) {
    uint32_t color = policeWheel(((i * 85) + currentPoliceColor) &255);
    setColumnColor(i, color);
    setRowColor(7, strip.Color(0,0,0,0));
  }
  ++currentPoliceColor;
  return 2;
}

byte currentPurpleRainbowColor = 0;
uint16_t purple()
{
  for (uint16_t i = 0; i <= 7; i++)
  {
    uint32_t color = purpleWheel(((i * 36) + currentPurpleRainbowColor) & 255);
    setRowColor(i, color);
  }
  ++currentPurpleRainbowColor;
  return 2;
}

byte currentOrange = 0;
uint16_t orange()
{
  for (uint16_t i = 0; i <= 7; i++)
  {
    uint8_t wheelPos = ((i * 36) + currentOrange) & 255;
    uint32_t color = orangeWheel(wheelPos);
    setRowColor(7 - i, color);
  }
  ++currentOrange;
  return 2;
}

uint16_t solid(uint16_t sensor)
{
  uint32_t color = sensor == 0 ? strip.Color(255, 255, 255, 255) : Wheel(sensor & 255);
  uint32_t off = strip.Color(0, 0, 0, 0);
  for (uint16_t i = 0; i <= 7; i++)
  {
    setRowColor(i, color);
  }
  return 30;
}

byte currentStrobe = 0;
uint16_t strobe(uint16_t sensor)
{
  uint32_t color = sensor == 0 ? strip.Color(255, 255, 255, 255) : Wheel(sensor & 255);
  uint32_t off = strip.Color(0, 0, 0, 0);
  for (uint16_t i = 0; i <= 7; i++)
  {
    if (currentStrobe == 1)
    {
      setRowColor(i, color);
    }
    else
    {
      setRowColor(i, off);
    }
  }
  currentStrobe = (currentStrobe + 1) % 2;
  return 30 + 30 * currentStrobe;
}

uint16_t fire()
{
  int r = 255;
  int g = 125;
  int b = 12;
  for (uint16_t i = 0; i <= 7; ++i)
  {
    int flicker = random(0, 40);
    int r1 = r - random(0, 40);
    int g1 = g - random(0, 40);
    int b1 = b - random(0, 40);
    if (r1 < 0)
    {
      r1 = 0;
    }
    if (b1 < 0)
    {
      b1 = 0;
    }
    if (g1 < 0)
    {
      g1 = 0;
    }
    setRowColor(i, strip.Color(r1, g1, b1));
  }
  return random(50, 150);
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

// Tower 0 : Row [0-6] = Row, 13 - Row, Row + 14
// Tower 1 : Row [0-6] = 22+ Row, 22 + 13 - Row, 22 + Row + 14
// Tower 0: Row 7 = Row + 14 (or just 21)
// Tower 1: Row 7 = 22 + Row + 14 (or just 43)
//
// 0::6 means iterate in a loop for each number 0 through 6
// Tower 0: Column [0-2] = Column * 7 + (0::6)
// Tower 1: Column [0-2] = 22 + Column * 7 + (0::6)

// Set color for a logical row (0-7), will set all 3 LEDs (or single for top).
// Tower should be 0 or 1.
void setRowColor(uint8_t row, uint8_t tower, uint32_t color)
{
  if (row < 7)
  {
    strip.setPixelColor(tower * 22 + row, color);
    strip.setPixelColor(tower * 22 + 13 - row, color);
    strip.setPixelColor(tower * 22 + row + 14, color);
  }
  else
  {
    strip.setPixelColor(tower * 22 + 21, color);
  }
}

// Set color for a logical row(0-7) on both towers at once.
void setRowColor(uint8_t row, uint32_t color)
{
  setRowColor(row, 0, color);
  setRowColor(row, 1, color);
}

// Set color for a column with column (0-2), will set the entire column.
// Tower should be 0 or 1.
void setColumnColor(uint8_t column, uint8_t tower, uint32_t color)
{
  for (uint8_t i = 0; i < 7; i++)
  {
    strip.setPixelColor(tower * 22 + column * 7 + i, color);
  }
}

// Set color for a column with column (0-2) on both towers at once.
void setColumnColor(uint8_t column, uint32_t color)
{
  setColumnColor(column, 0, color);
  setColumnColor(column, 1, color);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

uint32_t policeWheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, 0, 255 - WheelPos * 3, WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 0, 0, 255 - WheelPos * 3);
}

uint32_t orangeWheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 126)
  {
    return strip.Color(255, WheelPos, 0);
  }
  WheelPos -= 126;
  return strip.Color(255, 129 - WheelPos, 0);
}

uint32_t purpleWheel(byte WheelPos)
{
  // WheelPos = 255 - WheelPos;
  if (WheelPos < 128)
  {
    byte red = WheelPos <= 125 ? 125 - WheelPos : 0;
    byte green = WheelPos * 2;
    byte blue = 255 - WheelPos * 2;
    return strip.Color(red, green, blue);
  }
  else
  {
    WheelPos -= 128;
    byte red = WheelPos <= 125 ? WheelPos : 125;
    byte green = 255 - WheelPos * 2;
    byte blue = WheelPos * 2;
    return strip.Color(red, green, blue);
  }
}

uint8_t red(uint32_t c)
{
  return (c >> 16);
}
uint8_t green(uint32_t c)
{
  return (c >> 8);
}
uint8_t blue(uint32_t c)
{
  return (c);
}

void checkForSwitchPress()
{
  // Check if the button is pressed.
  uint8_t switchVal = digitalRead(SWITCHPIN);
  // A value of 0 means it is pressed.
  if (switchVal)
  {
    // Button is not pressed.
    switch_pressed = false;
    switch_press_recorded = false;
  }
  else
  {
    if (switch_press_recorded)
    {
      // We already processed this switch press, don't do anything until the button is released.
      return;
    }
    // Button is pressed.
    if (switch_pressed)
    {
      // Button was pressed last time, check if it has been pressed long enough to advance.
      if (millis() - last_press_start_time > BUTTON_PRESS_DELAY)
      {
        // Button was held long enough to qualify as a press.
        current_pattern = (current_pattern + 1) % PATTERNS_COUNT;
        switch_press_recorded = true;
      }
    }
    else
    {
      // Button was not pressed last loop, record the time.
      switch_pressed = true;
      last_press_start_time = millis();
    }
  }
}
