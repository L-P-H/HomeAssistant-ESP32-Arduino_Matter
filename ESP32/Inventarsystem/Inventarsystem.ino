#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESP32Ping.h>
#include <ESPmDNS.h>

#define LED_BUILTIN 13   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "Inventarsystem";

WiFiServer server(80);
WiFiClient client;
IPAddress IPAddresse = IPAddress(192, 168, 50, 1);

String stringData = "";
String stringName = "";
String stringAnzahl = "";
int iAnzahl = 0;
String stringOrt = "";
String stringSchrank = "";

#include "HTML.h" // client wird sonst nicht gefunden


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial.availableForWrite() && millis() < 5000) {}
  Serial.println("\nWaited for Serial: ");
  Serial.println(millis());
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  WiFi.softAPConfig(IPAddresse, IPAddresse, IPAddress(255, 255, 255, 0));
  if (!WiFi.softAP(ssid, 0, 1, 0, 10)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();

  Serial.println("Server started");
}

void loop() {
  Client_response();
}

void Client_response() {
  //WiFiClient client = server.available();   // listen for incoming clients
  client = server.available();

  if (client) {                             // if you get a client,
    String currentLine = "";                // make a String to hold incoming data from the client
    int iCase = 0;
    Serial.println("---------------------------------------");
    Serial.println("New Client.");           // print a message out the serial port
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c != '\n' && c != '\r') {
          currentLine += c;
          if (currentLine.startsWith("GET /?") && currentLine.endsWith("HTTP/1.1")) {
            stringData = currentLine.substring(6,currentLine.length() -9);
            stringName = stringData.substring(5, stringData.indexOf("&"));   //String("name=")
            Serial.print("\n\nName: "); Serial.println(stringName);
            stringAnzahl = stringData.substring(5+stringName.length()+1, stringData.length());
            stringAnzahl = stringAnzahl.substring(7, stringAnzahl.indexOf("&"));   //String("anzahl=")
            iAnzahl = stringAnzahl.toInt();
            Serial.print("Anzahl: "); Serial.println(iAnzahl);
            stringOrt = stringData.substring(5+stringName.length()+1+7+stringAnzahl.length()+1, stringData.length());
            stringOrt = stringOrt.substring(4, stringOrt.indexOf("&"));    //String("ort=")
            Serial.print("Ort: "); Serial.println(stringOrt);
            stringSchrank = stringData.substring(5+stringName.length()+1+7+stringAnzahl.length()+1+4+stringOrt.length()+1, stringData.length());
            stringSchrank = stringSchrank.substring(8, stringSchrank.length());   //String("schrank=")
            Serial.print("Schrank: "); Serial.println(stringSchrank);
          }
        }
        else {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            html_answer(stringName, iAnzahl, stringOrt, stringSchrank);
            Serial.println("HTML answer");
            break;
          } 
          else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}