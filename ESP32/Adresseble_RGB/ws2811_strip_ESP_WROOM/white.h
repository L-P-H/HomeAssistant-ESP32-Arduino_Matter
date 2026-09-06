void vWhite() {
  if (iCount > (255+255)) {iCount = 0;}
  if (iCount <= 255) {red = 255; green = iCount; blue = iCount; delay((255-iCount)/4);}
  if (iCount > 255) {red = 511 - iCount; green = 511 - iCount; blue = 255; delay(10);}

  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {                   // for each pixel
    Adafruit_NeoPixel.setPixelColor(pixel, Adafruit_NeoPixel.Color(green, red, blue));           // it only takes effect if pixels.show() is called
  }
  Adafruit_NeoPixel.show();
}