/*
 * FPPinExpanderTest
 *
 * Test of the  virtual pin emulation (expanded pins for chip selects)
 * using the 2-line to 3-line decoder
 *
 * Needs to run on the ESP8266.
 *
 */

#include "VirtualPinDecoder.h"

// Pin setup for the AVR chip used as front panel controller
const int DECODE_0_PIN = 15;
const int DECODE_1_PIN = 0;

VirtualPinDecoder xcsVirtualPin = VirtualPinDecoder(DECODE_0_PIN, DECODE_1_PIN, LOW, HIGH);
VirtualPinDecoder ramCSVirtualPin = VirtualPinDecoder(DECODE_0_PIN, DECODE_1_PIN, HIGH, HIGH);
VirtualPinDecoder fpCSVirtualPin = VirtualPinDecoder(DECODE_0_PIN, DECODE_1_PIN, HIGH, LOW);

const int MS = 1;
const int US = 0;


void setup() {
  Serial.begin(115200);

  Serial.println("Initializing");

  pinMode(DECODE_0_PIN, OUTPUT);
  pinMode(DECODE_1_PIN, OUTPUT);

  xcsVirtualPin.begin();
  ramCSVirtualPin.begin();
  fpCSVirtualPin.begin();
}

void loop() {
  unsigned long period = 50;

//  timestamp = micros();
  xcsVirtualPin.write(HIGH);
  wait(period, MS);
  xcsVirtualPin.write(LOW);
  wait(period, MS);
  ramCSVirtualPin.write(HIGH);
  wait(period, MS);
  ramCSVirtualPin.write(LOW);
  wait(period, MS);
  fpCSVirtualPin.write(HIGH);
  wait(period, MS);
  fpCSVirtualPin.write(LOW);
  wait(period, MS);
}

void wait(unsigned long period, int unit) {

  if (unit == US) {
    delayMicroseconds(period);
  }
  else {
    delay(period);
  }

}
