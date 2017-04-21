
/**
  *
  * Front Panel controller for the internet radio. It eithzer reads values from the front pannel or
  * controls indicators on it.
  * This is configured as  an SPI slave. It receives instructions (as one byte) from the SPI
  * master. The behavour depends on the instruction:
  *
  *  Instruction Name          Code     Description
  *  ====================      ======   ================================================================
  *  INST_NULL                0x00      Null instruction
  *  INST_GET_STATION          0x01     Delivers one byte representing up 256 stations
  *                                     (i.e. uses all 8 bits), altough this is restricted to 10 stations
  *                                     due to inaccurcies in reading the variable capacitance value.
  *  INST_GET_VOL              0x03     Delivers one byte representing up to 256 levels of the
  *                                     set volume
  *  INST_STATUS_OK            0x05     Used to indicate that the status is OK. No data is received or
  *                                     transmitted
  *  INST_STATUS_ERROR         0x06     Used to indicate an error status. One byte with the error code is
  *                                     transmitted. Value is from 0 (=OK) to 8.
 */

#include <SPI.h>

volatile boolean processInstruction;
volatile char instruction;
/// The number of stations that can be selected
const int NUMBER_STATIONS = 10;

// Instructions codes need to start at 0x00 and be contiguous
const int NUMBER_INSTRUCTIONS = 5;

const byte INST_NULL = 0x00;
const byte INST_GET_STATION = 0x01;
const byte INST_GET_VOL = 0x02;
const byte INST_STATUS_OK = 0x03;
const byte INST_STATUS_ERROR = 0x04;

const byte READ = 0x00;
const byte WRITE = 0x01;
const byte NO_TRANSFER = 0x03;

