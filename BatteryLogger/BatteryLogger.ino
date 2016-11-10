#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>
#include <Adafruit_FeatherOLED_WiFi.h>

#define BOARD_LED_PIN             13
#define VBAT_ENABLED              1
#define VBAT_PIN                  A7
  
Adafruit_FeatherOLED_WiFi  oled = Adafruit_FeatherOLED_WiFi();

void updateVbat() 
{
  float vbatFloat = 0.0F;
 
  // Read the analog in value:
  vbatFloat = analogRead(VBAT_PIN);
  vbatFloat = analogRead(VBAT_PIN);
  Serial.println(vbatFloat);

  vbatFloat *= 2.0F;
  vbatFloat *= 3.3F;
  vbatFloat /= 1024.0F;
  Serial.println(vbatFloat);
 
  oled.setBattery(vbatFloat);
  oled.refreshIcons();
 
}

void setup()
{
  Serial.begin(9600);
  // Wait for Serial Monitor
//  while(!Serial) delay(1);
   
  // Setup the LED pin
  pinMode(BOARD_LED_PIN, OUTPUT);
 
  // Setup the OLED display
  oled.init();
  oled.clearDisplay();

  oled.setBatteryIcon(true);
  updateVbat();
}
 
/**************************************************************************/
/*!
    @brief  This loop function runs over and over again
*/
/**************************************************************************/
void loop()
{
  // Update the battery level
  updateVbat();
  digitalWrite(BOARD_LED_PIN, HIGH);
  delay(100);
  digitalWrite(BOARD_LED_PIN, LOW);
  delay(1000);
}
