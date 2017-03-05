
/**
  *
  * Front Panel controller for the internet radio. It eithzer reads values from the front pannel or
  * controls indicators on it.
  * This is configured as  an SPI slave. It receives instructions (as one byte) from the SPI
  * master. The behavour depends on the instruction:
  *
  *  Instruction Name          Code     Description
  *  ====================      ======   ================================================================
  *  INST_DEBUG                0x00     Used to debug
  *  INST_GET_STATION          0x01     Delivers one byte representing up 256 stations per band 
  *                                     (i.e. uses all 8 bits)
  *  INST_GET_BAND             0x02     Delivers one byte with the band. Up to 8 bands are supported
  *                                     (ie. the first 4 bits are used).
  *  INST_GET_VOL              0x03     Delivers one byte representing up to 256 levels of the
  *                                     set volume
  *  INST_GET_TONE             0x04     Delivers one byte represening 256 tone levels. 0 is most bass,
  *                                     255 is most treble
  *  INST_STATUS_OK            0x05     Used to indicate that the status is OK. No data is received or
  *                                     transmitted
  *  INST_STATUS_ERROR         0x06     Used to indicate an error status. One byte with the error code is
  *                                     transmitted.
  *     *** --- TO BE DONE --- ***
  *  INST_COLOR_RED            0x07     Used to set the intensity of the red component of the front
  *                                     panel lighting. Receives one byte with the intensity
  *                                     (i.e. from 0 to 255).
  *  INST_COLOR_GREEN          0x08     Used to set the intensity of the green component of the front
  *                                     panel lighting. Receives one byte with the intensity
  *                                     (i.e. from 0 to 255).
  *  INST_COLOR_BLUE           0x09     Used to set the intensity of the blue component of the front
  *                                     panel lighting. Receives one byte with the intensity
  *                                     (i.e. from 0 to 255).

 */

#include <SPI.h>

volatile boolean processInstruction;
volatile char instruction;



// Instructions codes need to start at 0x00 and be contiguous
const int NUMBER_INSTRUCTIONS = 10;

const byte INST_DEBUG = 0x00;
const byte INST_GET_STATION = 0x01;
const byte INST_GET_BAND = 0x02;
const byte INST_GET_VOL = 0x03;
const byte INST_GET_TONE = 0x04;
const byte INST_STATUS_OK = 0x05;
const byte INST_STATUS_ERROR = 0x06;
const byte INST_COLOR_RED = 0x07;
const byte INST_COLOR_GREEN = 0x08;
const byte INST_COLOR_BLUE = 0x09;


const byte READ = 0x00;
const byte WRITE = 0x01;
const byte NO_TRANSFER = 0x03;

// Array with the number of bytes received or transmitted for
// each instruction. The array is indexed by the instruction code
// int nBytesPerInstruction[] = {
//                               0, // INST_DEBUG = 0x00;   // Used fr debug instruction
//                               1, // INST_GET_STATION = 0x01, i.e.up to 256 stations per band
//                               1, // INST_GET_BAND = 0x02, i.e  up to 8 bands
//                               2, // INST_GET_VOL = 0x03, up to 1024 volume levels
//                               1, // INST_GET_TONE = 0x04, i.e. up to 256 tone levels
//                               0, // INST_STATUS_OK = 0x05
//                               1, // INST_STATUS_ERROR = 0x06
//                               1, // INST_COLOR_RED = 0x07;
//                               1, // INST_COLOR_GREEN = 0x08;
//                               1  // INST_COLOR_BLUE = 0x09;
//                              };

// Array to specify if the instruction is read, write or if no transfer of bytes takes place.
// The array is indexed by the instruction code.

int instructionTypes[] = {
                              NO_TRANSFER , // INST_DEBUG = 0x00;
                              READ, // INST_GET_STATION = 0x01,
                              READ, // INST_GET_BAND = 0x02
                              READ, // INST_GET_VOL = 0x03
                              READ,  // INST_GET_TONE = 0x04
                              NO_TRANSFER, // INST_STATUS_OK = 0x05
                              WRITE,   // INST_STATUS_ERROR = 0x06
                              WRITE, // INST_COLOR_RED = 0x07;
                              WRITE, // INST_COLOR_GREEN = 0x08;
                              WRITE // INST_COLOR_BLUE = 0x09;
                          };

// The buffer containing the data to be transfered indexed by the instruction
// for the buffer
byte transferBuffer[NUMBER_INSTRUCTIONS];


// The states used during SPI transfer
const byte STATE_INITIAL = 0;
const byte STATE_RECEIVE_DATA = 1;
const byte STATE_TRANSMIT_DATA = 2;

