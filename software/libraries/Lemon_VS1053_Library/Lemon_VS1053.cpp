/***************************************************
  Basic library for the VS1053.

  This is stripped down version of the original  Adafruit VS1053 library
  written by Limor Fried/Ladyada for Adafruit Industries. The following
  functionality has been removed:
    - SD card functionality.
    - Hardware SPI pins. Assuming that in modern boards this is not going
      to be used.

  In addtion the following has been added:
    - For M0 based designs that use different SERCOMs for SPI a new
      constructor that specifies the configured SPI class to be used.


  -- Original text from  the Adafruit library -->

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

  <-- End original text
 ****************************************************/

#include <Lemon_VS1053.h>


#ifndef _BV
  #define _BV(x) (1<<(x))
#endif

// DOBLE TEST - FORCE THIS TO BE DEFINED
#ifndef SPI_HAS_TRANSACTION
  #define SPI_HAS_TRANSACTION 1
#endif


#define VS1053_CONTROL_SPI_SETTING  SPISettings(250000,  MSBFIRST, SPI_MODE0)
#define VS1053_DATA_SPI_SETTING     SPISettings(8000000, MSBFIRST, SPI_MODE0)  // Original SPISettings
//#define VS1053_DATA_SPI_SETTING     SPISettings(1700000, MSBFIRST, SPI_MODE0)


/* VS1053 'low level' interface */
static volatile PortReg *clkportreg, *misoportreg, *mosiportreg;
static PortMask clkpin, misopin, mosipin;


Lemon_VS1053::Lemon_VS1053(SPIClass *configuredSPI, int8_t rst, int8_t cs, int8_t dcs, int8_t dreq) {
  _mosi = 0;
  _miso = 0;
  _clk = 0;
  useHardwareSPI = true;
  useConfiguredSPI = true;
  _reset = rst;
  _cs = cs;
  _dcs = dcs;
  _dreq = dreq;
  _cs = cs;

  spi = configuredSPI;  // Repace the standard one with the configured one

}

Lemon_VS1053::Lemon_VS1053(int8_t rst, int8_t cs, int8_t dcs, int8_t dreq) {
  _mosi = 0;
  _miso = 0;
  _clk = 0;
  useHardwareSPI = true;
  useConfiguredSPI = false;
  _reset = rst;
  _cs = cs;
  _dcs = dcs;
  _dreq = dreq;
  _cs = cs;

  // SPI interface
  #if defined(ARDUINO_SAMD_ZERO)
    // Required for Serial on Zero based boards
    spi = new SPIClass(&sercom4, 22, 24, 23, SPI_PAD_2_SCK_3, SERCOM_RX_PAD_0);
  #else
    spi = &SPI;
  #endif

}



void Lemon_VS1053::applyPatch(const uint16_t *patch, uint16_t patchsize) {
  uint16_t i = 0;

 // Serial.print("Patch size: "); Serial.println(patchsize);
  while ( i < patchsize ) {
    uint16_t addr, n, val;

    addr = pgm_read_word(patch++);
    n = pgm_read_word(patch++);
    i += 2;

    //Serial.println(addr, HEX);
    if (n & 0x8000U) { // RLE run, replicate n samples
      n &= 0x7FFF;
      val = pgm_read_word(patch++);
      i++;
      while (n--) {
	sciWrite(addr, val);
      }
    } else {           // Copy run, copy n samples
      while (n--) {
	val = pgm_read_word(patch++);
	i++;
	sciWrite(addr, val);
      }
    }
  }
}



boolean Lemon_VS1053::readyForData(void) {
  return digitalRead(_dreq);
}

void Lemon_VS1053::playData(uint8_t *buffer, uint8_t buffsiz) {
  #ifdef SPI_HAS_TRANSACTION
  if (useHardwareSPI) spi->beginTransaction(VS1053_DATA_SPI_SETTING);
  #endif
  digitalWrite(_dcs, LOW);
  for (uint8_t i=0; i<buffsiz; i++) {
    spiwrite(buffer[i]);
  }
  digitalWrite(_dcs, HIGH);
  #ifdef SPI_HAS_TRANSACTION
  if (useHardwareSPI) spi->endTransaction();
  #endif
}

void Lemon_VS1053::setVolume(uint8_t left, uint8_t right) {
  uint16_t v;
  v = left;
  v <<= 8;
  v |= right;

  noInterrupts(); //cli();
  sciWrite(VS1053_REG_VOLUME, v);
  interrupts();  //sei();
}

