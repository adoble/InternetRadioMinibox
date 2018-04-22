/**
  * SpiRAMRingBufferTest
  *
  *    This sketch tests the SPI ring buffer library
  *
  */

#include <SPI.h>
#include "SPIRingBuffer.h"
#include "wiring_private.h"


const char programName[] = "SpiRAMRingBuffer - M0 Version";

// Pins
const int CS = A4; // Chip select pin = A4

SPIClass spi(&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

SPIRingBuffer ringBuffer(spi, CS);   // TODO Try setting spi in begin!!!


// Function protoypes for testing
void isEqual(String, int32_t, int32_t);
void isNotEqual(String, int32_t, int32_t);
void fail(String s);
void printState(String s);


// Test states
const int INIT     = 0;
const int TEST_1   = 1;
const int TEST_2_1 = 2;
const int TEST_2_2 = 3;
const int TEST_3_1 = 4;
const int TEST_3_2 = 5;
const int TEST_4_1 = 7;
const int TEST_4_2 = 8;
const int TEST_4_3 = 9;
const int TEST_4_4 = 10;
const int TEST_4_5 = 11;
const int NO_MORE_TESTS = 100;
int testState;

// Test variables
const int MAX_ERRORS = 40;
int nErrors = 0;
byte data = 0;
int32_t res;
byte testData[] = {0x21u, 0x22u, 0x23u, 0x24u, 0x25u};  // for test 2
byte expectedData;
int i = 0;
uint32_t half = ringBuffer.RING_BUFFER_LENGTH / 2;
String msg;

// Use SerialUSB for the M0
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif



void setup() {
  
  Serial.begin(115200);
  

  //spi.begin();   // DONT NEED TO DO THIS
  // Assign pins 11, 12, 13 to SERCOM functionality
 // pinPeripheral(11, PIO_SERCOM);
 // pinPeripheral(12, PIO_SERCOM);
 // pinPeripheral(13, PIO_SERCOM);


  // So we know what version we are running
  Serial.println(programName);
  Serial.println();


  // Raise high any other pins being used for chip select to other systems
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(6, OUTPUT);

  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A5, HIGH);
  digitalWrite(6, HIGH);


  delay(10);



  // Set up the ring buffer RAM
  ringBuffer.begin();

  // Set the first test
  //testState = TEST_1;
  testState = INIT; i = 0;


}


