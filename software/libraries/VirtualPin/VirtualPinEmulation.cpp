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
  SPI.transfer(VIRT_PIN_EMULATION_INSTRUCTION);
  delayMicroseconds(PROCESSING_TIME);   //Wait for the instruction to be processed by the slave.

  // Transfer byte representing a reset
  SPI.transfer(0);
  delayMicroseconds(PROCESSING_TIME);   //Wait for the data to be processed by the slave.

  digitalWrite(_cs, HIGH);
}

void VirtualPinEmulation::write(uint8_t value)
{
  digitalWrite(_cs, LOW);

  //  Transfer the instruction
  SPI.transfer(VIRT_PIN_EMULATION_INSTRUCTION);
  delayMicroseconds(PROCESSING_TIME);   // Wait for the instruction to be processed by the slave.

  // Transfer byte representing the expanded pin

  uint8_t data = 0x00;
  if (value == HIGH) {
    data  = 1<<7;
  }
  data |= _virtualOutPin;

  SPI.transfer(data);
  delayMicroseconds(PROCESSING_TIME);   // Wait for the data to be processed by the slave.

  digitalWrite(_cs, HIGH);

}

void VirtualPinEmulation::mode(uint8_t mode) {
  // Do nothing
}
