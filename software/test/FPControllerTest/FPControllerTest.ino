/*
 * FPController test

 */

#include <SoftwareSerial.h>   // We need this even if we're not using a SoftwareSerial object
                              // Due to the way the Arduino IDE compiles
#include <SerialCommand.h>    // See https://github.com/scogswell/ArduinoSerialCommand/blob/master/examples/SerialCommandExample/SerialCommandExample.pde
#include <SPI.h>


const byte INST_NULL = 0x00;
const byte INST_GET_STATION = 0x01;
const byte INST_GET_VOL = 0x02;
const byte INST_STATUS_OK = 0x03;
const byte INST_STATUS_ERROR = 0x04;

SerialCommand cli;   // The demo SerialCommand object

long lastInterval;
boolean repeatFlag;
boolean verbose = false;

byte spiBuffer[1];

// set pin 10 as the slave select (CS) :
const int slaveSelectPin = 10;

// Hardward Repeat pin for plotting
const int hardwareRepeatPin = 3;

void setup() {
  Serial.begin(115200);

  lastInterval = -1;
  repeatFlag = false;

  // set the slaveSelectPin as an output:
  pinMode(slaveSelectPin, OUTPUT);

  pinMode(hardwareRepeatPin, INPUT);

  Serial.println("Enter command (be sure to have CR activated).");
  Serial.println("     station : Get station");
  Serial.println("     vol: Get volume");
  Serial.println("     ok: Set status OK");
  Serial.println("     error <<error code  0 - 26>>: Set status error");
  Serial.println("     null: Null instruction");
  Serial.println("     debug: Debug instruction");
  Serial.println("     invalid: Invalid instruction");

  Serial.println();
  Serial.println("     *: Repeat instruction");
  Serial.println("     !: Stop Repeat");
  Serial.println("     terse: Set terse output");
  Serial.println("     verbose: Set verbose output");


  cli.addCommand("station", getStation);
  cli.addCommand("vol", getVolume);
  cli.addCommand("ok", setOK);
  cli.addCommand("error", setError);
  cli.addCommand("null", sendNull);
  cli.addCommand("invalid", sendInvalidInstruction);
  
  cli.addCommand("debug", setDebug);
  cli.addCommand("*", repeat);
  cli.addCommand("!", stopRepeat);
  cli.addCommand("terse", setTerse);
  cli.addCommand("verbose", setVerbose);
  cli.addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?")

  SPI.begin();

  sendNull(); // To reset THIS IS IMPORTANT --> Make it a reset function the master
}

void loop() {



  if (digitalRead(hardwareRepeatPin) == HIGH) {
    delay(250);
    //getStation();
    sendNull();
  }
  else {
    cli.readSerial();

    if (repeatFlag) {
      if (lastInterval > -1) {
        if ( (millis() - lastInterval) > 200) {
          sendError(12);
          lastInterval = millis();
        }
      } else {
        lastInterval = millis(); //Initialise

      }

    }
  }
}


void getStation()
{
   if (verbose) Serial.println("SEND GET_STATION");

   spiBuffer[0] = INST_GET_STATION;

   digitalWrite(slaveSelectPin, LOW);
   SPI.transfer(spiBuffer, 1);
   delayMicroseconds(20);
   byte station = SPI.transfer(0x00);
   digitalWrite(slaveSelectPin, HIGH);

   if (verbose) Serial.println("Station:");
   Serial.println(station);
}

void getVolume()
{
         // DEBUG
        Serial.println("SEND GET_VOL");

        spiBuffer[0] = INST_GET_VOL;

        digitalWrite(slaveSelectPin, LOW);
        SPI.transfer(spiBuffer[0]);
        delayMicroseconds(20);   //Wait for the instruction to be processed by the slave.

        // Transfer byte) from the slave and pack it into an unsigned int
        unsigned int volume  = SPI.transfer(0x00);
        digitalWrite(slaveSelectPin, HIGH);

        Serial.print("Volume:");
        Serial.print(volume);
        Serial.print(" | ");
        Serial.print(volume, BIN);
        Serial.print(" | ");
        Serial.println(volume, HEX);
}


void setOK() {
   Serial.println("SEND INST STATUS OK INSTRUCTION");
   Serial.println("... All lights should go off.");

   spiBuffer[0] = INST_STATUS_OK;
   digitalWrite(slaveSelectPin, LOW);
   SPI.transfer(spiBuffer, 1);
   digitalWrite(slaveSelectPin, HIGH);
}

void setError()
{
  int errorCode;

  char* arg = cli.next();    // Get the next argument from the SerialCommand object buffer
  if (arg != NULL)      // As long as it existed, take it
  {
    Serial.print(" Error code: ");

    // Convert to a number
    errorCode = String(arg).toInt();
    Serial.println(errorCode);
    if (errorCode >= 0 && errorCode <= 26)
    {
      sendError(errorCode);
    }
    else {
      Serial.println(); Serial.println("No or false error code entered");
    }
  }

}

void sendError(int errorCode)
{
  spiBuffer[0] = INST_STATUS_ERROR;
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(spiBuffer[0]);
  delayMicroseconds(20);   // Wait for the instruction to be processed by the slave.

  // Transfer error code  to slave
  SPI.transfer(errorCode & 0xFF);
  digitalWrite(slaveSelectPin, HIGH);
}

void sendNull()
{
  Serial.println("SEND NULL INSTRUCTION");
  spiBuffer[0] = INST_NULL;
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(spiBuffer, 1);
  digitalWrite(slaveSelectPin, HIGH);
}

void setDebug()
{
  Serial.println("SEND INST STATUS ERROR INSTRUCTION");
  Serial.println("... Red light should go on.");

  spiBuffer[0] = INST_STATUS_ERROR;
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(spiBuffer, 1);
  digitalWrite(slaveSelectPin, HIGH);
}

void sendInvalidInstruction()
{
  Serial.println("SEND INVALID INSTRUCTION");
  Serial.println("Should be ignored");

  spiBuffer[0] = 27;
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(spiBuffer, 1);
  digitalWrite(slaveSelectPin, HIGH);
  delayMicroseconds(20);   // Wait for the instruction to be processed by the slave.
}


//void setIntensity(byte instruction)
//{
//
//  int intensity;
//
//  char* arg = cli.next();    // Get the next argument from the SerialCommand object buffer
//  if (arg != NULL)      // As long as it existed, take it
//  {
//    Serial.print(" ");
//    Serial.println(arg);
//    // Convert to a number
//    intensity = String(arg).toInt();
//
//    if (intensity >= 0 && intensity <= 255)
//    {
//      spiBuffer[0] = instruction;
//      digitalWrite(slaveSelectPin, LOW);
//      SPI.transfer(spiBuffer[0]);
//      delayMicroseconds(20);   //Wait for the instruction to be processed by the slave.
//
//      // Transfer intensity to slave
//      SPI.transfer(intensity & 0xFF);
//      digitalWrite(slaveSelectPin, HIGH);
//    }
//    else {
//      Serial.println(); Serial.println("No or false intensity parameter entered");
//    }
//  }
//
//}

void repeat()
{
  Serial.println("Repeats station");

  repeatFlag = true;


}

void stopRepeat()
{
  Serial.println("Stop repeating station ");

  repeatFlag = false;
 }

 void setTerse() {
  verbose = false;
 }

 void setVerbose() {
  verbose = true;
 }

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized()
{
  Serial.println("What?");
}
