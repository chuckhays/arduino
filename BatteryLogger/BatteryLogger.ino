#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BOARD_LED_PIN             13
#define VBAT_ENABLED              1
#define VBAT_PIN                  A7
  
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void renderBattery (float battery)
{
  #define BATTTEXT_STARTX     77
  #define BATTTEXT_STARTY     0
  #define BATTICON_STARTX     110
  #define BATTICON_STARTY     0
  #define BATTICON_WIDTH      18
  #define BATTICON_BARWIDTH3  ((BATTICON_WIDTH - 6) / 3)


  // Render the voltage in text
  display.clearDisplay();
  display.setCursor(BATTTEXT_STARTX, BATTTEXT_STARTY);
  display.print(battery, 2);
  display.println("V");
  Serial.println("Sending to display");

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
  display.display();
}

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

  renderBattery(vbatFloat);
  //oled.setBattery(vbatFloat);
  //oled.refreshIcons();
 
}

void setup()
{
  Serial.begin(9600);
  // Wait for Serial Monitor
//  while(!Serial) delay(1);
   
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
  display.print("Test");
  display.display();
  
  //oled.init();
  //oled.clearDisplay();

  //oled.setBatteryIcon(true);
  updateVbat();
}

void loop()
{
  // Update the battery level
  updateVbat();
  digitalWrite(BOARD_LED_PIN, HIGH);
  delay(100);
  digitalWrite(BOARD_LED_PIN, LOW);
  delay(100);
}