// Set the tone.
// A positive value between 1 and 15 will give more treble
// A negative value between -1 and -15 will give omore bass
// Zero is the default setting
// Vaues over |15| will default to highest (positve of negative) value
void Lemon_VS1053::setTone(int tone) {
  int postTone;
  char treble;
  char bass;
  uint16_t sci_bass_value = 0;

  // Preprocess the tone value
  if (tone < -15) postTone = -15;
  else if (tone > 15) postTone = 15;
  else postTone = tone;
  if (postTone > 0) {
    treble = postTone;
  }
  else if (postTone < 0) {
    bass = abs(postTone);
  }
  else {
    treble = 0;
    bass = 0;
  }
  // set up the value for the SCI_BASE register
    sci_bass_value = treble;
    sci_bass_value <<= 4;
    sci_bass_value |= 0xA;  // Treble boot above 10 Khz
    sci_bass_value <<= 4;
    sci_bass_value |= bass;
    sci_bass_value <<= 4;
    sci_bass_value |= 0xA;  // Bass boot under 100 Hz


   // Send the SCI command
   noInterrupts(); //cli();
   sciWrite(VS1053_REG_BASS, sci_bass_value);
   interrupts();  //sei();

}


uint16_t Lemon_VS1053::decodeTime() {
  noInterrupts(); //cli();
  uint16_t t = sciRead(VS1053_REG_DECODETIME);
  interrupts(); //sei();
  return t;
}


void Lemon_VS1053::softReset(void) {
  sciWrite(VS1053_REG_MODE, VS1053_MODE_SM_SDINEW | VS1053_MODE_SM_RESET);
  delay(100);
}

void Lemon_VS1053::reset() {
  // TODO: http://www.vlsi.fi/player_vs1011_1002_1003/modularplayer/vs10xx_8c.html#a3
  // hardware reset
  if (_reset >= 0) {
    digitalWrite(_reset, LOW);
    delay(100);
    digitalWrite(_reset, HIGH);
  }
  digitalWrite(_cs, HIGH);
  digitalWrite(_dcs, HIGH);
  delay(100);
  softReset();
  delay(100);

  sciWrite(VS1053_REG_CLOCKF, 0x6000);

  setVolume(40, 40);
}

uint8_t Lemon_VS1053::begin(void) {

  if (_reset >= 0) {
    pinMode(_reset, OUTPUT);
    digitalWrite(_reset, LOW);
  }

  digitalWrite(_cs, HIGH);

  pinMode(_dcs, OUTPUT);
  digitalWrite(_dcs, HIGH);
  pinMode(_dreq, INPUT);

  if (! useHardwareSPI) {
    pinMode(_mosi, OUTPUT);
    pinMode(_clk, OUTPUT);
    pinMode(_miso, INPUT);
  } else {
    if (!useConfiguredSPI) {
      spi->begin();  // Alread initaliised when configuring the SPI using another SERCOM
      //spi->setDataMode(SPI_MODE0);   //REDUNDANT ? Already in SPISettings
      //spi->setBitOrder(MSBFIRST);    //REDUNDANT ? Already in SPISettings
      //spi->setClockDivider(SPI_CLOCK_DIV128);
    }
  }

  reset();

  // Make sure that the VS1053 board is set into MP3 mode
  setMP3Mode();
  //Serial.print("REG STATUS ");Serial.println(sciRead(VS1053_REG_STATUS));   // DEBUG

  return (sciRead(VS1053_REG_STATUS) >> 4) & 0x0F;
}

// DOBLE 21.08.2016
void Lemon_VS1053::setMP3Mode(void)
{
  //while (!readyForData());

  sciWrite(VS1053_REG_WRAMADDR, 0xc017);
  sciWrite(VS1053_REG_WRAM, 0x0003);

  sciWrite(VS1053_REG_WRAMADDR, 0xc019);
  sciWrite(VS1053_REG_WRAM, 0x0000);
  delay(100);
  softReset();
  delay(100);

}

void Lemon_VS1053::dumpRegs(void) {
  Serial.print("Mode = 0x"); Serial.println(sciRead(VS1053_REG_MODE), HEX);
  Serial.print("Stat = 0x"); Serial.println(sciRead(VS1053_REG_STATUS), HEX);
  Serial.print("ClkF = 0x"); Serial.println(sciRead(VS1053_REG_CLOCKF), HEX);
  Serial.print("Vol. = 0x"); Serial.println(sciRead(VS1053_REG_VOLUME), HEX);
}



void Lemon_VS1053::GPIO_pinMode(uint8_t i, uint8_t dir) {
  if (i > 7) return;

  sciWrite(VS1053_REG_WRAMADDR, VS1053_GPIO_DDR);
  uint16_t ddr = sciRead(VS1053_REG_WRAM);

  if (dir == INPUT)
    ddr &= ~_BV(i);
  if (dir == OUTPUT)
    ddr |= _BV(i);

  sciWrite(VS1053_REG_WRAMADDR, VS1053_GPIO_DDR);
  sciWrite(VS1053_REG_WRAM, ddr);
}


void Lemon_VS1053::GPIO_digitalWrite(uint8_t val) {
  sciWrite(VS1053_REG_WRAMADDR, VS1053_GPIO_ODATA);
  sciWrite(VS1053_REG_WRAM, val);
}

