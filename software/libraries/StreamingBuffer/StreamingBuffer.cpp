/*
 *
 *
 * Author: Andrew Doble
 * Creation date: 19.05.2018
 */

#include "StreamingBuffer.h"


 StreamingBuffer::StreamingBuffer(uint8_t *storage, int size) {
  _storage = storage;
  _size = size;
 }

/**
 *  Initialize
 *
 */
 void StreamingBuffer::begin()
 {
   // Reset the indexes.
   _head = -1;
   _tail = -1 ;
 }


/**
 * Puts a byte into the  buffer.
 * Returns -1 if the buffer is full. Else returns
 * the added data.
 */
int32_t StreamingBuffer::put(uint8_t data)
{

  // Advance the head address.
  // Also handles the initial condition when head is -1,
  // i.e. head is always positive when the modulo operator is appplied.
  _head = (_head + 1) % _size;

  // Check for buffer overload, i.e. the head will overtake the tail
  if (_head == _tail) return -1;

  // Add the data
  writeByte(_head, data);


  // If no data has been written as yet (i.e. tail is -1) then
  // update the tail, otherwise leave it as it  was.
  if (_tail == -1) _tail = 0;

  return data;
}

/**
 * Gets a byte from the buffer.
 * If the buffer is empty, returns -1.
 */
 int32_t StreamingBuffer::get(void)
{
  uint8_t data;

  // Initial condition and no data loaded or all data has been read
  if (_tail == -1) return -1;

  // Read the data
  data = readByte(_tail);


  if (_tail == _head) {
    // All data has been read so reset
    _head = -1;
    _tail = -1;
  }
  else {
    // Advance the tail index
    _tail = (_tail + 1) % _size;
  }
  return data;

}

/**
 * Returns how much data is in the streaming buffer
 */
int32_t StreamingBuffer::availableData(void)
{
  // Intial condition
  if (_tail == -1 && _head == -1) return 0;

  if (_tail <= _head) {
    return (_head - _tail + 1);
  }
  else {
    return (_size - (_tail - _head) + 1);
  }
}

/**
 * Returns how much space is available in the streaming buffer
 */
int32_t StreamingBuffer::availableSpace(void)
{
  return (_size - availableData());
}

/**
 * Returns true if the buffer is full
 */
bool StreamingBuffer::bufferFull()
{
  return ((_size - availableData()) == 0);
}


/**
 * Get the head index.
 * Note that the head index cannot be set.
 */
int32_t StreamingBuffer::getHead(void)
{
  return _head;
}

/**
 * Get the tail index.
 * Note that the tail index cannot be set.
 */
int32_t StreamingBuffer::getTail(void)
{
  return _tail;
}

/**
 * Read byte from buffer array
 *
 * @param address Memory address
 *
 * @return Byte read
 */
unsigned char StreamingBuffer::readByte(unsigned int address)

{
    return _storage[address];
}

/**
 * Write byte to SRAM
 *
 * @param address Memory address
 * @param data Data byte to be written
 */
void StreamingBuffer::writeByte(unsigned int address, char data)
{
  _storage[address] = data;
}
