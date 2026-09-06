/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-ws2812b-led-strip
 */

#include <Adafruit_NeoPixel.h>

#define PIN_WS2812B 8  // The ESP32 pin GPIO16 connected to WS2812B
#define NUM_PIXELS 12   // The number of LEDs (pixels) on WS2812B LED strip

Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

int red = 255;
int green = 0;
int blue = 0;
int red_ = 0;
int green_ = 0;
int blue_ = 0;
int fade = 1;
int gap = 0;
int count = 0;

void setup() {
  ws2812b.begin();  // initialize WS2812B strip object (REQUIRED)
  //ws2812b.setBrightness(30);
  ws2812b.clear();  // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
  Serial.begin(9600);
}

void loop() {
  // turn pixels to green one-by-one with delay between each pixel
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {                   // for each pixel

    //gap=count+(765/NUM_PIXELS*pixel);
    //if (gap > 765) {gap -= 765;}
    gap=count-(765/NUM_PIXELS*pixel);
    if (gap < 1) {gap += 765;}

    if (gap <= 255) {red = (255-gap); green = gap; blue = 0;}
    if (gap >= 256 && gap <= 510) {red = 0; green = (510-gap); blue = (gap-255);}
    if (gap >= 511) {red= (gap-510); green = 0; blue = (765-gap);}

    ws2812b.setPixelColor(pixel, ws2812b.Color(red, green, blue));           // it only takes effect if pixels.show() is called
    ws2812b.show();                                                // update to the WS2812B Led Strip

    delay(1);  // pause between each pixel
  }

  count += 1;
  if (count >= (256+255+255)) {count = 1;}
  
  Serial.print("Millis: ");
  Serial.print(1);
  Serial.print("  Rot: ");
  Serial.print(red);
  Serial.print(" Grün: ");
  Serial.print(green);
  Serial.print(" Blau: ");
  Serial.print(blue);
  Serial.print(" Count: ");
  Serial.println(count);
}