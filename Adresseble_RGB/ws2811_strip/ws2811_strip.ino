/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-ws2812b-led-strip
 */

#include <Adafruit_NeoPixel.h>
#include "variables.h"
#include "rainbow.h"
#include "fade.h"
#include "breathe.h"
#include "white.h"


void setup() {
  Adafruit_NeoPixel.begin();  // initialize WS2812B strip object (REQUIRED)
  //Adafruit_NeoPixel.setBrightness(30);
  Adafruit_NeoPixel.clear();  // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
  Serial.begin(9600);

  for (int i=0; i<sizeof(Out)/sizeof(int); i++) {
    pinMode(Out[i], OUTPUT);
    digitalWrite(Out[i], HIGH);
  }
  for (int i=0; i<sizeof(In)/sizeof(int); i++) {
    pinMode(In[i], INPUT_PULLDOWN);
  }
  
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {                   // for each pixel
    Adafruit_NeoPixel.setPixelColor(pixel, Adafruit_NeoPixel.Color(255, 0, 0));           // it only takes effect if pixels.show() is called
    Adafruit_NeoPixel.show();                                                // update to the WS2812B Led Strip
  }
  for (int i = 0; i < 6; i++) {digitalWrite(13, HIGH); delay(500); digitalWrite(13, LOW); delay(500);}
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {                   // for each pixel
    Adafruit_NeoPixel.setPixelColor(pixel, Adafruit_NeoPixel.Color(0, 255, 0));           // it only takes effect if pixels.show() is called
    Adafruit_NeoPixel.show();                                                // update to the WS2812B Led Strip
  }
  for (int i = 0; i < 6; i++) {digitalWrite(13, HIGH); delay(500); digitalWrite(13, LOW); delay(500);}
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {                   // for each pixel
    Adafruit_NeoPixel.setPixelColor(pixel, Adafruit_NeoPixel.Color(0, 0, 255));           // it only takes effect if pixels.show() is called
    Adafruit_NeoPixel.show();                                                // update to the WS2812B Led Strip
  }
  for (int i = 0; i < 6; i++) {digitalWrite(13, HIGH); delay(500); digitalWrite(13, LOW); delay(500);}
}

void loop() {
  Adafruit_NeoPixel.setBrightness(iBrightness);
  
       if (iMode == 1) {vRainbow();}
  else if (iMode == 2) {vFade();}
  else if (iMode == 3) {vBreathe();}
  else if (iMode == 4) {vWhite();}
  else                 {iMode = 1;}

  if (iSpeed < 0) {delay(-iSpeed); iCount += 1;}
  if (iSpeed > 0) {iCount += iSpeed;}
  
  if (digitalRead(brighter) == HIGH && iBrightness < 255) {iBrightness +=5; digitalWrite(13, HIGH); delay(100); digitalWrite(13, LOW); delay(100);}
  if (digitalRead(darker) == HIGH && iBrightness > 0) {iBrightness -= 5; digitalWrite(13, HIGH); delay(100); digitalWrite(13, LOW); delay(100);}
  if (digitalRead(speed_up) == HIGH) {iSpeed += 1; digitalWrite(13, HIGH); delay(100); digitalWrite(13, LOW); delay(100);}
  if (digitalRead(normal_speed) == HIGH) {iSpeed = 0;}
  if (digitalRead(speed_down) == HIGH) {iSpeed -= 1; digitalWrite(13, HIGH); delay(100); digitalWrite(13, LOW); delay(100);}
  if (digitalRead(next_mode) == HIGH) {iMode += 1; digitalWrite(13, HIGH); delay(100); digitalWrite(13, LOW); delay(400);}


  Serial.print("  Rot: ");
  Serial.print(red);
  Serial.print(" Grün: ");
  Serial.print(green);
  Serial.print(" Blau: ");
  Serial.print(blue);
  Serial.print(" iMode: ");
  Serial.print(iMode);
  Serial.print(" iBrightness: ");
  Serial.print(iBrightness);
  Serial.print(" iSpeed: ");
  Serial.println(iSpeed);
}