// The current state
byte volatile state = STATE_INITIAL;

// Number of bytes to transfer
int volatile nBytes = 0;

// Indicator pins for test
const int GREEN_PIN = 3;   // Needs to be a PWM pin
const int RED_PIN = 5;     // Needs to be a PWM pin
const int BLUE_PIN = 6;    // Needs to be a PWM pin
const int VOLUME_ANALOG_PIN = 0; // Needs to be an analog pin

const int STATION_TUNING_OUT_PIN = A5;
const int STATION_TUNING_IN_PIN = A4;

// Error code mapping to the red, green and blue LEDS (in that order. 
//    0  = off
//    1  = on
//    2 = blink
int errorCodeMap[][3] = {
                      {0,0,0},  // 0  =  No error
                      {0,0,1},  // 1
                      {0,0,2},  // 2
                      {0,1,0},  // 3
                      {0,1,1},  // 4
                      {0,1,2},  // 5
                      {0,2,0},  // 6
                      {0,2,1},  // 7
                      {0,2,2},  // 8
                      {1,0,0},  // 9
                      {1,0,1},  // 10
                      {1,0,2},  // 11
                      {1,1,0},  // 12
                      {1,1,1},  // 13
                      {1,1,2},  // 14
                      {1,2,0},  // 15
                      {1,2,1},  // 16
                      {1,2,2},  // 17
                      {2,0,0},  // 18
                      {2,0,1},  // 19
                      {2,0,2},  // 20
                      {2,1,0},  // 21
                      {2,1,1},  // 22
                      {2,1,2},  // 23
                      {2,2,0},  // 24
                      {2,2,1},  // 25
                      {2,2,2}   // 26
                  };

// The error codes
const int ERROR_NOT_PRESENT = 0;  // No error
const int ERROR_INVALID_INSTRUCTION = 26;  
                  
int errorCode = 0;

// Used to control the blinking of the status leds.
long lastBlinkRed = 0; 
long lastBlinkGreen = 0; 
long lastBlinkBlue = 0;
 
//DEBUG
bool volatile debugFlag = false;

