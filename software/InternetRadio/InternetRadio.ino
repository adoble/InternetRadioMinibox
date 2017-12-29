/**
* Internet Radio
*
*  This sketch:
*    - connects (WPA encryption)to an internet radio site using an ESP8266 module
*      configured as an Arduino using the libraries at https://github.com/esp8266/Arduino
*    - reads the mp3 data stream
*    - buffers the data in an external SPI controlled RAM
*    - transfers it to the VS1053 so that it can decode
*      the mp3 data into an audio signal.
*    TO DO - In addition it reads front panel settings  (e..g volume, station etc.) from
*      an external AVR chip (configured as Arduino) over an SPI interface. This
*      happens periodically when the interrupt service rounting (ISR) is triggered. The code
*      for the external AVM micro-controller can be found at software/FPController (FP means
*      Front Panel).
*
* Standard Libraries
* ==================
* Arduino           - Standard Arduino library
* SPI               - Serial Progamming Interface standard library
* ESP8266WiFi       - This is the WiFi library for the ESP8266 that behaves as the Arduino
*                     WiFI library.
* ESP8266WiFiMulti  - Standard library for connecting to  multiple access points from
*                     the ESP8266
* ESP8266HTTPClient - Standard library for HTTP processing on the ESP8266. Using
*                     version 2.3.0 of the board package.
*
* Created Libaries (for the project)
* ==================================
* SPIRingBuffer     - A ring buffer that is implemented to use an SPI based 23K256 RAM.
*                     This is used to buffer the data to that sproadic gaps in the internet connection
*                     are "smoothed out".
* Lemon_VS1053      - A local library to control the VS1053
*
*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>  //Using version 2.3.0 of the board package
#include <SPI.h>
#include "Lemon_VS1053.h"
#include "SPIRingBuffer.h"
#include "credentials.h"

const char programName[] = "InternetRadioV16";

// Front panel controller instruction codes
// TODO move these into a header file
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


#define USE_SERIAL Serial

// Pin setup for the VS1053
const int DREQ = 5;
const int XCS = 4;
const int XRST = 2;
const int XDCS = 16;

// Pin setup for the 23K256 RAM
const int RAMCS = 15;     // Chip select for the external rRAM used for the ring buffer

// Pin setup for the AVR chip used as front panel controller
const int FPCS = 0;


// Setup the VS1053 Lemon player using standard hardware SPI pins
Lemon_VS1053 player = Lemon_VS1053(XRST, XCS, XDCS, DREQ);

// Set up the external RAM as a ring buffer
SPIRingBuffer ringBuffer(RAMCS);

// Flag to ensure that the ring buffer is fully loaded on startup
bool bufferInitialized = false;

// The threshold at which the buffer will be loaded at a faster rate (i.e. to cacth up).
const int THRESHOLD = ringBuffer.RING_BUFFER_LENGTH / 5;   // 20%

// The skip flag to allow the buffer to catch up
int skip = 0;

volatile int checkControlStatus = 0;

// Setup the WIFI objects
ESP8266WiFiMulti WiFiMulti;
HTTPClient http;

// WiFi configuration using the defines in credentials.h
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PWD;

// URL of radio staion to access. Uncomment what is wanted.
// TODO replace this with code so that it can be configured by the user
const int NUMBER_STATIONS = 2;
//String station= "http://rpr1.fmstreams.de/stream1";   // RPR1   64kps  OBSOLEATE
String stations[NUMBER_STATIONS] = {
  "http://rpr1.radio.net/",   // RPR1
  "http://217.151.151.90:80/rpr-80er-128-mp3" // RPR1 Best of the 80s - 128kbs
};
//String station= "http://rpr1.radio.net/";"
//String station= "http://217.151.151.90:80/rpr-80er-128-mp3";   // RPR1   128kps
//String station= "http://rpr1.fmstreams.de/rpr-80er-128-mp3";   // RPR1 Best of the 80s - 128kbs
//String station= "http://swr-mp3-m-swr3.akacast.akamaistream.net/7/720/137136/v1/gnl.akacast.akamaistream.net/swr-mp3-m-swr3";   // SWR3 - 128kbs


//String station = "http://www.andrew-doble.homepage.t-online.de/";  //TEST
//String station = "http://ledjamradio.ice.infomaniak.ch/ledjamradio.mp3";

// Create a buffer to read in the mp3 data. Set to DATABUFFERLEN as this
// is the amount that can be transfered to the VS1053 in one SPI operation.
const int DATABUFFERLEN = 32;
uint8_t mp3Buffer[DATABUFFERLEN];
int bufferIndex = 0;

// The url of the station currently playing
String currentStation;

// number of byte availble in the read stream
size_t nBytes;

// Pointer to the payload stream, i.e. the MP3 data from the internet station.
WiFiClient * stream;

// Default setting for the tone control
// TODO the minbox does not have a tone control.
// TODO Before removing see what default setting sounds best.
int toneControl = 0;


// Function prototypes
void connectWLAN(const char*, const char*);
void handleRedirect();
void handleOtherCode(int);
String getStationURL();


void inline timerHandler (void){
  checkControlStatus = 1;
  //timer0_write(ESP.getCycleCount() + 41660000); // Period 470 mS
  timer0_write(ESP.getCycleCount() + 20830000); // Period 235 mS
  
  
}

void setup() {

    USE_SERIAL.begin(115200);
    delay(10);
    //USE_SERIAL.setDebugOutput(true);  //!!!!!

    // So we know what version we are running
    // TODO review this in light of using Git
    USE_SERIAL.println(programName);
    USE_SERIAL.println();

  // Before initialising using the libraries  make sure that the CS pins are
  // in the right state
  pinMode(XCS, OUTPUT);
  digitalWrite(XCS, HIGH);
  pinMode(XDCS, OUTPUT);
  digitalWrite(XDCS, HIGH);
  pinMode(RAMCS, OUTPUT);
  digitalWrite(RAMCS, HIGH);
  pinMode(FPCS, OUTPUT);
  digitalWrite(FPCS, HIGH);
  delay(1);

  // Set up the timer interupt
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(timerHandler);
  timer0_write(ESP.getCycleCount() + 41660000);
  interrupts();


  // Initialise the ring buffer
  ringBuffer.begin();   //TODO is the buffer too long, resulting in too long a delay?


  // Initialize the player
  if ( !player.begin()) { // initialise the player
     USE_SERIAL.println("Error in player init!");
     player.dumpRegs();
  }


    // Make sure the VS1053 is in MP3 mode.
    // For some MP3 decoder boards this is not the case.
    while (!player.readyForData()) {yield();}
    player.setMP3Mode();

    // Set the initial volume by reading from the front panal controller
       while (!player.readyForData()) {yield();}
//    player.setVolume(30,30);  // Higher is quieter.
//    player.dumpRegs();
      adjustVolume(); 

    // Connect to the WIFI access point
    // TODO  need to refactor so that a connection can be
    // reestablished after being lost
    Serial.print("Attempting to connect to WIFI AP ");
    Serial.println(ssid);

    connectWLAN(ssid, password);

      // Get the station
      currentStation =  getStationURL();

      USE_SERIAL.print("[HTTP] begin connection to ");
      USE_SERIAL.print(currentStation);
      USE_SERIAL.println(" ...");


      // Configure server and url
      http.begin(currentStation);


      USE_SERIAL.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();
      if(httpCode > 0) {
          // HTTP header has been sent and server response header has been handled
          USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

          // file found at server
          switch (httpCode) {
            case HTTP_CODE_OK:
              // get tcp stream of payload
              stream = http.getStreamPtr();
              break;
            case HTTP_CODE_TEMPORARY_REDIRECT:
               handleRedirect();
               break;
            case HTTP_CODE_PERMANENT_REDIRECT:
               handleRedirect();
               break;
            default:
              // HTTP cde retuned that cannot be handled
              handleOtherCode(httpCode);
              break;

          } // switch httpCode
      }
      else {
          // TODO replace this with the general error handling
          USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
  //}  // --- wifi connected

}

void loop() {
  int nRead = 0;
  int maxBytesToRead;

  if (!bufferInitialized) {
    // Load up the buffer
    nBytes = stream->available();
    if (nBytes) {
      // read in chunks of up to 32 bytes

      //    availableSpace()       nBytes         ->    maxBytesToRead
      //    =================   =================     ===============
      //      >=DATABUFFERLEN   >DATABUFFERLEN         DATABUFFERLEN
      //      >=DATABUFFERLEN   <=DATABUFFERLEN        nBytes
      //      <DATABUFFERLEN          -                availableSpace

      if (ringBuffer.availableSpace() < DATABUFFERLEN) maxBytesToRead = ringBuffer.availableSpace();
      else maxBytesToRead = (nBytes> DATABUFFERLEN ? DATABUFFERLEN : nBytes);
      nRead = stream->readBytes(mp3Buffer, maxBytesToRead);
      // Transfer to buffer
      for (int i = 0; i < nRead; i++) {
        ringBuffer.put(mp3Buffer[i]);
      }
      if (ringBuffer.availableSpace() == 0)  {
          bufferInitialized = true;
          USE_SERIAL.println("Buffer initialised");
      }
    }

  }   // -- if bufferInitialized

  // Adding data to buffer
  // Is ring buffer full?
  //    no:   is source data available?
  //           yes: read source, data --> ring buffer
  //           no:  no-op
  //    yes: no-op
  //
  if (bufferInitialized) {
    if (ringBuffer.availableSpace() > DATABUFFERLEN) {
      nBytes = stream->available();
      if (nBytes) {
        // read up to 32 bytes
        nRead = stream->readBytes(mp3Buffer, (nBytes> DATABUFFERLEN ? DATABUFFERLEN : nBytes));
        // Transfer to buffer
        for (int i = 0; i < nRead; i++) {
          ringBuffer.put(mp3Buffer[i]);
        }
      }
    }
  }

  // Test of the tone control
  //TODO  The minibox has no tone control. Find a default that sounds best.
  if (bufferInitialized && checkControlStatus == 1) {
    checkControlStatus = 0;
    if (toneControl == -15) toneControl = 15;
    else toneControl = toneControl -1;
    USE_SERIAL.println(toneControl);
    player.setTone(toneControl);

    unsigned int changes = getChanges();  
    if (changes &  1 << CHANGED_VOL_BIT) {
      adjustVolume();
    }
    if (changes &  1 << CHANGED_STATION_BIT) {
      // TODO Change station. 
    }
    


  }


  // Skip a transfer to the VS1053 to give the buffer a chance to reload if
  //  the data amout has fallen below THRESHOLD
  skip = ((bufferInitialized && (ringBuffer.availableData() < THRESHOLD)) ? (skip++)%2 : 0);

  // Moving data to VS1053
  // does VS1053 accept data? and not skip?
  //    yes: does ring buffer have data?
  //           yes: ring buffer data --> VS1053
  //           no:  no-op
  //    no:  no-op

  if (bufferInitialized  && !skip) {
      // Transfer to VS1053
      if (player.readyForData()) {
        nRead = (ringBuffer.availableData() > DATABUFFERLEN ? DATABUFFERLEN : ringBuffer.availableData());
        for (int i= 0; i < nRead; i++) {
          mp3Buffer[i] = ringBuffer.get();
        }
        player.playData(mp3Buffer, nRead);
      }

  }

    //    http.end();  // TODO Where?


}

/* Connect to the wireless LAN */
void connectWLAN(const char* ssid, const char* password) {

   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   while((WiFi.status() != WL_CONNECTED)) {
     yield();
     delay(500);
     USE_SERIAL.print(".");
   }
   USE_SERIAL.println();
   USE_SERIAL.println("Connected to WIFI AP");
   USE_SERIAL.print("IP Address: ");
   USE_SERIAL.println(WiFi.localIP());

}


