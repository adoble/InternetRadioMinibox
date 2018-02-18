
/**
  *
  * Front Panel controller for the internet radio. It either reads values from the front panel or
  * controls indicators on it.
  * This is configured as an SPI slave. It receives instructions (as one byte) from the SPI
  * master. The behavour depends on the instruction:
  *
  *  Instruction Name          Code     Description
  *  ====================      ======   ================================================================
  *  INST_NULL                 0x00     Null instruction
  *  INST_GET_STATION          0x01     Delivers one byte representing up 10 stations.
  *  INST_GET_VOL              0x02     Delivers one byte representing the volume. This has a value from
  *                                     1 to 31.
  *  INST_STATUS_OK            0x03     Used to indicate that the status is OK. No data is received or
  *                                     transmitted
  *  INST_STATUS_ERROR         0x04     Used to indicate an error status. One byte with the error code is
  *                                     transmitted. Value is from 0 (= OK) to 8.
  *  INST_GET_CHANGES          0x05     Gets a byte from the  FPController whose bits indicated what has changed.
  *                                     Once read the FPController sets ALL bits back to zero.
  *  INST_RESET_CHANGES        0x06     Resets the change bits. This should be used after ALL changes have
  *                                     been processed.
  *  INST_VIRTUAL_PIN         0x07      Drives the specified virtual pin used for SPI chip select pin low.
  *                                     See "Values taken by INST_VIRTUAL_PIN".
  *
  *  Change Status Bits (from INST_GET_CHANGES)
  *  Bit Name                 Code      Description
  *  ==================       =======   ===============================================================
  *  CHANGED_VOL              0x01      The volume has changed
  *  CHANGED_STATION          0x02      A new station has been selected
  *
  *  Values taken by INST_VIRTUAL_PIN
  *  Bit           Value           Description
  *  =======       =============   ===============================================================
  *  7             1               If the specified virtual pin should be driven HIGH.
  *                0               If the specified virtual pin should be driven LOW
  *  0-6           Virtual pin     This has a range from 1 - 63
  *  Note: If this byte is 0 then this is acts as a reset.
  */

#include <SPI.h>
#include <EEPROM.h>

// SPI instruction handling from the interrupts
volatile boolean processInstruction;
volatile char instruction;

/// The number of stations that can be selected
const int NUMBER_STATIONS = 10;

// Instructions codes need to start at 0x00 and be contiguous
const int NUMBER_INSTRUCTIONS = 8;

const byte INST_NULL              = 0x00;
const byte INST_GET_STATION       = 0x01;
const byte INST_GET_VOL           = 0x02;
const byte INST_STATUS_OK         = 0x03;
const byte INST_STATUS_ERROR      = 0x04;
const byte INST_GET_CHANGES       = 0x05;
const byte INST_RESET_CHANGES     = 0x06;
const byte INST_VIRTUAL_PIN      = 0x07;

// Changed status bits
const byte CHANGED_VOL_BIT     = 0;     // The volume has changed
const byte CHANGED_STATION_BIT = 1;     // A new station has been selected

// Values for the instruction INST_VIRTUAL_PIN.
const byte EXP_PIN_1 = 1;  // Mapped to SPI_XCS
const byte EXP_PIN_2 = 2;  // Mapped to SPI_RAM_CS

const byte READ = 0x00;
const byte WRITE = 0x01;
const byte NO_TRANSFER = 0x03;

// Array to specify if the instruction is read (read from the controller),
// write (writtten to the controller)
// or if no transfer of bytes takes place.
// The array is indexed by the instruction code.
int instructionTypes[] = {
                              NO_TRANSFER, // INST_NULL = 0x00;
                              READ,         // INST_GET_STATION = 0x01,
                              READ,         // INST_GET_VOL = 0x02
                              NO_TRANSFER,  // INST_STATUS_OK = 0x03
                              WRITE,        // INST_STATUS_ERROR = 0x04
                              READ,         // INST_GET_CHANGES  = 0x05
                              NO_TRANSFER,  // INST_RESET_CHANGES = 0x06
                              WRITE         // INST_VIRTUAL_PIN = 0x07;
                          };




// The buffer containing the data to be transfered indexed by the instruction
// for the buffer
byte transferBuffer[NUMBER_INSTRUCTIONS];

// The states used during SPI transfer
const byte SPI_STATE_INITIAL = 0;
const byte SPI_STATE_RECEIVE_DATA = 1;
const byte SPI_STATE_TRANSMIT_DATA = 2;

// The current state
byte volatile spiState = SPI_STATE_INITIAL;  // Changedin the SPI interrup routine

// Number of bytes transfered
int volatile nBytes = 0;

// Pins
const int GREEN_PIN = 3;    // Green LED
const int RED_PIN = 5;      // Red LED
const int encoderPinA = 2;  // Interrupt 0 pin for rotary encoder (volume control)
const int encoderPinB = 4;  // Other pin for for rotary encoder (volume control)

