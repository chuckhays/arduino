#include <avr/sleep.h>

#define RELAY 2

void setup() {
  ADCSRA &= ~(1<<ADEN);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
}

void loop() {
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(RELAY, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(RELAY, LOW);
  delay(1000);
  sleep_mode();
}
