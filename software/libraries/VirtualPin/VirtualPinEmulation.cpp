#include "VirtualPinEmulation.h"

VirtualPinEmulation::VirtualPinEmulation(uint8_t cs, uint8_t virtualOutPin)
{
  _cs = cs;
  _virtualOutPin = virtualOutPin;
}

void VirtualPinEmulation::begin()
{
  digitalWrite(_cs, LOW);

  //  Transfer the instruction
  SPI.transfer(EXP_PIN_EMULATION_INSTRUCTION);
  delayMicroseconds(50);   //Wait for the instruction to be processed by the slave.

  // Transfer byte representing a reset
  SPI.transfer(0);
  digitalWrite(_cs, HIGH);
}

void VirtualPinEmulation::write(uint8_t value)
{
  digitalWrite(_cs, LOW);

  //  Transfer the instruction
  SPI.transfer(EXP_PIN_EMULATION_INSTRUCTION);
  delayMicroseconds(50);   //Wait for the instruction to be processed by the slave.

  // Transfer byte representing the expanded pin
  SPI.transfer(_virtualOutPin);
  digitalWrite(_cs, HIGH);

}

void VirtualPinEmulation::mode(uint8_t mode) {
  // Do nothing
}
