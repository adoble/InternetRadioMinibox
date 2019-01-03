#include <SPI.h>
#include "wiring_private.h" // pinPeripheral() function
  
SPIClass mySPI (&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

void setup() {
  Serial.begin(115200);

  pinMode(A2, OUTPUT);
  digitalWrite(A2, HIGH);
  pinMode(A4, OUTPUT);
  digitalWrite(A4, HIGH);
  pinMode(A5, OUTPUT);
  digitalWrite(A5, HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  

  // do this first, for Reasons
  mySPI.begin();

  // Assign pins 11, 12, 13 to SERCOM functionality
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(12, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);
}

uint8_t i=0;
void loop() {
  Serial.println(i);
  mySPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
 // mySPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  //mySPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  //mySPI.transfer(i++);
  mySPI.transfer(B01010101);
  
  mySPI.endTransaction();
}