void loop() {

  switch (testState) {
    case INIT:
      if (i==0) Serial.println("Starting initalization");
      isNotEqual("Init put", ringBuffer.put(0), -1);
      i++;
      if (i == ringBuffer.RING_BUFFER_LENGTH) {
        i = 0;
        testState = TEST_1;
        Serial.println("Finished initalization");
      }

      break;
    case TEST_1:

      // TEST 1 - Simple test
      Serial.println("Start TEST 1 --------------");

      ringBuffer.reset();
      printState("put() pre");
      isNotEqual("Test 1 first put", ringBuffer.put(0x11), -1);
      printState("put() post");


      printState("put() pre");
      //isNotEqual("Test 1 second put", ringBuffer.put(0x12), -1);
      isNotEqual("Test 1 second put", ringBuffer.put(0x012), -1);
      printState("put() post");

      isEqual("Test 1 Available data", ringBuffer.availableData(), 2);
      isEqual("Test 1 Available space", ringBuffer.availableSpace(), ringBuffer.RING_BUFFER_LENGTH - 2);


      res = ringBuffer.get();
      isEqual("Test 1 Read first value ", res, 0x11);
      res = ringBuffer.get();
      isEqual("Test 1 second value", res, 0x12);

      isEqual("Test 1 Available data", ringBuffer.availableData(), 0);
      isEqual("Test 1 Available space", ringBuffer.availableSpace(), ringBuffer.RING_BUFFER_LENGTH);
      Serial.println("End TEST 1 --------------");
      testState = TEST_2_1;
      Serial.println("Start TEST 2 --------------");
      ringBuffer.reset();
      i = 0;
      break;

    case TEST_2_1:
      // TEST 2 Add some data and read it
      // 1. load the data
      Serial.print("TEST 2 put:"); Serial.println(i);
      printState("put() pre");
      res = ringBuffer.put(testData[i++]);
      printState("put() post");
      isNotEqual("TEST 2 put", res, -1);
      if (i == sizeof(testData)) {
        i = 0;
        testState = TEST_2_2;
      }
      break;
    case TEST_2_2:
      // TEST 2 2. read the data

      Serial.print("TEST 2 get:"); Serial.println(i);
      printState("get() pre");
      res = ringBuffer.get();
      printState("get() post");
      if ( res == -1) fail("TEST 2 get returns -1");
      isEqual("TEST 2 full buffer test", (byte)res, testData[i++]);

      if ( i == sizeof(testData)) {
        isEqual("TEST 2 Fill buffer test: available data", ringBuffer.availableData(), 0);
        isEqual("TEST 2 Fill buffer test: available space", ringBuffer.availableSpace(), ringBuffer.RING_BUFFER_LENGTH);
        Serial.println("End TEST 2 --------------"); Serial.println();
        testState = TEST_3_1;
        Serial.println("Start TEST 3 --------------");
        ringBuffer.reset();
        i = 0;
      }
      break;

    case TEST_3_1:     // TEST 3 - More data to test using the SPI RAM
    // 1. half fill the memory

    msg = "TEST 3 put at address ";
    msg.concat(i);
    data = i % 256;
    isNotEqual(msg, ringBuffer.put(data), -1);
    i++;

    if (i == half)  { // have half filled the memory
       printState("State at end of  TEST 3.1");

       testState = TEST_3_2;
       i = 0;
    }
    break;

  case TEST_3_2:
    // 2. Now read and check that all is ok

    msg = "TEST 3 get at address ";
    msg.concat(i);
    expectedData = i % 256;
    isEqual(msg, ringBuffer.get(), expectedData);
    i++;
    if (i == half) {
      Serial.println("End TEST 3 --------------"); Serial.println();
      testState = TEST_4_1;
      i = 0;
      Serial.println("Start TEST 4 --------------");
    }
    break;

  case TEST_4_1:   // TEST 4 SPI RAM Wraparound test
    //  1. Fill the memory with 0x0A (just a value we can check)
    msg = "TEST 4 put at address ";
    msg.concat(i);
    isNotEqual(msg, ringBuffer.put(0x0A), -1);
    i++;

    if (i == ringBuffer.RING_BUFFER_LENGTH) {
      if (!ringBuffer.bufferFull()) {
        fail("TEST 4.1 Buffer full but not indicated");
      }
      printState("End test 4.1, after filling up the memory");
      testState = TEST_4_2;
      i = 0;
    }
    break;

  case TEST_4_2:
    // 2. Now read 100 bytes
    isNotEqual("TEST 4 problem with reading 100 byte", ringBuffer.get(), -1);
    i++;

    if (i == 100) {
       printState("TEST 4.2 after reading 100 bytes");
       testState = TEST_4_3;
       i = 0;
    }
    break;

    case TEST_4_3:
    // 3. Write in another 50 bytes whch should wrap around to the beginning
    if (i == 0) printState("TEST 4 before writing 50 bytes");

    msg = "TEST 4.3 put one of 50 bytes at address ";
    msg.concat(i);
    data = i;
    isNotEqual(msg, ringBuffer.put(data), -1);
    i++;

    if (i == 50)  {
      printState("TEST 4.3 after writing 50 bytes");
      i = 0;
      testState = TEST_4_4;
    }
    break;

  case TEST_4_4:
    // 4. Check that we have 256K-100 bytes with 0x0A ...
    isEqual("TEST 4.4 read problem", ringBuffer.get(), (int)0x0A);
    i++;

    if (i == (ringBuffer.RING_BUFFER_LENGTH - 100)) {
      Serial.println("Finished TEST 4.4");
      testState = TEST_4_5;
      i = 0;
    }
    break;

  case TEST_4_5:
    // 5. ... and then 50 bytes with an index starting at 0
    isEqual("TEST 4.5 read problem with the 50 bytes", ringBuffer.get(), i);
    i++;

    if (i == 50) {
        Serial.println("END TEST 4 --------------");
        testState = NO_MORE_TESTS;
        i = 0;
        Serial.println();
       Serial.println("------------ ALL TESTS FINISHED ---------------");

    }
    break;

  case NO_MORE_TESTS:
    delay(50000);
    break;

  }  // -- switch


}   // -- loop()

// Test functions

void isEqual(String s, int32_t actual, int32_t expected)
{
  if (actual != expected) {
    Serial.print("FAIL ");
    Serial.print(s);
    Serial.print(" expected ");
    Serial.print(expected);
    Serial.print(" but got ");
    Serial.println(actual);
  }

}

void isNotEqual(String s, int32_t actual, int32_t expected)
{
  if (actual == expected) {
    Serial.print("FAIL ");
    Serial.print(s);
    Serial.print(" did not expect ");
    Serial.print(expected);
    Serial.print(" but got ");
    Serial.println(actual);
  }
}

void printState(String s)
{
  Serial.print("     STATE ");
  Serial.print(s);
  Serial.print(" h:");
  Serial.print(ringBuffer.getHead());
  Serial.print(" t:");
  Serial.println(ringBuffer.getTail());
}

void fail(String s) {
  Serial.print("FAIL ");
  Serial.println(s);
}