/*
 *  Handles HTTP redirects
 *  As there are currently difficulties in finding a site that has redirects
 *  FIXME this function has NOT BEEN TESTED
 */
void handleRedirect() {
    String newSite;
    // TODO
    USE_SERIAL.println("REDIRECT!");

    if (http.hasHeader("Location")) {
      newSite = http.header("Location");
    }

    http.begin(newSite);

    // start connection and send HTTP header
    int httpCode = http.GET();

    if(httpCode > 0) {
        // file found at server
        switch (httpCode) {
          case HTTP_CODE_OK:
            // TODO Add this code
            break;
          case HTTP_CODE_FOUND:
            // TODO add this code
            break;
          default:
            // AN HTTP code has been returned that cannot be handled
            handleOtherCode(httpCode);
            break;
        }
    }
    else {
      // TODO replace this with the general error handling
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }


}

/*
 *  Handles other HTTP codes
 *
 */
void handleOtherCode(int httpCode) {
  // TODO as part of the general error handling
  USE_SERIAL.print("Cannot handle this HTTP code:");
  USE_SERIAL.println(httpCode);

}

/* Read the URL of the station that the readi is currently tuned into
 *
 * NOTE: This is a dummy function to be replaced with a an SPI read of the station URL.
 */
String getStationURL()
{
   return stations[1];  // RPR 80s

}

