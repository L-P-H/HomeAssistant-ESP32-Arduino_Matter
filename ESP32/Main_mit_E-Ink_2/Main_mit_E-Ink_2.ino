/*
Light dependend Resistor:
 *int sensorPin = A0

E-Ink Display:
 * GND to GND
 * VCC to 3,3V
 * SDI to digital pin 11
 * SCLK to digital pin 13
 * CS to digital pin 10
 * DC to digital pin 9
 * RST to digital pin 8
 * BUSY to digital pin 7

WiFiAccessPoint:
 Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port
*/

#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <Adafruit_NeoPixel.h>
#include "secrets.h"

#define EPD_SS 10
#define EPD_DC 9
#define EPD_RST 8
#define EPD_BUSY 7
#define MAX_DISPLAY_BUFFER_SIZE 800
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
GxEPD2_3C<GxEPD2_290_C90c, MAX_HEIGHT(GxEPD2_290_C90c)>
display(GxEPD2_290_C90c(EPD_SS, EPD_DC, EPD_RST, EPD_BUSY));

#define PIN_WS2812B 8  // The ESP32 pin GPIO16 connected to WS2812B
#define NUM_PIXELS 12  // The number of LEDs (pixels) on WS2812B LED strip
#define Rotate 1       // Rotation of 12 o'clock
Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

int Data = 0;
String Data_string = "";
bool bPrint = false;
bool bSendToDisplay = false;

float Temp = 0;
int Temp_ = 2;
float Hum = 0;
int Hum_ = 2;
float Moist = 0;
int Moist_ = 2;

int brightness_led = 0;   // 0 is off
int fadeAmount_led = 1;

int Second = 0;
int Minute = 0;
int Hour = 0;
int Day = 0;
int Month = 0;
int Year = 0;
int TimeUpdate = 0;
int Systime = 0;

// const char *ssid = "";  // Set these to your desired credentials
// const char *password = "";
WiFiServer server(80);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ws2812b.begin();
  ws2812b.setBrightness(30);
  ws2812b.clear();
  display.init(9600, true, 2, false);

  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);

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
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("---------------------------------------");
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c != '\n' && c != '\r') {
          currentLine += c;
          if (currentLine.startsWith("GET /") && currentLine.endsWith("HTTP/1.1")) {
            Data_string = currentLine.substring(5,currentLine.length() -8);
            Data = Data_string.toInt();

            if (Data == 1) {Temp_ = 2; Hum_ = 2; bSendToDisplay = true; analogWrite(14, 255);}
            else if (Data == 2) {Moist_ = 2; bSendToDisplay = true; analogWrite(14, 255);}
            else if (Data >= 1000 && Data <= 2000) {Data -= 1000; Moist = Data / 10; analogWrite(16, 250); Moist_ = 1; bPrint = true;}
            else if (Data > 1000000 && Data < 2000000) {Data -= 1000000; Temp = Data / 10000; Hum = (Data-1000*int(Data/1000)) / 10; analogWrite(15, 250); Temp_ = 1; Hum_ = 1; bPrint = true;}
            else if (Data > 1000000000 && Data < 1100000000) {Data -= 1000000000; Year = Data / 10000; Month = (Data-10000*Year) / 100; Day = (Data-10000*Year-100*Month);}
            else if (Data > 1100000000 && Data < 1200000000) {Data -= 1100000000; TimeUpdate = Data - millis();}
            else {analogWrite(14, 0);}
          }
        }
        else {                    // if the byte is a newline character

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
            client.print("<p>Bodenfeuchte: ");
            client.print(Moist, 1);
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
      }
    }
    // close the connection:
    client.stop();
    Serial.print("Client Disconnected.\nData: ");
    Serial.println(Data);
    if (bSendToDisplay == true && bPrint == false) {bSendToDisplay = false; SendToDisplay(Temp, Temp_ , Hum, Hum_, Moist, Moist_);}
    if (bPrint == true) {bSendToDisplay = false; bPrint = false; Print();}
  }

  Systime = TimeUpdate + millis();
  Hour = Systime / 3600000;
  Minute = (Systime - 3600000 * Hour) / 60000;
  Second = (Systime - 3600000 * Hour - 60000 * Minute) / 1000;
  Serial.print(Year); if(Month < 10) {Serial.print("-0");} else {Serial.print("-");}
  Serial.print(Month); if(Day < 10) {Serial.print("-0");} else {Serial.print("-");}
  Serial.print(Day); if(Hour < 10) {Serial.print(" 0");} else {Serial.print(" ");}
  Serial.print(Hour); if(Minute < 10) {Serial.print(":0");} else {Serial.print(":");}
  Serial.print(Minute); if(Second < 10) {Serial.print(":0");} else {Serial.print(":");}
  Serial.println(Second);

  if (millis() % 300000 < 2000) {analogWrite(14, 255); analogWrite(15, 255); analogWrite(16, 255);} //alle 5 min
  updateClock();
  delay(1000);
}