const int STATION_TUNING_OUT_PIN = A5;
const int STATION_TUNING_IN_PIN = A4;


// Chip select pins
const int SPI_XCS = A0;           // The chip select for the VS1053 codec
const int SPI_RAM_CS = A1;        // The chip select for the RAM (the ring buffer)

// Interrupt line to external processor. Pulsed low when a change to the controls occurs.
const int FP_CHANGE_INTR = A2;

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
                      {2,1},  // 7  = Invalid expanded output pin specified.
                      {2,2}   // 8  = Invalid instruction
                  };


// The error codes
const int ERROR_NOT_PRESENT = 0;  // No error
const int ERROR_INVALID_INSTRUCTION = 8;

int errorCode = 0;

// Used to control the blinking of the status leds.
long lastBlinkRed = 0;
long lastBlinkGreen = 0;

/* ---- Encoder (volume control) position --- */
// Limit the range
const int MAX_POS = 31;
const int MIN_POS = 1;  // Not to think that it is switched off
byte oldVolume = -1;
byte volume;
volatile int encoderPos = MIN_POS; // variables changed within interrupts are volatile
const byte DEFAULT_ENCODER_POS = (MAX_POS - MIN_POS)/ 2; // The default position value for the rotary encoder, i.e. half way


/* ----- Sampling of the variable cap, to smooth out the (noisy) measurements ---- */
const int MAX_SAMPLES = 30; // Empirically determined
int capSamples[MAX_SAMPLES];
int sampleIndex = 0;
const int CAP_NOISE_TOLERANCE = 10;   // Empirically determined

const int MIN_CAP_VALUE = 50;    // Empirically determined
const int MAX_CAP_VALUE = 200;   // Empirically determined

// EEPROM Settings. Note the EEPROM is used to store the last volume setting
const byte EEPROM_ID = 0x99; // Used to identify if valid data in EEPROM
const int  ID_ADDR = 0;    // The address used to store the EEPROM ID
const int  ENCODER_POS_ADDR = 1;    // The address used to store the last encoder position

int station;
int oldStation = -1;

byte expandedOutputPin;

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

  pinMode(SPI_XCS, OUTPUT);
  pinMode(SPI_RAM_CS, OUTPUT);
  digitalWrite(SPI_XCS, HIGH);
  digitalWrite(SPI_RAM_CS, HIGH);


  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // get ready for an interrupt
  processInstruction= false;

  // now turn on interrupts
  SPI.attachInterrupt();
  attachInterrupt(0, doEncoder, RISING); // encoder pin on interrupt 0 (pin 2) for roatay encoder (volume control)

  // Set up the SPI state variable
  spiState = SPI_STATE_INITIAL;

  // Read the old encoder position from the EEPROM
  byte id = EEPROM.read(ID_ADDR);
  if (id == EEPROM_ID) {
    // Apparently valid data has been written to the EEPROM
    encoderPos = EEPROM.read(ENCODER_POS_ADDR);
    //However, if not valid then set to the default value;
    if (encoderPos > MAX_POS || encoderPos < MIN_POS) encoderPos = DEFAULT_ENCODER_POS;
  }
  else {
    // The ID has not been found so write the default data to the EEPROM
    EEPROM.write(ID_ADDR, EEPROM_ID);
    EEPROM.write(ENCODER_POS_ADDR, DEFAULT_ENCODER_POS);
    encoderPos = DEFAULT_ENCODER_POS;
  }
  // Move the volume into the transfer buffer
  transferBuffer[INST_GET_VOL] = toVolume(encoderPos);

  // Set that there has been a change so that the initial values are read
  transferBuffer[INST_GET_CHANGES] = 0;
  bitSet(transferBuffer[INST_GET_CHANGES], CHANGED_VOL_BIT);
  bitSet(transferBuffer[INST_GET_CHANGES], CHANGED_STATION_BIT);


}  // end of setup


/**
 * SPI interrupt routine
 *
 * Implemented as a simple state machine to reading in the instructions
 * and any data associated with them.
 */
