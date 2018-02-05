
#include "VirtualPinDecoder.h"

// Test Code for expander simulation
int vsChipSelectPin = 4;
int memChipSelectPin = 5;
int fpChipSelectPin = 6;
int inAPin = 7;
int inBPin = 8;
int state;


VirtualPin vsChipSelect = VirtualPin(12,13, HIGH, HIGH);
VirtualPin memChipSelect = VirtualPin(12,13, HIGH,LOW);
VirtualPin fpChipSelect = VirtualPin(12,13, LOW, LOW);

void setup() {
  pinMode(vsChipSelectPin, OUTPUT);
  pinMode(memChipSelectPin, OUTPUT);
  pinMode(fpChipSelectPin, OUTPUT);
  pinMode(inAPin, INPUT);
  pinMode(inBPin, INPUT);

}

void loop() {

    vsChipSelect.write(LOW);
    multiplex();
    delay(1);
    memChipSelect.write(LOW);
    multiplex();
    delay(1);
    fpChipSelect.write(LOW);
    multiplex();
    delay(1);

}

void multiplex() {
  int inA = digitalRead(inAPin);
  int inB = digitalRead(inBPin);

  if (inA == HIGH && inB == HIGH) {
    digitalWrite(vsChipSelectPin, LOW);
    digitalWrite(memChipSelectPin, HIGH);
    digitalWrite(fpChipSelectPin, HIGH);
  }
  else if  (inA == HIGH && inB == LOW) {
    digitalWrite(vsChipSelectPin, HIGH);
    digitalWrite(memChipSelectPin, LOW);
    digitalWrite(fpChipSelectPin, HIGH);
  }
  else if  (inA == LOW && inB == LOW) {
    digitalWrite(vsChipSelectPin, HIGH);
    digitalWrite(memChipSelectPin, HIGH);
    digitalWrite(fpChipSelectPin, LOW);
  }
  else {
    digitalWrite(vsChipSelectPin, HIGH);
    digitalWrite(memChipSelectPin, HIGH);
    digitalWrite(fpChipSelectPin, HIGH);
  }

}
