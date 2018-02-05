/*
 *
 *
 * Author: Andrew Doble
 * Creation date: 07.08.2016
 */

#include "SPIRingBuffer.h"

/**
 * Class constructor
 *
 * param chipSelectPin The digital pin used a SPI slave select
 */
SPIRingBuffer::SPIRingBuffer(uint8_t chipSelectPin)
{
  //csPin = chipSelectPin;
  _virtualChipSelect = new VirtualPin(chipSelectPin);
  operationMode = 0xFF;   // TODO need it?
}

/**
 * Class constructor with expanded pin
 *
 * param virtualChipSelect The virtual expanded pin used
 *                         to drive the SPI slave select
 */
SPIRingBuffer::SPIRingBuffer(VirtualPin virtualChipSelect) {
  _virtualChipSelect = &virtualChipSelect;
  operationMode = 0xFF;   // TODO need it?
}


/**
 *  Initialize the SPI communication
 *
 */
 void SPIRingBuffer::begin(void)
 {
   //pinMode(csPin, OUTPUT);
   _virtualChipSelect->mode(OUTPUT);
   chipSelect(false);

   //spi.setFrequency(RAMCLK);
   spi.begin();

   // reset the indexes.
   //reset();
 }

 /**
  *  Initialize the indexes.
  *  Existing data will be overwritten started at address 0.
  */
void SPIRingBuffer::reset(void)
{
  head = -1;
  tail = -1 ;
}

/**
 * Puts a byte into the streaming buffer in the SPI RAM.
 * Returns -1 if the buffer is full. Else returns
 * the added data.
 */
int32_t SPIRingBuffer::put(uint8_t data)
{

  // Advance the head address.
  // Also handles the initial condition when head is -1,
  // i.e. head is always positive when the modulo operator is appplied.
  head = (head + 1) % RING_BUFFER_LENGTH;

  // Check for buffer overload, i.e. the head will overtake the tail
  if (head == tail) return -1;

  // Add the data
  writeByte(head, data);


  // If no data has been written as yet (i.e. tail is -1) then
  // update the tail, otherwise leave it as it  was.
  if (tail == -1) tail = 0;

  return data;
}

/**
 * Gets a byte from the streaming buffer in SPI RAM.
 * If the buffer is empty, returns -1.
 */
 int32_t SPIRingBuffer::get(void)
{
  uint8_t data;  // DOBLE type was byte

  // Initial condition and no data loaded or all data has been read
  if (tail == -1) return -1;

  // Read the data
  data = readByte(tail);


  if (tail == head) {
    // All data has been read so reset
    head = -1;
    tail = -1;
  }
  else {
    // Advance the tail index
    tail = (tail + 1) % RING_BUFFER_LENGTH;
  }
  return data;

}

/**
 * Returns how much data is in the streaming buffer
 */
int32_t SPIRingBuffer::availableData(void)
{
  // Intial condition
  if (tail == -1 && head == -1) return 0;

  if (tail <= head) {
    return (head - tail + 1);
  }
  else {
    return (RING_BUFFER_LENGTH - (tail - head) + 1);
  }
}

/**
 * Returns how much space is available in the streaming buffer
 */
int32_t SPIRingBuffer::availableSpace(void)
{
  return (RING_BUFFER_LENGTH - availableData());
}

/**
 * Returns true if the buffer is full
 */
bool SPIRingBuffer::bufferFull()
{
  return ((RING_BUFFER_LENGTH - availableData()) == 0);
}


/**
 * Get the head index. This is mainly used for debugging purposes.
 * Note that the head index cannot be set.
 */
int32_t SPIRingBuffer::getHead(void)
{
  return head;
}

/**
 * Get the tail index. This is mainly used for debugging purposes.
 * Note that the tail index cannot be set.
 */
int32_t SPIRingBuffer::getTail(void)
{
  return tail;
}

/**
 * Set operation mode
 *
 * @param mode Operation mode
 * TODO need it?
 */
void SPIRingBuffer::setMode(char mode)
{
  if (mode != operationMode)
  {
    //SPI.beginTransaction(RAM_SPI_SETTING);
    chipSelect(true);
    spi.transfer(WRSR);
    spi.transfer(mode);
    chipSelect(false);
    //SPI.endTransaction();
    operationMode = mode;
  }
}

/**
 * Read byte from SRAM
 *
 * @param address Memory address
 *
 * @return Byte read
 */
//unsigned char SPIRingBuffer::readByte(unsigned int address)
unsigned char SPIRingBuffer::readByte(unsigned int address)

{
  //unsigned char res;
  unsigned char res;


  //SPI.beginTransaction(RAM_SPI_SETTING);

  // Set byte mode
  setMode(BYTE_MODE);

  // Write address, read data
  chipSelect(true);
  spi.transfer(READ);
  spi.transfer((unsigned char)(address >> 8));
  spi.transfer((unsigned char)address);

  res = spi.transfer(0xFF);

  // TODO DEBUG
  //Serial.print("Just read: 0x"); Serial.print(res, HEX);Serial.print(" from 0x"); Serial.println(address, HEX);

  chipSelect(false);

  //SPI.endTransaction();

  return res;
}

/**
 * Write byte to SRAM
 *
 * @param address Memory address
 * @param data Data byte to be written
 */
void SPIRingBuffer::writeByte(unsigned int address, char data)
{
  //SPI.beginTransaction(RAM_SPI_SETTING);

  // Set byte mode
  setMode(BYTE_MODE);


  //Serial.print("Write MSB: 0x"); Serial.print((uint8_t)(address >> 8), HEX);
  //Serial.print(" LSB: 0x"); Serial.println((uint8_t)(address), HEX);

  // Write address, read data
  chipSelect(true);
  spi.transfer(WRITE);
  spi.transfer((unsigned char)(address >> 8));
  spi.transfer((unsigned char)address);

  spi.transfer(data);

  // TODO remve debug statements
  //Serial.print("Just wrote: 0x");Serial.println(data, HEX);Serial.print(" to 0x"); Serial.println(address, HEX);

  chipSelect(false);

  //SPI.endTransaction();
}

/**
 * Enable chip via CS pin
 *
 * @param state True to select. False to unselect
 */
 void  SPIRingBuffer::chipSelect(bool state)
 {
   //digitalWrite(csPin, !state);
   _virtualChipSelect->write(!state);
 }
