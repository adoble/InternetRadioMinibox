/*
 *
 * A ring buffer that is implemented to use an SPI based RAM such as the 23K256.
 * It uses the functions put() and get() to place bytes in the
 * buffer and retreive them respectively.
 *
 * Author: Andrew Doble
 * Creation date: 07.08.2016
 * Last update  : 21.08.2016
 */

#ifndef _SPIRingBuffer_H
#define _SPIRingBuffer_H

#include <Arduino.h>
#include <SPI.h>

// SRAM instructions
#define RDSR  0x05
#define WRSR  0x01
#define READ  0x03
#define WRITE 0x02

 // SRAM Hold disable bit.
#define HOLD_DISABLE 1

// SRAM byte mode
#define BYTE_MODE (0x00 | HOLD_DISABLE)

 // Clock speed 16 MHz
 #define RAMCLK 16000000

 // SPI setting for the RAM chip
 #define RAM_SPI_SETTING SPISettings(RAMCLK,  MSBFIRST, SPI_MODE0)

class SPIRingBuffer
{
  private:
    // Chip select pin
    uint8_t csPin;

    // SPI interface
    #if defined(ARDUINO_SAMD_ZERO)
      // Required for Serial on Zero based boards
      SPIClass spi = SPIClass(&sercom4, 22, 24, 23, SPI_PAD_2_SCK_3, SERCOM_RX_PAD_0);
    #else
      SPIClass spi;
    #endif


    // Operation mode
    unsigned char operationMode;

    // Head address where the last data was added or -1 of no data added
    int32_t head = -1;
    // Tail address where the last unread data is or -1 if no data has been read.
    int32_t tail = -1 ;

    /**
     * Enable chip via CS pin
     *
     * @param state True to select. False to unselect
     */
     void chipSelect(bool state);

    /**
     * Set operation mode
     *
     * @param mode Operation mode
     */
    void setMode(char mode);

  public:

    // The length of the ring buffer (in bytes) is the size of the SPI RAM, i.e. 256Kbit = 32KByte
    const int32_t RING_BUFFER_LENGTH = 32768;

    /**
     * Class constructor
     *
     * param chipSelectPin The digital pin used a SPI slave select
     */
    SPIRingBuffer(uint8_t chipSelectPin);

    /**
     *  Initialize the communication and indexes
     *
     */
     void begin(void);

     /**
      *  Initialize the indexes.
      *  Existing data will be overwritten started at address 0.
      */
    void reset(void);

    /**
     * Puts a byte into the ring buffer in the SPI RAM.
     * Returns -1 if the buffer is full. Else returns
     * the added data (which as a size of one byte).
     */
     int32_t put(uint8_t);

     /**
      * Gets a byte from the ring buffer in SPI RAM.
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

    /**
     * Read a byte from the SRAM
     *
     * param address Memory address to read the byte from
     *
     * return Byte read
     */
    unsigned char readByte(unsigned int address);

    /**
     * Write byte to the SRAM
     *
     * param address The memory address
     * param data Data byte to be written
     */
    void writeByte(unsigned int address, char data);
};

#endif