void setup (void)
{
  Serial.begin (115200);   // debugging

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(STATION_TUNING_OUT_PIN, OUTPUT); 
  pinMode(STATION_TUNING_IN_PIN, OUTPUT); 
  
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // get ready for an interrupt
  processInstruction= false;

  // now turn on interrupts
  SPI.attachInterrupt();

  state = STATE_INITIAL;

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte data;
  //byte data = SPDR;  // grab byte from SPI Data Register

  switch (state) {
      case STATE_INITIAL:
        //  SPDR (and now data) contain an instruction
        //     NOTE: this does no handle the case of a instruction requiring both
        //     read and write. Need to restucture to do this, but for the
        //     SPIslave controller for the internet radio this is not required.
        data = SPDR;  // grab byte from SPI Data Register
        instruction = data;

        switch (instructionTypes[instruction]) {
            case READ:        processInstruction = false;
                              // Move the first byte of the tranmitted data into
                              // the SPDR register in preparation for the
                              // next transfer
                              SPDR = transferBuffer[instruction];
                              state = STATE_TRANSMIT_DATA;
                              break;
            case WRITE:       state = STATE_RECEIVE_DATA;
                              processInstruction = false;
                              break;
            case NO_TRANSFER: state = STATE_INITIAL;
                              processInstruction = true;
                              break;
            }
         break;
      case STATE_TRANSMIT_DATA:
        // Transfer the  data for the current instruction from the
        // transfer buffer into SPDR
        SPDR = transferBuffer[instruction];
        state = STATE_INITIAL;  // All data has been  transmitted and the master should have read it all.
        break;
      case STATE_RECEIVE_DATA:
         data = SPDR;  // grab byte from SPI Data Register
         transferBuffer[instruction] = (byte) data;
         processInstruction = true;
         state = STATE_INITIAL;
         break;
      default:
         state = STATE_INITIAL;
         break;
    }


}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop()
{
  int volume;
  int band;
  int station;
  int intensity; 

  if (debugFlag) {
     digitalWrite(BLUE_PIN, HIGH);
     delay(400);
     digitalWrite(BLUE_PIN, LOW);
     debugFlag = false;
  }


  if (processInstruction)
    {
      if (instruction == INST_DEBUG) {
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(RED_PIN, LOW);
        band = 0;
        station = 0;
      }
      else if (instruction == INST_STATUS_OK ) {
        OK();
      }
      else if (instruction == INST_STATUS_ERROR) {
        errorCode = transferBuffer[INST_STATUS_ERROR];
        Serial.print("Error code rcx:"); Serial.println(errorCode);  
      }
      else if (instruction == INST_COLOR_RED) {
        intensity = transferBuffer[INST_COLOR_RED];
        analogWrite(RED_PIN, intensity);
      }
      else if (instruction == INST_COLOR_GREEN) {
        intensity = transferBuffer[INST_COLOR_GREEN];
        analogWrite(GREEN_PIN, intensity);
      }
       else if (instruction == INST_COLOR_BLUE) {
        intensity = transferBuffer[INST_COLOR_BLUE];
        analogWrite(BLUE_PIN, intensity);
      }
      else {
        error(ERROR_INVALID_INSTRUCTION); 
      }
    processInstruction= false;
    }  // end of process instruction

    /*****  Continually read in the controls *********/ 
    
    // Process volume, reducing the range from 0 to 1023 to 0 to 255
    volume = analogRead(VOLUME_ANALOG_PIN);
    volume >>= 2;
    volume &= 0xFF;  // Just to make sure


    // Transfer the volume to the tranferBuffer in a critical section
    noInterrupts();
    transferBuffer[INST_GET_VOL] = volume;
    interrupts();

//    //DEBUG
//    if (transferBuffer[INST_GET_VOL][0] > 0) digitalWrite(BLUE_PIN, HIGH);
//    else digitalWrite(BLUE_PIN, LOW);

   // Process Station DUMMY
//   station = 10;
//   noInterrupts();
//   transferBuffer[INST_GET_STATION] = station  & 0x00FF;
//   interrupts();

   // Read in the station value
   pinMode(STATION_TUNING_IN_PIN, INPUT);
   digitalWrite(STATION_TUNING_OUT_PIN, HIGH);
   int stationVal = analogRead(STATION_TUNING_IN_PIN);

   // Clear for the next measurement
   digitalWrite(STATION_TUNING_OUT_PIN, LOW); 
   pinMode(STATION_TUNING_IN_PIN, OUTPUT);
   noInterrupts();
   transferBuffer[INST_GET_STATION] = stationVal  & 0x00FF;
   interrupts();


   //Process Band DUMMY
   band = 2;
   noInterrupts();
   transferBuffer[INST_GET_BAND] = band & 0x00FF;
   interrupts();


   // Display the eroor code
   error(errorCode); 


}  // end of loop

void error(int errorCode)
{
  const int red = 0;
  const int green = 1;
  const int blue = 2;

  Serial.print("ErrorCodeMap[");Serial.print(errorCode);Serial.print("] = ");
  
  
  Serial.print(errorCodeMap[errorCode][red]);
  Serial.print(" ");
  Serial.print(errorCodeMap[errorCode][green]);
  Serial.print(" ");
  Serial.println(errorCodeMap[errorCode][blue]);
  
  
  if (errorCodeMap[errorCode][red] == 0) digitalWrite(RED_PIN, LOW);   
  if (errorCodeMap[errorCode][red] == 1) digitalWrite(RED_PIN, HIGH);   
  if (errorCodeMap[errorCode][red] == 2) blinkRed();   

  if (errorCodeMap[errorCode][green] == 0) digitalWrite(GREEN_PIN, LOW);   
  if (errorCodeMap[errorCode][green] == 1) digitalWrite(GREEN_PIN, HIGH);   
  if (errorCodeMap[errorCode][green] == 2) blinkGreen();   

  if (errorCodeMap[errorCode][blue] == 0) digitalWrite(BLUE_PIN, LOW);   
  if (errorCodeMap[errorCode][blue] == 1) digitalWrite(BLUE_PIN, HIGH);   
  if (errorCodeMap[errorCode][blue] == 2) blinkBlue();   

}

void blinkRed() 
{
  if (millis() - lastBlinkRed > 500) {
    lastBlinkRed = millis(); 
    digitalWrite(RED_PIN, !digitalRead(RED_PIN));  //  Toggle led state
  }
}

void blinkGreen() 
{
  if (millis() - lastBlinkGreen > 500) {
    lastBlinkGreen = millis(); 
    digitalWrite(GREEN_PIN, !digitalRead(GREEN_PIN));  //  Toggle led state
  }
}

void blinkBlue() 
{
  if (millis() - lastBlinkBlue > 500) {
    lastBlinkBlue = millis(); 
    digitalWrite(BLUE_PIN, !digitalRead(BLUE_PIN));  //  Toggle led state
  }
}


void OK()
{
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

