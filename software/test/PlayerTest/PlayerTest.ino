/**
  * Player Test
  *
  *    This sketch tests the VS1053 player
  *
  */

#include <SPI.h>
#include "wiring_private.h"
#include "Lemon_VS1053.h"

const char programName[] = "Player Test v2";

/*---------------  PIN SETUP ---------------------*/
// Pin setup for the VS1053
const int DREQ = A3;  // Used as digital pin
const int XRST = A1;  // Used as digital pin
const int XDCS = A2;  // Used as digital pin
const int XCS = 6;

// SPI void set up
//SPIClass *spi;
SPIClass spi(&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

// Setup the VS1053 Lemon player using standard hardware SPI pins and the
// pin for the command chip select (XCS)
Lemon_VS1053 player(&spi, XRST, XCS, XDCS, DREQ);


void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println(programName);

//  pinMode(DREQ, INPUT);
//  pinMode(XRST, OUTPUT);
//  pinMode(XDCS, OUTPUT);
//  pinMode(XCS, OUTPUT);
  

  // Assign pins 11, 12, 13 to SERCOM functionality
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(12, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);


  Serial.println("Begin");
  player.begin();
  player.setVolume(40,40);
  player.dumpRegs();

  Serial.println("Begin Sine Test");
  player.sineTest(216, 5000);  //216 = 5168 Hz, 5 secs
  Serial.println("End Sine Test");

}

void loop() {
 
 }
