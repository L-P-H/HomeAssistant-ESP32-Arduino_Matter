/*
Light dependend Resistor:
 *int sensorPin = A0

LCD Display:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 2K resistor:
 * ends to V0(pin 3) and ground
 * 220 resistor:
 * ends to A and 5V
 * K to ground

WiFiAccessPoint:
 Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port
*/

//#define IR_RECEIVE_PIN 2
//#include <IRremote.h>
#include <LiquidCrystal.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "secrets.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

float Data = 0;
float Temp = 0;
float Hum = 0;
int brightness_led = 0;   // 0 is off
int fadeAmount_led = 1;
int print = 0;
String Data_string = "";
// const char *ssid = "";  // Set these to your desired credentials
// const char *password = "";
WiFiServer server(80);


void setup() {
  // put your setup code here, to run once:
  //IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK); // Start the receiver

  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  Serial.begin(9600);

  lcd.begin(16, 2);  // set up the LCD's number of columns and rows

  delay(3000);
  Serial.println();
  Serial.println("Configuring access point...");
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.decodedRawData, DEC); // Print "old" raw data
    //IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
    //IrReceiver.printIRSendUsage(&Serial);   // Print the statement required to send this data
    Data = IrReceiver.decodedIRData.decodedRawData, DEC;
  */

  WiFiClient client = server.available();   // listen for incoming clients

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
            client.print("<p>Temperatur: ");
            client.print(Temp, 1);
            client.print("&deg;C</p><br>");
            client.print("<p>Luftfeuchtigkeit: ");
            client.print(Hum, 1);
            client.print("%</p><br>");

            // The HTTP response ends with another blank line:
            client.println();
            client.println();
            // break out of the while loop:
            break;
          } 
          else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } 
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.startsWith("GET /") && currentLine.endsWith("HTTP/1.1")) {
          Data_string = currentLine.substring(5,currentLine.length() -9);
          Data = Data_string.toInt();
          if (Data == 1) {lcd.clear(); print = 0; analogWrite(14, 255);}
          else if (Data >= 10000 && Data <= 11000) {Data -= 10000; Temp = Data / 10; analogWrite(15, 250); print = 1;}
          else if (Data >= 15000 && Data <= 16000) {Data -= 15000; Hum = Data / 10; analogWrite(16, 250); print = 1;}
          else {analogWrite(14, 0);}
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }

  if (print == 1) {
    print = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T = " );
    lcd.print(Temp, 1);
    lcd.print(char(0xDF));   //=char(223)=°
    lcd.print("C");
    Serial.print("  Temp: ");
    Serial.print(Temp, 1);
    Serial.println("°C");

    lcd.setCursor(0, 1);
    lcd.print("H = " );
    lcd.print(Hum, 1);
    lcd.print("%" );
    Serial.print("  Hum: ");
    Serial.print(Hum, 1);
    Serial.println("%");

    analogWrite(14, 255);
    analogWrite(15, 255);
    analogWrite(16, 255);
  }
}
