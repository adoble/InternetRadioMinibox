/**
  * Player Test
  *
  *    This sketch tests the VS1053 player
  *
  */

#include <SPI.h>
#include "wiring_private.h"
#include "Lemon_VS1053.h"

const char programName[] = "Player Test v1";

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
//Lemon_VS1053 *player;
Lemon_VS1053 player(&spi, XRST, XCS, XDCS, DREQ);


void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println(programName);

  // SPI  set up
  //spi = new SPIClass(&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

  //spi->begin(); 
  
  // Assign pins 11, 12, 13 to SERCOM functionality
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(12, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);


// Setup the VS1053 Lemon player using standard hardware SPI pins and the
// pin for the command chip select (XCS)
 //player = new Lemon_VS1053(spi, XRST, XCS, XDCS, DREQ);
//
Serial.println("Begin");
  player.begin();
//
Serial.println("Begin Sine Test");

  player.sineTest(216, 5000);  //216 = 5168 Hz, 5 secs

Serial.println("End Sine Test");

}

void loop() {
//  // Noise!
//  spi->beginTransaction(SPISettings(250000, MSBFIRST, SPI_MODE0));
//  spi->transfer(B01010101);
//  spi->endTransaction();

}