// Array to specify if the instruction is read, write or if no transfer of bytes takes place.
// The array is indexed by the instruction code.
int instructionTypes[] = {
                              NO_TRANSFER , // INST_NULL = 0x00;
                              READ, // INST_GET_STATION = 0x01,
                              READ, // INST_GET_VOL = 0x03
                              NO_TRANSFER, // INST_STATUS_OK = 0x05
                              WRITE    // INST_STATUS_ERROR = 0x06
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

// Pins
const int GREEN_PIN = 3;    // Green LED
const int RED_PIN = 5;      // Red LED
const int encoderPinA = 2;  // Interrupt 0 pin for rotary encoder (volume control)
const int encoderPinB = 4;  // Other pin for for rotary encoder (volume control)

const int STATION_TUNING_OUT_PIN = A5;
const int STATION_TUNING_IN_PIN = A4;

// Error code mapping to the red, green LEDS in that order.
//    0  = off
//    1  = on
//    2 = blink
int errorCodeMap[][3] = {
                      {0,0},  // 0  =  No error
                      {0,1},  // 1
                      {0,2},  // 2
                      {1,0},  // 3
                      {1,1},  // 4
                      {1,2},  // 5
                      {2,0},  // 6
                      {2,1},  // 7
                      {2,2}   // 8
                  };


// The error codes
const int ERROR_NOT_PRESENT = 0;  // No error
const int ERROR_INVALID_INSTRUCTION = 26;

int errorCode = 0;

// Used to control the blinking of the status leds.
long lastBlinkRed = 0;
long lastBlinkGreen = 0;

/* ---- Encoder (volume control) position --- */
// Limit the range
const int MAX_POS = 31;
const int MIN_POS = 1;  // Not to think that it is switched off
int oldPos;
int volume;
volatile int encoderPos = MIN_POS; // variables changed within interrupts are volatile


/* ----- Sampling of the variable cap, to smooth out the measurements ---- */
const int MAX_SAMPLES = 30; // Empirically determined
int capSamples[MAX_SAMPLES];
int sampleIndex = 0;
const int CAP_NOISE_TOLERANCE = 10;   // Empirically determined

const int MIN_CAP_VALUE = 50;    // Empirically determined
const int MAX_CAP_VALUE = 200;   // Empirically determined


//DEBUG
bool volatile debugFlag = false;

void setup (void)
{
  Serial.begin (115200);   // debugging

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  pinMode(STATION_TUNING_OUT_PIN, OUTPUT);
  pinMode(STATION_TUNING_IN_PIN, OUTPUT);

  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // get ready for an interrupt
  processInstruction= false;

  // now turn on interrupts
  SPI.attachInterrupt();
  attachInterrupt(0, doEncoder, RISING); // encoder pin on interrupt 0 (pin 2) for roatay encoder (volume control)

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

// Rotary control interrupt routine.
// The position is limited to MIN_POS to MAX_POS.
// Note: the driection of count has been chosen due to the orientation
// of the roraty encoder in the Minibox.
void doEncoder()
{
  if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
    if (encoderPos > MIN_POS) encoderPos--;    //count down if encoder pins are different
  }
  else {
    if (encoderPos <  MAX_POS) encoderPos++;    // count up if both encoder pins are the same
  }
} // end of interrupt routine doEncoder

// main loop - wait for flag set in interrupt routine
void loop()
{
  int station;
  int lowest, highest;   // Range of noisy measurements of the variable cap.

  if (debugFlag) {
     // ... TODO
  }


  if (processInstruction)
    {
      if (instruction == INST_NULL) {
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(RED_PIN, LOW);
        station = 0;
      }
      else if (instruction == INST_STATUS_OK ) {
        OK();
      }
      else if (instruction == INST_STATUS_ERROR) {
        errorCode = transferBuffer[INST_STATUS_ERROR];
        Serial.print("Error code rcx:"); Serial.println(errorCode);
      }
      else {
        error(ERROR_INVALID_INSTRUCTION);
      }
    processInstruction= false;
    }  // end of process instruction

    /*****  Continually read in the controls *********/

    // Process volume in a critical section
    noInterrupts();
    volume = encoderPos;
    if(volume != oldPos)
    {
      oldPos = volume;
      // Transfer the volume to the transferBuffer
      transferBuffer[INST_GET_VOL] = volume;
    }
    interrupts();

   // Read in the station value
   pinMode(STATION_TUNING_IN_PIN, INPUT);
   digitalWrite(STATION_TUNING_OUT_PIN, HIGH);
   int stationVal = analogRead(STATION_TUNING_IN_PIN);

   capSamples[sampleIndex++] = stationVal/4;
   if (sampleIndex == MAX_SAMPLES) {
    //Check that the samples are stable
    lowest = 256;
    highest = 0;
    for (int i=0; i < MAX_SAMPLES; i++) {
      lowest = (capSamples[i] < lowest) ?  capSamples[i]: lowest;
      highest = (capSamples[i] > highest) ?  capSamples[i]: highest;
    }
    if ( (highest - lowest) < CAP_NOISE_TOLERANCE) {
      noInterrupts();
      station = map(highest, MIN_CAP_VALUE, MAX_CAP_VALUE, 0, NUMBER_STATIONS);
      transferBuffer[INST_GET_STATION] = station & 0x00FF;
      interrupts();
    }
    sampleIndex = 0;
   }
   // Clear for the next measurement
   digitalWrite(STATION_TUNING_OUT_PIN, LOW);
   pinMode(STATION_TUNING_IN_PIN, OUTPUT);


   // Display the error code
   error(errorCode);

}  // end of loop

void error(int errorCode)
{
  const int red = 0;
  const int green = 1;

  Serial.print("ErrorCodeMap[");Serial.print(errorCode);Serial.print("] = ");


  Serial.print(errorCodeMap[errorCode][red]);
  Serial.print(" ");
  Serial.println(errorCodeMap[errorCode][green]);


  if (errorCodeMap[errorCode][red] == 0) digitalWrite(RED_PIN, LOW);
  if (errorCodeMap[errorCode][red] == 1) digitalWrite(RED_PIN, HIGH);
  if (errorCodeMap[errorCode][red] == 2) blinkRed();

  if (errorCodeMap[errorCode][green] == 0) digitalWrite(GREEN_PIN, LOW);
  if (errorCodeMap[errorCode][green] == 1) digitalWrite(GREEN_PIN, HIGH);
  if (errorCodeMap[errorCode][green] == 2) blinkGreen();
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


void OK()
{
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}
