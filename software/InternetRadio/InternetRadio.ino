/**
* Internet Radio
*
*  This sketch:
*    - connects (WPA encryption) to an internet radio site using a
*      Adafruit Feather M0 WiFi with ATWINC1500 (processor ARM Cortex M0+)
*      with Arduino bootloader
*    - reads the mp3 data stream
*    - buffers the data in internal RAM
*    - transfers it to the VS1053 so that it can decode the mp3 data into
*      an audio signal.
*    In addition it reads front panel settings  (volume and station) from
*      an external AVR chip (configured with Arduino bootloader) over an SPI
*      interface. This happens periodically when the interrupt service routine
*      (ISR) is triggered. The code for the external AVM micro-controller can
*      be found at software/FPController (FP means Front Panel).
*
* Standard Libraries
* ==================
* Arduino           - Standard Arduino library
* SPI               - Serial Progamming Interface standard library
* WiFi101           - This is the WiFi library for ATWINC1500 used by the Feather.
* ArduinoHTTPClient - Standard library for HTTP processing.
*
* Created Libaries (for the project)
* ==================================
* StreamingBuffer   - A ring buffer that uses the internal RAM.
*                     This is used to buffer the data to that sproadic gaps in the internet connection
*                     are "smoothed out".
* Lemon_VS1053      - A local library to control the VS1053
*
*/

#include <Arduino.h>

#include <WiFi101.h>
#include <ArduinoHttpClient.h>
#include <SPI.h>
#include "wiring_private.h" // pinPeripheral() function
#include "Lemon_VS1053.h"
#include "StreamingBuffer.h"

#include "credentials.h"

const char programName[] = "M0_VERSION ";

/*---------------  PIN SETUP ---------------------*/
// Pin setup for the VS1053
const int DREQ = A3;  // Used as digital pin
const int XRST = A1;  // Used as digital pin
const int XDCS = A2;  // Used as digital pin
const int XCS = 6;

// Pin setup for the RAM (ring buffer)
//const int RAMCS = A4;  // Used as digital pin

// Pin setup for the front panel controller
const int FPCS = A5;  // Used as digital pin

// Interrupt signalling pin that the front panel controller has detected a change
const int FP_CHANGE_INTR= 5;

// Test pins
const int TEST_PIN_D = 10;
const int TEST_PIN_A = A0;

// Front panel controller instruction codes
// TODO move these into a header file
const byte INST_NULL          = 0x00;
const byte INST_GET_STATION   = 0x01;
const byte INST_GET_VOL       = 0x02;
const byte INST_STATUS_OK     = 0x03;
const byte INST_STATUS_ERROR  = 0x04;
const byte INST_GET_CHANGES   = 0x05;
const byte INST_RESET_CHANGES = 0x06;

// Changed status bits
const byte CHANGED_VOL_BIT     = 0;     // The volume has changed
const byte CHANGED_STATION_BIT = 1;     // A new station has been selected