// Get the volument from the front panal controller 
// and adjust it on the player.
void adjustVolume() {
    unsigned int volume = getVolume();
    int playerVol = map(volume, 1, 31, 200, 0);
    
    player.setVolume(playerVol,playerVol);
}

/*
 * Get the volume set from the front panel controller.
 */
unsigned int getVolume() {
  unsigned int volume;

  digitalWrite(FPCS, LOW);
  SPI.transfer(INST_GET_VOL);
  delayMicroseconds(20);   //Wait for the instruction to be processed by the slave.

  // Transfer byte) from the slave and pack it into an unsigned int
  volume  = SPI.transfer(0x00);
  digitalWrite(FPCS, HIGH);

  return volume;
}

// Get the change status from the front panal controller
unsigned int getChanges() {
  unsigned int changes;

  digitalWrite(FPCS, LOW);
  SPI.transfer(INST_GET_CHANGES);
  delayMicroseconds(20);   //Wait for the instruction to be processed by the slave.

  // Transfer byte) from the slave and pack it into an unsigned int
  changes = SPI.transfer(0x00);
  digitalWrite(FPCS, HIGH);

  return changes;
}


// Set the VS1053 chip into MP3 mode
//void set_mp3_mode()
//{
//
//   while (!player.readyForData());
//
//   player.sciWrite(VS1053_REG_WRAMADDR, 0xc017);
//   player.sciWrite(VS1053_REG_WRAM, 0x0003);
//
//   player.sciWrite(VS1053_REG_WRAMADDR, 0xc019);
//   player.sciWrite(VS1053_REG_WRAM, 0x0000);
//   delay(100);
//   player.softReset();
//
//   delay(100);
//
//}
