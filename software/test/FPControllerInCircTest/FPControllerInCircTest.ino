/*
 * FPController in circuit test

 */

#include <SoftwareSerial.h>   // We need this even if we're not using a SoftwareSerial object
                              // Due to the way the Arduino IDE compiles
#include <SPI.h>

// TODO put all this into a header file
const byte INST_NULL = 0x00;
const byte INST_GET_STATION = 0x01;
const byte INST_GET_VOL = 0x02;
const byte INST_STATUS_OK = 0x03;
const byte INST_STATUS_ERROR = 0x04;
const byte INST_GET_CHANGES  = 0x05;
const byte INST_RESET_CHANGES = 0x06;

// Changed status bits
const byte CHANGED_VOL_BIT     = 0;     // The volume has changed
const byte CHANGED_STATION_BIT = 1;     // A new station has been selected


// set GPIO 0 as the slave select (CS).
const int slaveSelectPin = 0;

long controlSampleTime = 0;
long currentTime;

void setup() {
  Serial.begin(115200);

  Serial.println("Starting FP Controller test.");

  // set the slaveSelectPin as an output:
  pinMode(slaveSelectPin, OUTPUT);


  SPI.begin();

  FP_begin(); // To reset THIS IS IMPORTANT --> Make it a reset function the master

  currentTime = millis();
}

void loop() {
  byte changes;

  currentTime = millis();

    if ((currentTime - controlSampleTime) > 200) {
      // Read the front panel controller
      changes = readChanges();
      if ((changes >> CHANGED_VOL_BIT) & 1) {    
        changeVolume();
      }
      if ((changes >> CHANGED_STATION_BIT) & 1) {
        changeStation();
      }
      controlSampleTime = currentTime;
   }

}

unsigned int readChanges() {
  unsigned int changes;

  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(INST_GET_CHANGES);
  delayMicroseconds(20);
  changes = SPI.transfer(0x00);
  digitalWrite(slaveSelectPin, HIGH);

  Serial.print("Read Changes:");
  Serial.println(changes, BIN);

  return changes;

}

void changeStation()
{
   unsigned int selectedStation = FP_getStation();
   FP_resetChanges();

   Serial.print("Station:");
   Serial.println(selectedStation);
}

/**
 *  Get the selected station from the front panel controller.
 */
unsigned int FP_getStation() {
   unsigned int station;

   digitalWrite(slaveSelectPin, LOW);
   SPI.transfer(INST_GET_STATION);
   delayMicroseconds(20);
   station = SPI.transfer(0x00);
   digitalWrite(slaveSelectPin, HIGH);

   return station;
 }

void changeVolume()
{
  unsigned int volume = FP_getVolume();
  FP_resetChanges();

  Serial.print("Volume:");
  Serial.println(volume);
}

/*
 * Get the volume set from the front panel controller.
 */
unsigned int FP_getVolume() {
  unsigned int volume;

  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(INST_GET_VOL);
  delayMicroseconds(20);   //Wait for the instruction to be processed by the slave.

  // Transfer byte) from the slave and pack it into an unsigned int
  volume  = SPI.transfer(0x00);
  digitalWrite(slaveSelectPin, HIGH);

  return volume;
}

void FP_resetChanges() {
   digitalWrite(slaveSelectPin, LOW);
   SPI.transfer(INST_RESET_CHANGES);
   delayMicroseconds(20);   //Wait for the instruction to be processed by the slave.

   digitalWrite(slaveSelectPin, HIGH);
}

void FP_begin() {
  // Send null instruction to reset
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(INST_NULL);
  digitalWrite(slaveSelectPin, HIGH);
}
