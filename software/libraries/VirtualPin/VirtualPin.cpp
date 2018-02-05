/*
 * Uses the VirtualPin interface, but just does the same as directly
 * using a pin.
 * This class is used wwhen a default implementation of a pin is
 * required, for instance when a library as an constructor that has
 * two function prototypes: ine  uses a normal pin and the other the
 * expanded pin. The normal pin can be replaced with the this class so
 * that the code for both cases is the same (i.e. polymorphism)
 * Author: Andrew Doble
 * Creation date: 03.02.2018
 */

#include "VirtualPin.h"

VirtualPin::VirtualPin() {}

VirtualPin::VirtualPin(int8_t pin) {
  _pin = pin;
}


void VirtualPin::begin() {
  pinMode(_pin, OUTPUT);
}

void VirtualPin::write(uint8_t value) {
  digitalWrite(_pin, value);
}

void VirtualPin::mode(uint8_t mode) {
  pinMode(_pin, mode);
}
