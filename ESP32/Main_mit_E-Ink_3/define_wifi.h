/*
WiFiAccessPoint:
 Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port
*/

// const char *charSSID = "";  // Set these to your desired credentials
// const char *charPassword = "";
const uint16_t port = 80;
const char * host = "192.168.4.1"; // ip or dns
WiFiServer server(port);
WiFiClient client;