// Set up the SPI on SERCOM1. Not using the standard SPI pins as these are
// connected to the WLAN module and this SPI bus is very chatty. The following
// assigns pins as follows:
//     SCK on Digital 13
//     MISO on Digital 12
//     MOSI on Digital 11
SPIClass spi(&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

// Set up the internal RAM as a ring buffer
const int STREAMING_BUFFER_SIZE = 20000; // NEED TO INCREASE
uint8_t streamingBufferStorage[STREAMING_BUFFER_SIZE];

StreamingBuffer streamingBuffer(streamingBufferStorage, STREAMING_BUFFER_SIZE);
// Flag to ensure that the ring buffer is fully loaded on startup
bool bufferInitialized = false;
// The threshold at which the buffer will be loaded at a faster rate (i.e. to catch up).
const int THRESHOLD = STREAMING_BUFFER_SIZE / 5;   // 20%
// The skip flag to allow the buffer to catch up
int skip = 0;

// Setup the VS1053 Lemon player using standard hardware SPI pins and the
// pin for the command chip select (XCS)
Lemon_VS1053 player = Lemon_VS1053(&spi, XRST, XCS, XDCS, DREQ);

volatile int checkControlStatus = 0;

int loopCount = 0;

// Setup WiFi client library
WiFiClient wifiClient;


// WiFi configuration using the defines in credentials.h
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PWD;

// URL of radio staion to access. Uncomment what is wanted.
// TODO replace this with code so that it can be configured by the user
const int NUMBER_STATIONS = 10;
//String station= "http://rpr1.fmstreams.de/stream1";   // RPR1   64kps  OBSOLEATE
String stations[NUMBER_STATIONS] = {
  //"http://217.151.151.90:80/rpr-80er-128-mp3", // RPR1 Best of the 80s - 128kbs
  "http://streams.rpr1.de/rpr-kaiserslautern-128-mp3",
  "http://streams.rp1.de/rpr-80er-128-mp3", // RPR1 Best of the 80s - 128kbs
  "http://swr-swr3-live.cast.addradio.de/swr/swr3/live/mp3/128/stream.mp3", // SWR3 - 128kps
  "http://bbcmedia.ic.llnwd.net/stream/bbcmedia_radio1_mf_p?s=1484901640&e=1484916040&h=0f66aa9663a84ce5183be725ec127cad",  // BBC1
  "http://bbcmedia.ic.llnwd.net/stream/bbcmedia_radio2_mf_p?s=1484901608&e=1484916008&h=1c6ffbe262747262e4c6267fd2f54bc1",  // BBC2
  "http://listen.181fm.com/181-classical_128k.mp3",  // Classical
  "http://mp3channels.webradio.antenne.de/antenne",   // Antenne
  "http://chai5she.cdn.dvmr.fr:80/fip-webradio1.mp3",  // FIP autour du rock
  "http://wdr-wdr3-live.icecast.wdr.de/wdr/wdr3/live/mp3/128/stream.mp3",  // WDR 3
  "http://185.52.127.157/de/33001/mp3_128.mp3"  // NRJ Berlin
  };

  //NOTE for RPR1 Stations see http://streams.rpr1.de/


// Create a buffer to read in the mp3 data. Set to DATABUFFERLEN as this
// is the amount that can be transfered to the VS1053 in one SPI operation.
const int DATABUFFERLEN = 32;
uint8_t mp3Buffer[DATABUFFERLEN];
int bufferIndex = 0;

// The url of the station currently playing
String currentStation;

// Number of bytes available in the read stream
size_t nBytes;

// Pointer to the payload stream, i.e. the MP3 data from the internet station.
//WiFiClient * stream;

// HTTP codes not covered by the Arduino HTTP client
// and are relevant
const int HTTP_OK = 200;
const int HTTP_TEMPORARY_REDIRECT = 307;
const int HTTP_PERMANENT_REDIRECT = 308;


// Use SerialUSB for the M0
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

// Function prototypes
void connectWLAN(const char*, const char*);
void handleRedirect();
void handleOtherCode(int);
String getStationURL();
String getHostFromURL(String);
String getPathFromURL(String);


/* Interrupt function for a change in the volume of the station
*/
void changeISR() {
  checkControlStatus = 1;
}

void setup() {

  //Configure pins for Adafruit ATWINC1500 Feather

  //while (!Serial)   ; // wait for serial port to connect. Needed for native USB port only
  Serial.begin(115200);

  // So we know what version we are running
  // TODO review this in light of using Git
  Serial.println(programName);
  Serial.println();

  // Set up the test pins
  pinMode(TEST_PIN_D, OUTPUT);
  digitalWrite(TEST_PIN_D, LOW);
  pinMode(TEST_PIN_A, OUTPUT);
  analogWrite(TEST_PIN_A, 0);

  // Before initialising using the libraries make sure that the CS pins are
  // in the right state
  pinMode(XCS, OUTPUT);
  digitalWrite(XCS, HIGH);
  pinMode(XDCS, OUTPUT);
  digitalWrite(XDCS, HIGH);
  //pinMode(RAMCS, OUTPUT);
  //digitalWrite(RAMCS, HIGH);


  // Assign pins 11, 12, 13 to SERCOM functionality
  // TODO Symbolic names for the pins
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(12, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);

  delay(50);

  Serial.println("Init ring buffer");

  // Initialise the ring buffer
  streamingBuffer.begin();   //TODO is the buffer too long, resulting in too long a delay?


  Serial.println("Init player");
  delay(100);

  // Initialize the player
  int nTries = 0;
  while ( !player.begin() && nTries < 20) { // initialise the player
    delay(1000);
    Serial.println("Error in player init!");
       digitalWrite(TEST_PIN_D, HIGH);
     player.dumpRegs();
     nTries++;


  }


 // player.begin();


  // Make sure the VS1053 is in MP3 mode.
  // For some MP3 decoder boards this is not the case.
  while (!player.readyForData()) {}
  player.setMP3Mode();
  Serial.println("MP3 Mode set.");

  // Set the initial volume by reading from the front panel controller
  while (!player.readyForData()) { }
  Serial.println("Volume setting to 60");
  player.setVolume(60,60);  // Higher is quieter.
  //adjustVolume();   //TODO provide a way for the FP controller to say that data is ready and that can work when it is disconnected

    // Setup the interrupt for changes to the controls
    attachInterrupt(digitalPinToInterrupt(FP_CHANGE_INTR), changeISR, FALLING);

    // Connect to the WIFI access point
    // TODO  need to refactor so that a connection can be
    // reestablished after being lost

    // Set up the pins for the Adafruit SPI connection to the Wifi component
    WiFi.setPins(8,7,4,2);

    Serial.print("Attempting to connect to WIFI AP ");
    Serial.println(ssid);

    connectWLAN(ssid, password);


      // Get the station
      currentStation =  getStationURL();

      Serial.print("[HTTP] begin connection to ");
      Serial.print(currentStation);
      Serial.println(" ...");

      String host = getHostFromURL(currentStation);
      String path = getPathFromURL(currentStation);

      Serial.print("Host:"); Serial.println(host);
      Serial.print("Path:"); Serial.println(path);

      // Configure server and url
      HttpClient http = HttpClient(wifiClient, host);

      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      http.get(path);

      int httpCode = http.responseStatusCode();
      if(httpCode > 0) {
          // HTTP header has been sent and server response header has been handled
          Serial.print("[HTTP] GET... code: "); Serial.println(httpCode);

          // file found at server
          switch (httpCode) {
            case HTTP_OK:
              // Continue to the loop
              // get tcp stream of payload
              //stream = http.getStreamPtr();
              break;
            case HTTP_TEMPORARY_REDIRECT:
               handleRedirect();
               break;
            case HTTP_PERMANENT_REDIRECT:
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
          Serial.print("[HTTP] GET... failed, http code: "); Serial.println(httpCode);
      }
  //}  // --- wifi connected

}

void loop() {
  int nRead = 0;
  int maxBytesToRead;

  //TEST write the sife of the bufer to the analog pin
  //analogWrite(TEST_PIN_A,  (streamingBuffer.availableData() / STREAMING_BUFFER_SIZE)*255);
  //Serial.print("B DATA: "); Serial.println(streamingBuffer.availableData());

  if (!bufferInitialized) {
    // Load up the buffer
    //nBytes = stream->available();
    nBytes = wifiClient.available();
    if (nBytes) {
      // read in chunks of up to 32 bytes

      //    availableSpace()       nBytes         ->    maxBytesToRead
      //    =================   =================     ===============
      //      >=DATABUFFERLEN   >DATABUFFERLEN         DATABUFFERLEN
      //      >=DATABUFFERLEN   <=DATABUFFERLEN        nBytes
      //      <DATABUFFERLEN          -                availableSpace

      if (streamingBuffer.availableSpace() < DATABUFFERLEN) maxBytesToRead = streamingBuffer.availableSpace();
      else maxBytesToRead = (nBytes> DATABUFFERLEN ? DATABUFFERLEN : nBytes);
      nRead = wifiClient.read(mp3Buffer, maxBytesToRead);   // Assuming that this exists (undocumented) in the WiFI101 library
      // Transfer to buffer
      for (int i = 0; i < nRead; i++) {
        streamingBuffer.put(mp3Buffer[i]);
      }
      if (streamingBuffer.availableSpace() == 0)  {
          bufferInitialized = true;
          Serial.print("Buffer initialised with ");Serial.print(streamingBuffer.availableData());Serial.println(" bytes.");
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
    if (streamingBuffer.availableSpace() > DATABUFFERLEN) {
      //nBytes = stream->available();
      nBytes = wifiClient.available();
      if (nBytes) {
        // read up to 32 bytes
        nRead = wifiClient.read(mp3Buffer, (nBytes> DATABUFFERLEN ? DATABUFFERLEN : nBytes));  // Assuming that this exists (undocumented) in the WiFI101 library
        // Transfer to buffer
        for (int i = 0; i < nRead; i++) {
          streamingBuffer.put(mp3Buffer[i]);

        }
      }
    }
  }

  // Read the front panel controller to ascertain the state of the controls.
  if (bufferInitialized && checkControlStatus == 1) {
    checkControlStatus = 0;

    //player.setTone(toneControl);  // TODO experiment with the best tone setting and place in setup()

    unsigned int changes = getChanges();
    //Serial.println(changes, BIN) ;
    if (changes &  (1 << CHANGED_VOL_BIT)) {
      adjustVolume();
    }
    if (changes &  (1 << CHANGED_STATION_BIT)) {
      // TODO Change station.
    }

    //adjustVolume();   // DOES NOT WORK!but used to
    // player.setVolume(50,50);


  }

  // Skip a transfer to the VS1053 to give the buffer a chance to reload if
  //  the data amount has fallen below THRESHOLD
  skip = ((bufferInitialized && (streamingBuffer.availableData() < THRESHOLD)) ? (skip++)%2 : 0);

  // Moving data to VS1053
  // does VS1053 accept data? and not skip?
  //    yes: does ring buffer have data?
  //           yes: ring buffer data --> VS1053
  //           no:  no-op
  //    no:  no-op

  if (bufferInitialized  && !skip) {
      // Transfer to VS1053
      if (player.readyForData()) {
        nRead = (streamingBuffer.availableData() > DATABUFFERLEN ? DATABUFFERLEN : streamingBuffer.availableData());
        for (int i= 0; i < nRead; i++) {
          mp3Buffer[i] = streamingBuffer.get();
        }
        player.playData(mp3Buffer, nRead);
      }

  }

    //    http.end();  // TODO Where?


}



/* Connect to the wireless LAN */
void connectWLAN(const char* ssid, const char* password) {
   const int maxAttempts = 30;
   int nAttempts = 0;

   Serial.println("Connecting to WLAN ...");
   //WiFi.mode(WIFI_STA);  //ESP8266
   WiFi.begin(ssid, password);
   while((WiFi.status() != WL_CONNECTED && (nAttempts < maxAttempts))) {
     delay(500);
     Serial.print(".");
     nAttempts++;
   }

   Serial.println();
   if (WiFi.status() == WL_CONNECTED ) {
     // Connection successful
     Serial.println("Connected to WIFI AP");
     Serial.print("IP Address: ");
     Serial.println(WiFi.localIP());
   } else {
     //Connection unsuccessful
     Serial.println("Could not connect to WIFI AP. Disconnecting.");
     WiFi.disconnect();
   }

}


/*
 *  Handles HTTP redirects
 *  As there are currently difficulties in finding a site that has redirects
 *  FIXME this function NEEDS TO BE WRITTEN
 */
void handleRedirect() {
/*

    String newSite;
    // TODO
    Serial.println("REDIRECT!");

    if (http.hasHeader("Location")) {
      newSite = http.header("Location");
    }

    http.begin(newSite);

    // start connection and send HTTP header
    int httpCode = http.get();

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
      Serial.print("[HTTP] GET... failed, error: "); Serial.prinln(httpCode);
*/


}

/*
 *  Handles other HTTP codes
 *
 */
void handleOtherCode(int httpCode) {
  // TODO as part of the general error handling
  Serial.print("Cannot handle this HTTP code:");
  Serial.println(httpCode);

}

/* Read the URL of the station that the radio is currently tuned into
 *
 * NOTE: This is a dummy function to be replaced with a an SPI read of the station URL.
 */
String getStationURL()
{
   return stations[0];  // RPR1

}

// Get the volume from the front panel controller
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
  // Transfer byte from the slave and pack it into an unsigned int
  volume  = SPI.transfer(0x00);
  digitalWrite(FPCS, HIGH);

  return volume;
}

/**
 * Get the change status from the front panel controller
 */
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


String getHostFromURL(String url) {
  int startPos;
  int endPos;

  // Skip past any protocol specification. Assuming that is http:// or is not specified
  if (url.startsWith("http://")) {
    startPos = 7;
  } else {
    startPos = 0;
  }

  endPos = url.indexOf('/', startPos);
  // Extract the host name
  return url.substring(startPos, endPos);
}

String getPathFromURL(String url) {
  int startPos;
  int endPos;

  // Skip past any protocol specification. Assuming that is http:// or is not specified
  if (url.startsWith("http://")) {
    startPos = 7;
  } else {
    startPos = 0;
  }

  // Now extract the path
  startPos = url.indexOf('/', startPos);
  return url.substring(startPos);

}

void pulseTestPin() {
  digitalWrite(TEST_PIN_D, HIGH);
  delayMicroseconds(1);
  digitalWrite(TEST_PIN_D, LOW);

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
