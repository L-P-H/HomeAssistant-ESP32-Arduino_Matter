#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <ESP32Ping.h>
#include <ESPmDNS.h>
#include "secrets.h"

#define LED_BUILTIN 13   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
// const char *ssid = "HomeAssistant-WIFI";
// const char *password = "";
const IPAddress IPAddress_Server = IPAddress(192, 169, 250, 1);
int iAktiveIPs[256];
IPAddress IPAddress_Test;

WiFiServer server(80);
WiFiClient client = server.available();   // listen for incoming clients


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial.availableForWrite() && millis() < 5000) {}
  Serial.println("\nWaited for Serial: ");
  Serial.println(millis());
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password, 5, 0, 15)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  //IPAddress myIP = WiFi.softAPIP();
  WiFi.setMinSecurity(WIFI_AUTH_WPA2_WPA3_PSK);
  WiFi.softAPConfig(IPAddress_Server, IPAddress_Server, IPAddress(255, 255, 255, 0));
  Serial.print("AP IP address: ");
  Serial.println(IPAddress_Server);
  server.begin();

  Serial.println("Server started");

  if (!MDNS.begin("ESP32_Browser")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
}

void loop() {
  Client_response();
  static byte round = 0;
  if (millis() % 1000 == 0) {Client_list(); round++;}
  if (round >= 10) {browseService("wled", "tcp"); round=0;}
}

void Client_response() {
  //WiFiClient client = server.available();   // listen for incoming clients
  client = server.available();

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<title>WLED-Server</title>");
            client.print("<h1>WLED-Server</h1>");
            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br><br>");
            browseServiceHTML("wled", "tcp");
            client.print("<p>Test IP: ");
            client.print(IPAddress_Test);
            client.print("</p>");
            client.print("<p> Gefundene IPs: <br>");
            for (int i = 0; i<256; i++) {
              if(iAktiveIPs[i] == 1) {
                client.print("192.168.250."); client.print(i); client.print("<br>");
              }
            }
            client.print("</p>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void Client_list(){
  static byte IPbyte = 0;
  IPAddress_Test = IPAddress(169, 254, 254, IPbyte);
  if(Ping.ping(IPAddress_Test, 1)){
    Serial.print("---------------------------");
    Serial.println(IPAddress_Test);
    iAktiveIPs[IPbyte] = 1;
    for (int i = 0; i<256; i++) {Serial.print(iAktiveIPs[i]);}
    Serial.println();
  }
  else {iAktiveIPs[IPbyte] = 0;}
  //Serial.println(Ping.ping(IPAddress_Test, 3));
  IPbyte ++;
}

void browseService(const char *service, const char *proto) {
  Serial.printf("Browsing for service _%s._%s.local. ... ", service, proto);
  int n = MDNS.queryService(service, proto);
  if (n == 0) {
    Serial.println("no services found");
  } else {
    Serial.print(n);
    Serial.println(" service(s) found");
    for (int i = 0; i < n; ++i) {
      // Print details for each service found
      Serial.print("  ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(MDNS.instanceName(i));
      Serial.print(" - ");
      Serial.print(MDNS.hostname(i));
      Serial.print(" (");
      Serial.print(MDNS.address(i));
      Serial.print(":");
      Serial.print(MDNS.port(i));
      Serial.println(")");
    }
  }
  Serial.println();
}

void browseServiceHTML(const char *service, const char *proto) {
  client.printf("<p>Browsing for service _%s._%s.local. ... ", service, proto);
  int n = MDNS.queryService(service, proto);
  if (n == 0) {
    client.println("no services found");
  } else {
    client.print(n);
    client.println(" service(s) found<br>");
    for (int i = 0; i < n; ++i) {
      // Print details for each service found
      client.print("  ");
      client.print(i + 1);
      client.print(": ");
      client.print(MDNS.instanceName(i));
      client.print(" - ");
      client.print(MDNS.hostname(i));
      client.print(" (");
      client.print(MDNS.address(i));
      client.print(":");
      client.print(MDNS.port(i));
      client.println(")<br>");
    }
  }
  client.println("</p>");
}
