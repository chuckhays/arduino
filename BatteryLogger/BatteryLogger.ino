#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"
#include <RTCZero.h>

#define BOARD_LED_PIN             13
#define VBAT_PIN                  A7
#define V12_PIN                   A2
  
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
RTC_DS3231 offboard_rtc;

/* Create an rtc object */
RTCZero rtc;

#define BATTTEXT_STARTX     77
#define BATTTEXT_STARTY     0
#define BATTICON_STARTX_OFFSET     33
#define BATTICON_STARTY     0
#define BATTICON_WIDTH      18
#define BATTICON_BARWIDTH3  ((BATTICON_WIDTH - 6) / 3)
  
void renderBattery (float battery, int startX)
{

  int BATTICON_STARTX = startX + BATTICON_STARTX_OFFSET;

  // Render the voltage in text
  display.setCursor(startX, BATTTEXT_STARTY);
  display.print(battery, 2);
  display.println("V");

  // Draw the base of the battery
  display.drawLine( BATTICON_STARTX + 1,
            BATTICON_STARTY,
            BATTICON_STARTX + BATTICON_WIDTH - 4,
            BATTICON_STARTY,
            WHITE);
  display.drawLine( BATTICON_STARTX,
            BATTICON_STARTY + 1,
            BATTICON_STARTX,
            BATTICON_STARTY + 5,
            WHITE);
  display.drawLine( BATTICON_STARTX + 1,
            BATTICON_STARTY + 6,
            BATTICON_STARTX + BATTICON_WIDTH - 4,
            BATTICON_STARTY + 6,
            WHITE);
  display.drawPixel(BATTICON_STARTX + BATTICON_WIDTH - 3,
            BATTICON_STARTY + 1,
            WHITE);
  display.drawPixel(BATTICON_STARTX + BATTICON_WIDTH - 2,
            BATTICON_STARTY + 1,
            WHITE);
  display.drawLine( BATTICON_STARTX + BATTICON_WIDTH - 1,
            BATTICON_STARTY + 2,
            BATTICON_STARTX + BATTICON_WIDTH - 1,
            BATTICON_STARTY + 4,
            WHITE);
  display.drawPixel(BATTICON_STARTX + BATTICON_WIDTH - 2,
            BATTICON_STARTY + 5,
            WHITE);
  display.drawPixel(BATTICON_STARTX + BATTICON_WIDTH - 3,
            BATTICON_STARTY + 5,
            WHITE);
  display.drawPixel(BATTICON_STARTX + BATTICON_WIDTH - 3,
            BATTICON_STARTY + 6,
            WHITE);

  // Draw the appropriate number of bars
  if (battery > 4.26F)
  {
    // USB (Solid Rectangle)
    display.fillRect(BATTICON_STARTX + 2,     // X
             BATTICON_STARTY + 2,     // Y
             BATTICON_BARWIDTH3 * 3,  // W
             3,                       // H
             WHITE);
  }
  else if ((battery <= 4.26F) && (battery >= 4.1F))
  {
    // Three bars
    for (uint8_t i = 0; i < 3; i++)
    {
      display.fillRect(BATTICON_STARTX + 2 + (i * BATTICON_BARWIDTH3),
               BATTICON_STARTY + 2,
               BATTICON_BARWIDTH3 - 1,
               3,
               WHITE);
    }
  }
  else if ((battery < 4.1F) && (battery >= 3.8F))
  {
    // Two bars
    for (uint8_t i = 0; i < 2; i++)
    {
      display.fillRect(BATTICON_STARTX + 2 + (i * BATTICON_BARWIDTH3),
               BATTICON_STARTY + 2,
               BATTICON_BARWIDTH3 - 1,
               3,
               WHITE);
    }
  }
  else if ((battery < 3.8F) && (battery >= 3.4F))
  {
    // One bar
    display.fillRect(BATTICON_STARTX + 2,
             BATTICON_STARTY + 2,
             BATTICON_BARWIDTH3 - 1,
             3,
             WHITE);
  }
  else
  {
    // No bars
  }
}

float getVbat() 
{
  float vbatFloat = 0.0F;
  // Read the analog in value:
  vbatFloat = analogRead(VBAT_PIN);
  vbatFloat = analogRead(VBAT_PIN);
  // 10k/10k divider.
  vbatFloat *= 2.0F;
  vbatFloat *= 3.3F;
  vbatFloat /= 1024.0F;

  return vbatFloat;
}

float getV12()
{
  float v12Float = 0.0F;
  // Read the analog in value:
  v12Float = analogRead(V12_PIN);
  v12Float = analogRead(V12_PIN);

  // 480/2.7k divider.
  v12Float *= 6.96F; 
  v12Float *= 3.3F;
  v12Float /= 1024.0F;
  return v12Float;
}

void setup()
{
  Serial.begin(9600);
  // Wait for Serial Monitor
//  while(!Serial) delay(1);

  if (! offboard_rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (offboard_rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    offboard_rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  rtc.begin();
  DateTime now = offboard_rtc.now();
  rtc.setTime(now.hour(), now.minute(), now.second());
  rtc.setDate(now.day(), now.month(), now.year());

  //rtc.setAlarmTime(00, 00, 10);
  //rtc.enableAlarm(rtc.MATCH_SS);
   
  // Setup the LED pin
  pinMode(BOARD_LED_PIN, OUTPUT);
 
  // Setup the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
}

void loop()
{
  display.clearDisplay();
  // Update the battery level
  float v1 = getVbat();
  renderBattery(v1, 0);
  float v2 = getV12();
  renderBattery(v2, BATTTEXT_STARTX);

  DateTime now = offboard_rtc.now();
  display.setCursor(0, 10);
  display.print(now.year(), DEC);
  display.print('/');
  display.print(now.month(), DEC);
  display.print('/');
  display.print(now.day(), DEC);
  display.print(" ");
  display.print(now.hour(), DEC);
  display.print(':');
  display.print(now.minute(), DEC);
  display.print(':');
  display.print(now.second(), DEC);
  display.println();
  display.display();
  digitalWrite(BOARD_LED_PIN, HIGH);
  delay(100);
  digitalWrite(BOARD_LED_PIN, LOW);
  delay(100);
  //rtc.standbyMode();
}