void Print() {
  SendToDisplay(Temp, Temp_ , Hum, Hum_, Moist, Moist_);

  Serial.print("  Temp: ");
  Serial.print(Temp, 1);
  Serial.println("°C");

  Serial.print("  Hum: ");
  Serial.print(Hum, 1);
  Serial.println("%");

  analogWrite(14, 255);
  analogWrite(15, 255);
  analogWrite(16, 255);
}


void SendToDisplay(float Temp, int Temp_ , float Hum, int Hum_, float Moist, int Moist_) {
  Serial.println("");
  // center the bounding box by transposition of the origin:
  //uint16_t x = ((display.width() - tbw) / 2) - tbx;
  //uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setRotation(3);
  display.setTextSize(2);
  display.setFont(&FreeMonoBold9pt7b);
  int16_t tbx, tby; uint16_t tbw, tbh;

  display.getTextBounds("Placeholder", 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t ax = (0);
  uint16_t ay = (tbh);
  display.firstPage();

  do {
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(ax, ay);

    if (Temp_ == 0) {display.setTextColor(GxEPD_WHITE);}
    else if (Temp_ == 1) {display.setTextColor(GxEPD_BLACK);}
    else {display.setTextColor(GxEPD_RED);}
    display.print("Temp:  ");
    display.print(Temp, 1);
    display.println("C");

    if (Hum_ == 0) {display.setTextColor(GxEPD_WHITE);}
    else if (Hum_ == 1) {display.setTextColor(GxEPD_BLACK);}
    else {display.setTextColor(GxEPD_RED);}
    display.print("Hum:   ");
    display.print(Hum, 1);
    display.println("%");

    if (Moist_ == 0) {display.setTextColor(GxEPD_WHITE);}
    else if (Moist_ == 1) {display.setTextColor(GxEPD_BLACK);}
    else {display.setTextColor(GxEPD_RED);}
    display.print("Moist: ");
    display.print(Moist, 1);
    display.print("%");
  } while (display.nextPage());

  do {
    display.setPartialWindow(ax, 5.1*ay, 180, 20);
    display.fillRect(ax, 5.1*ay, 180, 20, GxEPD_BLACK);
    display.setCursor(ax, 5.7*ay);
    display.setTextSize(1);
    display.setTextColor(GxEPD_WHITE);
    display.print(Year); if(Month < 10) {display.print("-0");} else {display.print("-");}
    display.print(Month); if(Day < 10) {display.print("-0");} else {display.print("-");}
    display.print(Day); if(Hour < 10) {display.print(" 0");} else {display.print(" ");}
    display.print(Hour); if(Minute < 10) {display.print(":0");} else {display.print(":");}
    display.print(Minute);
  } while (display.nextPage());
  display.hibernate();
}


void updateClock() {
  ws2812b.clear();

  int Second_ = Rotate + (Second / 5);
  while (Second_ >= 12) {Second_ -= 12;}
  ws2812b.setPixelColor(Second_, ws2812b.Color(255, 0, 0));  // it only takes effect if pixels.show() is called

  int Minute_ = Rotate + (Minute / 5);
  while (Minute_ >= 12) {Minute_ -= 12;}
  ws2812b.setPixelColor(Minute_, ws2812b.Color(0, 255, 0));  // it only takes effect if pixels.show() is called

  int Hour_ = Rotate + Hour;
  while (Hour_ >= 12) {Hour_ -= 12;}
  ws2812b.setPixelColor(Hour_, ws2812b.Color(0, 0, 255));  // it only takes effect if pixels.show() is called

  if (Second_ == Minute_) {ws2812b.setPixelColor(Minute_, ws2812b.Color(128, 128, 0));}
  if (Second_ == Hour_) {ws2812b.setPixelColor(Hour_, ws2812b.Color(128, 0, 128));}
  if (Minute_ == Hour_) {ws2812b.setPixelColor(Hour_, ws2812b.Color(0, 128, 128));}
  if (Second_ == Minute_ && Minute_ == Hour_) {ws2812b.setPixelColor(Hour_, ws2812b.Color(85, 85, 85));}

  ws2812b.show(); 
}
