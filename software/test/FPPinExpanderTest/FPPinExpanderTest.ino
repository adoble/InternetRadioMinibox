/*
 * FPPinExpanderTest
 *
 * Test of the FPController virtual pin emulation (expanded pins for chip selects).
 *
 * Needs to run on the ESP8266.
 *
 */

#include <SPI.h>

#include "VirtualPinEmulation.h"

const int SLAVE_SELECT_PIN = 0; // TODO check that this is correct one
                                // This is the pin on the ESP8266 side connected to the SS pin
                                // AVR uC running the FP controller software

VirtualPinEmulation virtualPin1 = VirtualPinEmulation(SLAVE_SELECT_PIN, 1);
VirtualPinEmulation virtualPin2 = VirtualPinEmulation(SLAVE_SELECT_PIN, 2);

const byte INST_NULL = 0x00;


const int MS = 1;
const int US = 0;

boolean z = false;

void setup() {
  Serial.begin(115200);

  Serial.println("Initializing");
  SPI.begin();

  pinMode(SLAVE_SELECT_PIN, OUTPUT);
  
  FP_begin(); // To reset THIS IS IMPORTANT --> Make it a reset function the master

  virtualPin1.begin();
  virtualPin2.begin();

  }

void loop() {
  unsigned long timestamp; 
  
//  timestamp = micros();
  virtualPin1.write(HIGH);
//  timestamp = micros() - timestamp;
//  Serial.print("-->"); Serial.println(timestamp); 
  
//  wait(500, MS);
//  virtualPin2.write(LOW);
  //wait(50, US);
  virtualPin1.write(LOW);
  //wait(50, US);
//  virtualPin2.write(HIGH);
//  wait(1000, US);

}

void wait(unsigned long period, int unit) {

  if (unit == US) {
    delayMicroseconds(period);
  }
  else {
    delay(period);
  }
  
}


void FP_begin() {
  Serial.println("SEND NULL INSTRUCTION TO RESET");
  digitalWrite(SLAVE_SELECT_PIN, LOW);
  SPI.transfer(INST_NULL);
  digitalWrite(SLAVE_SELECT_PIN, HIGH);
}