void Lemon_VS1053::GPIO_digitalWrite(uint8_t i, uint8_t val) {
  if (i > 7) return;

  sciWrite(VS1053_REG_WRAMADDR, VS1053_GPIO_ODATA);
  uint16_t pins = sciRead(VS1053_REG_WRAM);

  if (val == LOW)
    pins &= ~_BV(i);
  if (val == HIGH)
    pins |= _BV(i);

  sciWrite(VS1053_REG_WRAMADDR, VS1053_GPIO_ODATA);
  sciWrite(VS1053_REG_WRAM, pins);
}

uint16_t Lemon_VS1053::GPIO_digitalRead(void) {
  sciWrite(VS1053_REG_WRAMADDR, VS1053_GPIO_IDATA);
  return sciRead(VS1053_REG_WRAM) & 0xFF;
}

boolean Lemon_VS1053::GPIO_digitalRead(uint8_t i) {
  if (i > 7) return 0;

  sciWrite(VS1053_REG_WRAMADDR, VS1053_GPIO_IDATA);
  uint16_t val = sciRead(VS1053_REG_WRAM);
  if (val & _BV(i)) return true;
  return false;
}

uint16_t Lemon_VS1053::sciRead(uint8_t addr) {
  uint16_t data;

  #ifdef SPI_HAS_TRANSACTION
  if (useHardwareSPI) spi->beginTransaction(VS1053_CONTROL_SPI_SETTING);
  #endif
  digitalWrite(_cs, LOW);
  spiwrite(VS1053_SCI_READ);
  spiwrite(addr);
  delayMicroseconds(10);
  data = spiread();
  data <<= 8;
  data |= spiread();
  digitalWrite(_cs, HIGH);
  #ifdef SPI_HAS_TRANSACTION
  if (useHardwareSPI) spi->endTransaction();
  #endif

  return data;
}


void Lemon_VS1053::sciWrite(uint8_t addr, uint16_t data) {
  #ifdef SPI_HAS_TRANSACTION
  if (useHardwareSPI) spi->beginTransaction(VS1053_CONTROL_SPI_SETTING);
  #endif
  digitalWrite(_cs, LOW);
  spiwrite(VS1053_SCI_WRITE);
  spiwrite(addr);
  spiwrite(data >> 8);
  spiwrite(data & 0xFF);
  digitalWrite(_cs, HIGH);
  #ifdef SPI_HAS_TRANSACTION
  if (useHardwareSPI) spi->endTransaction();
  #endif
}



uint8_t Lemon_VS1053::spiread(void)
{
  int8_t i, x;
  x = 0;

  // MSB first, clock low when inactive (CPOL 0), data valid on leading edge (CPHA 0)
  // Make sure clock starts low

  if (useHardwareSPI) {
    x = spi->transfer(0x00);
  } else {
    for (i=7; i>=0; i--) {
      if ((*misoportreg) & misopin)
	x |= (1<<i);
      *clkportreg |= clkpin;
      *clkportreg &= ~clkpin;
      //    asm("nop; nop");
    }
    // Make sure clock ends low
    *clkportreg &= ~clkpin;
  }
  return x;
}

void Lemon_VS1053::spiwrite(uint8_t c)
{
  // MSB first, clock low when inactive (CPOL 0), data valid on leading edge (CPHA 0)
  // Make sure clock starts low

  if (useHardwareSPI) {
    spi->transfer(c);

  } else {
    for (int8_t i=7; i>=0; i--) {
      *clkportreg &= ~clkpin;
      if (c & (1<<i)) {
	*mosiportreg |= mosipin;
      } else {
	*mosiportreg &= ~mosipin;
      }
      *clkportreg |= clkpin;
    }
    *clkportreg &= ~clkpin;   // Make sure clock ends low
  }
}



void Lemon_VS1053::sineTest(uint8_t n, uint16_t ms) {

  reset();


  uint16_t mode = sciRead(VS1053_REG_MODE);
  mode |= 0x0020;
  sciWrite(VS1053_REG_MODE, mode);

  while (!digitalRead(_dreq));
	   delay(10);

  #ifdef SPI_HAS_TRANSACTION
  if (useHardwareSPI) spi->beginTransaction(VS1053_DATA_SPI_SETTING);
  #endif


  digitalWrite(_dcs, LOW);
  spiwrite(0x53);
  spiwrite(0xEF);
  spiwrite(0x6E);
  spiwrite(n);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  digitalWrite(_dcs, HIGH);
  #ifdef SPI_HAS_TRANSACTION
  if (useHardwareSPI) spi->endTransaction();
  #endif

  delay(ms);

  #ifdef SPI_HAS_TRANSACTION
  if (useHardwareSPI) spi->beginTransaction(VS1053_DATA_SPI_SETTING);
  #endif

  digitalWrite(_dcs, LOW);
  spiwrite(0x45);
  spiwrite(0x78);
  spiwrite(0x69);
  spiwrite(0x74);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  digitalWrite(_dcs, HIGH);
  #ifdef SPI_HAS_TRANSACTION
  if (useHardwareSPI) spi->endTransaction();
  #endif
}
