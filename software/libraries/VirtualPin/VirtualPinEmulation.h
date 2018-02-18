/*
 *  Driver for an AVR emulation for a pin decoder.
 *
 *  Uses to SPI to send two nBytes
 *  1  - Instruction to tell the emulation that it should act as a virtual pin.
 *
 *  2  - A byte with the following structure
 *          Bit 7   - 1 if the specified virtual pin should be driven HIGH.
 *                    0 if the specified virtual pin should be driven LOW
 *          Bit 0-6 - The virtual pin. This has a range from 1 - 63
 *       If this byte is 0 then this is acts as a reset.
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
const byte VIRT_PIN_EMULATION_INSTRUCTION = 0x07;

const int PROCESSING_TIME = 85; // The time to process an instruction or data in microseconds

class VirtualPinEmulation : public VirtualPin
{
private:
  uint8_t _cs;             // The chip select line used for the expanded pin emulator chip
  uint8_t _virtualOutPin;  // The virtual pin that is translated to another pin by
                           // the expanded pin emulator.

public:
   VirtualPinEmulation(uint8_t cs, uint8_t virtualOutPin);
   void begin();
   void write(uint8_t value);
   void mode(uint8_t mode);


};

#endif
