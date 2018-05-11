#include <SPI.h>
#include "wiring_private.h" // pinPeripheral() function

// SRAM instructions
#define RDSR  0x05
#define WRSR  0x01
#define READ  0x03
#define WRITE 0x02

// SRAM Hold disable bit.
#define HOLD_DISABLE 1

// SRAM byte mode
#define BYTE_MODE (0x00 | HOLD_DISABLE)

SPIClass *spi;
//SPIClass spi(&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

//SPISettings spiSettings(16000000, MSBFIRST, SPI_MODE0);   // NOT OK
  SPISettings spiSettings(8000000, MSBFIRST, SPI_MODE0);  // OK
// SPISettings spiSettings(1000000, MSBFIRST, SPI_MODE0);  //OK
// SPISettings spiSettings(1200000, MSBFIRST, SPI_MODE0);  //OK
// SPISettings spiSettings(1400000, MSBFIRST, SPI_MODE0);    //OK
// SPISettings spiSettings(1200000, MSBFIRST, SPI_MODE0);   
// SPISettings spiSettings(10000000, MSBFIRST, SPI_MODE0);   // NOT OK
//  SPISettings spiSettings(14000000, MSBFIRST, SPI_MODE0);  // NOT OK


    


// Pins
const int RAM_CS = A4; // Chip select pin for RAM = A4

const int FP_CONTROLLER_CS = A5; 
const int XDCS = A2;
const int XCS = 6;

int mode;
int operationMode; 

boolean testOK = true; 
int testId = 0; 

void setup() {
  while(!Serial);
  Serial.begin(115200);

  spi = new SPIClass(&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

  pinMode(RAM_CS, OUTPUT);

  // Make sure other chsip selects are high
  pinMode(FP_CONTROLLER_CS, OUTPUT); 
  pinMode(XDCS, OUTPUT); 
  pinMode(XCS, OUTPUT); 
  digitalWrite(FP_CONTROLLER_CS, HIGH);

   // Pulling the folling high causes incorrect reads, don't know why  --->  FIXME
  digitalWrite(XDCS, HIGH); //!!!!!!!!
  digitalWrite(XCS, HIGH); 

  delay(10);


  // do this first, for Reasons  !!!!!!!!
  spi->begin();

  // Assign pins 11, 12, 13 to SERCOM functionality
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(12, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);

    // Set mode
  setMode(BYTE_MODE);

  initMem();

}

uint8_t i=0;
void loop() {
  //Serial.println(i);
  //mySPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  //spi.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));


  delayMicroseconds(50);
  unsigned int address = B00001111;
  unsigned char  testVal = B1010100;
  writeByte(address, testVal);

  //delayMicroseconds(1);

  unsigned char val = 0;
  val = readByte(address);

  if (val != testVal) testOK = false;

  delay(3);
  Serial.print(testId, 10);
  Serial.print("   ");
  Serial.print(testVal, BIN);
  Serial.print(":");
  Serial.print(val, BIN);
  Serial.print(val==testVal? "     OK": "     NOT OK");
  Serial.print(testOK?"":  "  Incorrect test was previously detected.");
  Serial.println();

  testId++; 
  

    //mySPI.transfer(i++);
  //spi.transfer(B01010101);
  
  //spi.endTransaction();
}

void initMem() {
  for (unsigned int i = 0; i< 256; i++) {
    writeByte(i, 0);
  }
  Serial.println("Finished init");
}

void setMode(char mode)
{
  
    spi->beginTransaction(spiSettings);
    chipSelect(true);
    spi->transfer(WRSR);
    spi->transfer(mode);
    chipSelect(false);
    spi->endTransaction();
    operationMode = mode;
  
}

void writeByte(unsigned int address, char data)
{

  // Set byte mode
  //setMode(BYTE_MODE);

  // Write address, read data
  spi->beginTransaction(spiSettings); // !!!! before chi select 
  chipSelect(true);  
  spi->transfer(WRITE);
  spi->transfer((unsigned char)(address >> 8));
  spi->transfer((unsigned char)address);

  spi->transfer(data);
  chipSelect(false);

  spi->endTransaction();  // !!!!! after chip select 

}

unsigned char readByte(unsigned int address)

{
  //unsigned char res;
  unsigned char res;

  // Set byte mode
  //setMode(BYTE_MODE);

  // Write address, read data
  spi->beginTransaction(spiSettings);
  
  chipSelect(true);
  
  spi->transfer(READ);
  spi->transfer((unsigned char)(address >> 8));
  spi->transfer((unsigned char)address);

  res = spi->transfer(0xFF);
  chipSelect(false);

  spi->endTransaction();
 
  
  return res;
}

void  chipSelect(bool state)
 {
   digitalWrite(RAM_CS, !state);
 }
