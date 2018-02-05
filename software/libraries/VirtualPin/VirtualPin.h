/*
 * Interface for expanded pins.
 * Author: Andrew Doble
 * Creation date: 21.01.2018
 */

#ifndef _VirtualPin_H
#define _VirtualPin_H

#include <Arduino.h>

class VirtualPin
{
private:
  uint8_t _pin;

public:
  VirtualPin();
  VirtualPin(int8_t pin);

  /*
   * Initialise the standard pin
   */
  virtual void begin();

  /*
   * Write a LOW or HIGH value to the pin represented by
   * this expanded pin object.
   */
  virtual void write(uint8_t value);

  /*
   * Set the mode of the virtual pins, i,e:
   *    INPUT, OUTPUT, or INPUT_PULLUP
   * Note: Not all derived classes and thier corresponding devices can
   * accept all these values.
   */
  virtual void mode(uint8_t);

};

#endif
