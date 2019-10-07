#include <IRremote.h>
#include <Wire.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;
uint8_t data[4];

void setup()
{
  Wire.begin();
  //Serial.begin(9600);
  //Serial.println("Enabling IRin + Wire");
  irrecv.enableIRIn(); // Start the receiver
  //Serial.println("Enabled IRin");
}

void loop() {
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    unsigned long v = results.value;
    data[0] = (v >> 24) & 0xFF;
    data[1] = (v >> 16) & 0xFF;
    data[2] = (v >> 8) & 0xFF;
    data[3] = v & 0xFF;
    Wire.beginTransmission(8);
    Wire.write(data, 4);
    Wire.endTransmission();
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
