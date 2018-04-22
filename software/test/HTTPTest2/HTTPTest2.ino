
/*
  Web client - uses an alternative approach to HTTPTest
  to read in the data. 

 This sketch connects to a website 
 using a Adafruit M0 and does an HTTP get.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 Circuit:
 * Alone standing Adafruit Feather WiFi

 Based on the the WiFi101 library examples 

 */


#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoHttpClient.h>


#include "credentials.h" 

char ssid[] = WIFI_SSID;   //network SSID (name)
char pass[] = WIFI_PWD;    // network password (use for WPA, or use as key for WEP)
int keyIndex = 0;          //  network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "www.andrew-doble.homepage.t-online.de";    // name address (using DNS)

String response;
int statusCode = 0;

uint8_t buff[32];

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

// Use SerialUSB for the M0
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 1 second for connection:
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.println("\nRequesting (using get) a page from the  server...");
  // if you get a connection, report back via serial:

  HttpClient http  = HttpClient(client, server);
  http.get("/");
  // read the status code and body of the response
  statusCode = http.responseStatusCode();
  //response = http.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
 

}

void loop() {
  int nBytes;
  int bytesToRead;
  
  // Read in to the buffer 32 bytes or the nunber of bytes that 
  // are avaliable - whatever is the smallest number. 
  nBytes = client.available();
  bytesToRead = (nBytes > 32 ? 32 : nBytes);
  for (int i = 0; i < bytesToRead; i++) {
    buff[i] = client.read();
  }

  // Print the buffer contents out
  for (int i = 0; i < bytesToRead; i++) {
    Serial.write(buff[i]);
  }
  
   

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}