ISR (SPI_STC_vect)
{
  byte data;

  switch (spiState) {
      case SPI_STATE_INITIAL:
        //  SPDR  contain an instruction
        //     NOTE: this does no handle the case of a instruction requiring both
        //     read and write. Need to restucture to do this, but for the
        //     SPIslave controller for the internet radio this is not required.
        data = SPDR;  // grab byte from SPI Data Register
        instruction = data;

        // If invalid instruction then ignore
        if (instruction < 0 || instruction >= NUMBER_INSTRUCTIONS)  return;

        switch (instructionTypes[instruction]) {
            case READ:        processInstruction = false;
                              // Move the first byte of the tranmitted data into
                              // the SPDR register in preparation for the
                              // next transfer
                              SPDR = transferBuffer[instruction];
                              spiState = SPI_STATE_TRANSMIT_DATA;
                              break;
            case WRITE:       spiState = SPI_STATE_RECEIVE_DATA;
                              processInstruction = false;
                              break;
            case NO_TRANSFER: spiState = SPI_STATE_INITIAL;
                              processInstruction = true;
                              break;
            }
         break;
      case SPI_STATE_TRANSMIT_DATA:
        // Transfer the  data for the current instruction from the
        // transfer buffer into SPDR
        SPDR = transferBuffer[instruction];
        spiState = SPI_STATE_INITIAL;  // All data has been  transmitted and the master should have read it all.
        break;
      case SPI_STATE_RECEIVE_DATA:
         data = SPDR;  // grab byte from SPI Data Register
         transferBuffer[instruction] = (byte) data;
         processInstruction = true;
         spiState = SPI_STATE_INITIAL;
         break;
      default:
         spiState = SPI_STATE_INITIAL;
         break;
    }
}  // end of interrupt routine SPI_STC_vect

/**
 * Rotary control interrupt routine.
 *
 * The position is limited to MIN_POS to MAX_POS.
 * Note: the direction of count has been chosen due to the orientation
 * of the rotary encoder in the Minibox.
 */
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

  int lowest, highest;   // Range of noisy measurements of the variable cap.
Serial.print("INST:"); Serial.println(instruction, OCT);
   if (processInstruction)
    {
      if (instruction == INST_NULL) {
        errorCode = 0;
        station = 0;
      }
      else if (instruction == INST_RESET_CHANGES) {
        transferBuffer[INST_GET_CHANGES] = 0;
      }
      else if (instruction == INST_STATUS_OK ) {
        OK();
      }
      else if (instruction == INST_STATUS_ERROR) {
        errorCode = transferBuffer[INST_STATUS_ERROR];
        Serial.print("Error code rcx:"); Serial.println(errorCode);
      }
      else if (instruction == INST_VIRTUAL_PIN) {
        expandedOutputPin = transferBuffer[INST_VIRTUAL_PIN] & 0x177;
        // Map to the actual pins. Only 2 pins so just hardcode it.
        int expandedPinValue = (transferBuffer[INST_VIRTUAL_PIN] >> 7) ? HIGH : LOW;

        Serial.print("expandedOutputPin ");
        Serial.print(expandedOutputPin);
        Serial.print(" = ");
        Serial.println(expandedPinValue, OCT);
        
        if (expandedOutputPin ==  EXP_PIN_1) digitalWrite(SPI_XCS, expandedPinValue);
        if (expandedOutputPin ==  EXP_PIN_2) digitalWrite(SPI_RAM_CS, expandedPinValue);
        if (expandedOutputPin == 0) {
          // Reset. Formally this should happen when  the value is zero,
          // but extend this for any other value.
          digitalWrite(SPI_XCS, HIGH);
          digitalWrite(SPI_RAM_CS, HIGH);;
        }
      }

    processInstruction= false;
    }  // end of process instruction

    /*****  Continually read in the controls *********/

    // Process volume in a critical section
    noInterrupts();
    volume = toVolume(encoderPos);
    if (volume != oldVolume) {
      oldVolume = volume;
      bitSet(transferBuffer[INST_GET_CHANGES], CHANGED_VOL_BIT);
      // Transfer the volume to the transferBuffer
      transferBuffer[INST_GET_VOL] = volume;
    }
    interrupts();


    // Now store the new position of the rotary encoder in the EEPROM
    EEPROM.write(ENCODER_POS_ADDR, encoderPos);

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
      station = map(highest, MIN_CAP_VALUE, MAX_CAP_VALUE, 0, NUMBER_STATIONS);
      if (station != oldStation) {
        oldStation = station;
        noInterrupts();
        bitSet(transferBuffer[INST_GET_CHANGES], CHANGED_STATION_BIT);
        transferBuffer[INST_GET_STATION] = station & 0x00FF;
        interrupts();
      }

    }
    sampleIndex = 0;
   }
   // Clear for the next measurement
   digitalWrite(STATION_TUNING_OUT_PIN, LOW);
   pinMode(STATION_TUNING_IN_PIN, OUTPUT);

   // If a change has occured then pulse the front panel interrupt signal
   // to indicate that a change has talen replace
   // Now pulse the front panel interrupt signal
   // to indicate that a change has talen replace
   if (transferBuffer[INST_GET_CHANGES] & (1<<CHANGED_VOL_BIT)) {
     digitalWrite(FP_CHANGE_INTR, LOW);
     delayMicroseconds(10);  //TODO from literature
     digitalWrite(FP_CHANGE_INTR, HIGH);
   }

   // Display the error code
   error(errorCode);

}  // end of loop

void error(int errorCode)
{
  const int red = 0;
  const int green = 1;

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

/* Translates and encoder position to a volume */
byte toVolume(byte anEncoderPos) {
 return anEncoderPos;   // Simple translation
}
