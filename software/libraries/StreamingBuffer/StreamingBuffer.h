/*
 * A straeming (ring) buffer that is implemented using internal RAM
 * It uses the functions put() and get() to place bytes in the
 * buffer and retreive them respectively.
 *
 * Author: Andrew Doble
 * Creation date: 19.05.2018
 *
 */

#ifndef _StreamingBuffer_H
#define _StreamingBuffer_H

#include <Arduino.h>

class StreamingBuffer
{

  public:

    StreamingBuffer(uint8_t *storage, int size);


    /**
     *  Initialize the indexes.
     *  Existing data will be overwritten started at address 0.
     */
     void begin();


    /**
     * Puts a byte into the ring buffer.
     * Returns -1 if the buffer is full. Else returns
     * the added data (which has a size of one byte).
     */
     int32_t put(uint8_t);

     /**
      * Gets a byte from the ring buffer.
      * If the buffer is empty, returns -1.
      */
     int32_t get();

     /**
      * Returns how much data is in the ring buffer
      *
      */
     int32_t availableData();

    /**
     * Returns how much space is available in the streaming buffer
     *
     */
     int32_t availableSpace();

     /**
      * Returns true if the buffer is full
      */
     bool bufferFull();

    /**
     * Get the head index. This is mainly used for debugging purposes.
     * Note that the head index cannot be set.
     */
     int32_t getHead();

    /**
     * Get the tail index. This is mainly used for debugging purposes.
     * Note that the tail index cannot be set.
     */
     int32_t getTail();

   private:
     // Head address where the last data was added or -1 of no data added
     int32_t _head = -1;
     // Tail address where the last unread data is or -1 if no data has been read.
     int32_t _tail = -1 ;
     // The length of the data stroge array in bytes
     int _size;
     // The storage
     uint8_t *_storage;

     /**
     * Read a byte from the array
     *
     * param address Memory address to read the byte from
     *
     * return Byte read
     */
     unsigned char readByte(unsigned int address);

     /**
     * Write byte to the array
     *
     * param address The memory address
     * param data Data byte to be written
     */
     void writeByte(unsigned int address, char data);

};

#endif
