/*
 *
 * Author: Andrew Doble
 * Creation date: 02.02.2018
 */

#ifndef _VirtualPinEmulation_H
#define _VirtualPinEmulation_H

#include <Arduino.h>
#include <SPI.h>

#include "VirtualPin.h"

// Emulation instruction code
const byte EXP_PIN_EMULATION_INSTRUCTION        = 0x07;

class VirtualPinEmulation : public VirtualPin
{
private:
  uint8_t _cs;             // The chip select line used for teh expanded pin emulator chip
  uint8_t _virtualOutPin;  // The virtual pin that is translated to another pin by
                          // the expanded pin emulator.

public:
   VirtualPinEmulation(uint8_t cs, uint8_t virtualOutPin);
   void begin();
   void write(uint8_t value);
   void mode(uint8_t mode);


};

#endif
