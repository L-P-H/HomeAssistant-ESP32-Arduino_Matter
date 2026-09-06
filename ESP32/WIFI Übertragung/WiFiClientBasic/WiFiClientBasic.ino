/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <WiFi.h>
#include "secrets.h"

// const char* ssid     = ""; // Change this to your WiFi SSID
// const char* password = ""; // Change this to your WiFi password
const uint16_t port = 80;
const char * host = "192.168.4.1"; // ip or dns


void setup()
{
  Serial.begin(9600);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  delay(10);

  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");
  analogWrite(14, 0);
  analogWrite(15, 0);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}


void loop() {

  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println("Waiting 5 seconds before retrying...");
    analogWrite(14, 0);
    analogWrite(15, 255);
    delay(5000);
    return;
  }


  // This will send a request to the server
  //uncomment this line to send an arbitrary string to the server
  //client.print("Send this data to the server");
  //uncomment this line to send a basic document request to the server
  client.print("GET /10500 HTTP/1.1");
  client.print("\n\n");

  int maxloops = 0;

  //wait for the server's reply to become available
  while (!client.available() && maxloops < 1000) {
    maxloops++;
    delay(1); //delay 1 msec
  }
  while (client.available()) {
    if (client.available() > 0) {
      //read back one line from the server
      String line = client.readStringUntil('\n');
      Serial.println(line);
      analogWrite(14, 255);
      analogWrite(15, 0);
      analogWrite(16, 255);
    }
    else {
      Serial.println("client.available() timed out ");
      analogWrite(14, 0);
      analogWrite(15, 255);
      break;
    }
  }

  Serial.println("Closing connection.");
  client.stop();

  Serial.println("Waiting 5 seconds before restarting...");
  delay(5000);
}
