#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>
#include <Adafruit_VEML6075.h>

Adafruit_FeatherOLED display = Adafruit_FeatherOLED();
Adafruit_VEML6075 uv = Adafruit_VEML6075();

#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5

#define VBATPIN A7

float getBatteryVoltage() {

  float measuredvbat = analogRead(VBATPIN);

  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage

  return measuredvbat;

}

void setup() {
  display.init();
  display.setBatteryVisible(true);
  display.setBatteryIcon(true);

  Serial.println("OLED begun");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Initializing...");
  display.setCursor(0, 0);
  display.display(); // actually display all of the above
  uv.begin();
  uv.setIntegrationTime(VEML6075_100MS);
  //pinMode( 13, OUTPUT );
  //      while ( ! Serial ) { delay( 100 ); digitalWrite( 13, ! digitalRead( 13 )); }
  //      digitalWrite( 13, LOW );

  Serial.println("UV reader");
  // Clear the buffer.
  display.clearDisplay();
  display.display();

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);


}

void loop() {
  display.clearDisplay();
  Serial.print("Reading A...");
  float uva = max(0, uv.readUVA());
  Serial.println(uva);
  Serial.print("Reading B...");
  float uvb = max(0, uv.readUVB());
  Serial.println(uvb);
  Serial.print("Reading I...");
  float uvi = max(0, uv.readUVI());
  Serial.println(uvi);

  float battery = getBatteryVoltage();
  display.setBattery(battery);
  display.renderBattery();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("A:");
  display.println(uva, 0);
  display.print("B:");
  display.println(uvb, 0);
  display.print("I:");
  display.println(uvi, 1);
  display.setCursor(0, 0);
  display.display(); // actually display all of the above
}
