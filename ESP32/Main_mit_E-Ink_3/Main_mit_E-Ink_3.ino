/*
Light dependend Resistor:
 *int sensorPin = A7

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
#include <Adafruit_NeoPixel.h>

#include "variables.h"
#include "define_e_ink_display.h"
#include "define_pixel_ring.h"
#include "define_wifi.h"
#include "secrets.h"

#include "html_answer.h"

#define Light_Sensor_Pin A7


void setup() {
  for (int i = 0; i < Chart_length+1; i++) {iMoist_Array[i] = -1;}
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
  if (!WiFi.softAP(charSSID, charPassword)) {
    log_e("Soft AP creation failed.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.print("SSID: ");
  Serial.println(charSSID);
  Serial.print("Password: ");
  Serial.println(charPassword);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  client = server.available();   // listen for incoming clients

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
          if (currentLine.startsWith("GET /") && currentLine.endsWith("HTTP/1.1")) {
            stringData = currentLine.substring(5,currentLine.length() -9);
            iData = stringData.toInt();

                 if (iData == 1)                                                                               {iCase = 1; iTemp_ = 2; iHum_ = 2; bSendToDisplay = true; analogWrite(15, 250);}
            else if (iData == 2)                                                                               {iCase = 2; iMoist_ = 2; bSendToDisplay = true; analogWrite(15, 250);}
            else if (iData == 100)                                                                             {iCase = 100; analogWrite(15, 250);}
            else if (stringData == "favicon.ico")                                                              {iCase = 101; analogWrite(14, 0); analogWrite(15, 0); analogWrite(16, 0);}
            else if (iData == 101 || iData == 102)                                                             {iCase = 102; analogWrite(14, 0);}
            else if (iData >= 1000 && iData < 2000)                                                            {iCase = 1000; iData -= 1000; fMoist = iData; analogWrite(16, 250); iMoist_ = 1; bPrint = true; iTemp_color_count += 1; iHum_color_count += 1; iMoist_color_count = 0;}
            else if (iData >= 2000 && iData < 3000) {
                   if (iData >= 2000 && iData <= 2120)                                                         {iCase = 2000; iData -= 2000; for (int i = 0; i < Chart_length; i++) {iMoist_Array[i] = iMoist_Array[i+1];} iMoist_Array[Chart_length] = iData;}
              else if (iData == 2199)                                                                          {iCase = 2000;}
              else if (iData >= 2200 && iData <= 2300)                                                         {iCase = 2200; iData -= 2200; iMin_Moist_percent = iData;}
              else if (iData >= 2400 && iData <= 2500)                                                         {iCase = 2400; iData -= 2400; iStop_Moist_percent = iData;}
            }
            else if (currentLine.startsWith("GET /?iMin_Moist_percent=") && currentLine.endsWith("HTTP/1.1"))  {iCase = 2200; stringData = currentLine.substring(25,currentLine.length() -9); iData = stringData.toInt(); iMin_Moist_percent = iData;}
            else if (currentLine.startsWith("GET /?iStop_Moist_percent=") && currentLine.endsWith("HTTP/1.1")) {iCase = 2400; stringData = currentLine.substring(26,currentLine.length() -9); iData = stringData.toInt(); iStop_Moist_percent = iData;}
            else if (iData > 1000000 && iData < 2000000)                                                       {iCase = 1000000; iData -= 1000000; fTemp = iData / 10000; fHum = (iData-1000*int(iData/1000)) / 10; analogWrite(16, 250); iTemp_ = 1; iHum_ = 1; bPrint = true; iTemp_color_count = 0; iHum_color_count = 0; iMoist_color_count += 1;}
            else if (iData > 1000000000 && iData < 1100000000)                                                 {iCase = 1000000000; iData -= 1000000000; iYear = iData / 10000; iMonth = (iData-10000*iYear) / 100; iDay = (iData-10000*iYear-100*iMonth);}
            else if (iData > 1100000000 && iData < 1200000000)                                                 {iCase = 1100000000; iData -= 1100000000; iTimeUpdate = iData - millis(); analogWrite(15, 250);}
            else                                                                                               {analogWrite(14, 0);}
          }
        }
        else {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            html_answer(iCase);
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
    Serial.print("Client Disconnected.\nData: ");
    Serial.println(iData);
    if (bSendToDisplay == true && bPrint == false) {bSendToDisplay = false; SendToDisplay();}
    if (bPrint == true) {bSendToDisplay = false; bPrint = false; Print();}
  }

  iSystime = iTimeUpdate + millis();
  iHour = iSystime / 3600000;
  iMinute = (iSystime - 3600000 * iHour) / 60000;
  iSecond = (iSystime - 3600000 * iHour - 60000 * iMinute) / 1000;
  Serial.print(iYear); if(iMonth < 10) {Serial.print("-0");} else {Serial.print("-");}
  Serial.print(iMonth); if(iDay < 10) {Serial.print("-0");} else {Serial.print("-");}
  Serial.print(iDay); if(iHour < 10) {Serial.print(" 0");} else {Serial.print(" ");}
  Serial.print(iHour); if(iMinute < 10) {Serial.print(":0");} else {Serial.print(":");}
  Serial.print(iMinute); if(iSecond < 10) {Serial.print(":0");} else {Serial.print(":");}
  Serial.println(iSecond);

  updateClock();
  if (millis() % 60000 < 1100) {          //alle 1 min
    analogWrite(14, 255); analogWrite(15, 255); analogWrite(16, 255);
    Serial.println("Moist: ");
    for (int i = 0; i <= Chart_length; i++) {Serial.println(iMoist_Array[i]);}
    Serial.println("");
  }
  delay(1000);
}


void Print() {
  SendToDisplay();

  Serial.print("  Temp: ");
  Serial.print(fTemp, 1);
  Serial.println("°C");

  Serial.print("  Hum: ");
  Serial.print(fHum, 1);
  Serial.println("%");

  Serial.print("  Moist: ");
  Serial.print(fMoist, 1);
  Serial.println("%");
}


void SendToDisplay() {
  if (iTemp_color_count >= 3)   {iTemp_ = 2;}
  if (iHum_color_count >= 3)    {iHum_ = 2;}
  if (iMoist_color_count >= 3)  {iMoist_ = 2;}
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

    if (iTemp_ == 0) {display.setTextColor(GxEPD_WHITE);}
    else if (iTemp_ == 1) {display.setTextColor(GxEPD_BLACK);}
    else {display.setTextColor(GxEPD_RED);}
    display.print("Temp:  ");
    display.print(fTemp, 0);
    display.println("C");

    if (iHum_ == 0) {display.setTextColor(GxEPD_WHITE);}
    else if (iHum_ == 1) {display.setTextColor(GxEPD_BLACK);}
    else {display.setTextColor(GxEPD_RED);}
    display.print("Hum:   ");
    display.print(fHum, 0);
    display.println("%");

    if (iMoist_ == 0) {display.setTextColor(GxEPD_WHITE);}
    else if (iMoist_ == 1) {display.setTextColor(GxEPD_BLACK);}
    else {display.setTextColor(GxEPD_RED);}
    display.print("Moist: ");
    display.print(fMoist, 0);
    display.print("%");
  } while (display.nextPage());

  do {
    display.setPartialWindow(ax, 5.1*ay, 180, 20);
    display.fillRect(ax, 5.1*ay, 180, 20, GxEPD_BLACK);
    display.setCursor(ax, 5.7*ay);
    display.setTextSize(1);
    display.setTextColor(GxEPD_WHITE);
    display.print(iYear); if(iMonth < 10) {display.print("-0");} else {display.print("-");}
    display.print(iMonth); if(iDay < 10) {display.print("-0");} else {display.print("-");}
    display.print(iDay); if(iHour < 10) {display.print(" 0");} else {display.print(" ");}
    display.print(iHour); if(iMinute < 10) {display.print(":0");} else {display.print(":");}
    display.print(iMinute);
  } while (display.nextPage());
  display.hibernate();
}


void updateClock() {
  ws2812b.clear();

  if (analogRead(Light_Sensor_Pin) < 4095) {
  int iSecond_ = Rotate + (iSecond / 5);
  while (iSecond_ >= 12) {iSecond_ -= 12;}
  ws2812b.setPixelColor(iSecond_, ws2812b.Color(255, 0, 0));  // it only takes effect if pixels.show() is called

  int iMinute_ = Rotate + (iMinute / 5);
  while (iMinute_ >= 12) {iMinute_ -= 12;}
  ws2812b.setPixelColor(iMinute_, ws2812b.Color(0, 255, 0));  // it only takes effect if pixels.show() is called

  int iHour_ = Rotate + iHour;
  while (iHour_ >= 12) {iHour_ -= 12;}
  ws2812b.setPixelColor(iHour_, ws2812b.Color(0, 0, 255));  // it only takes effect if pixels.show() is called

  if (iSecond_ == iMinute_) {ws2812b.setPixelColor(iMinute_, ws2812b.Color(128, 128, 0));}
  if (iSecond_ == iHour_) {ws2812b.setPixelColor(iHour_, ws2812b.Color(128, 0, 128));}
  if (iMinute_ == iHour_) {ws2812b.setPixelColor(iHour_, ws2812b.Color(0, 128, 128));}
  if (iSecond_ == iMinute_ && iMinute_ == iHour_) {ws2812b.setPixelColor(iHour_, ws2812b.Color(85, 85, 85));}
  }

  ws2812b.show(); 
}
