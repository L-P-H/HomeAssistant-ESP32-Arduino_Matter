void vFade() {
  if (iCount >= (256+255+255)) {iCount = 1;}
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {                   // for each pixel
    if (iCount <= 255) {red = (255-iCount); green = iCount; blue = 0;}
    if (iCount >= 256 && iCount <= 510) {red = 0; green = (510-iCount); blue = (iCount-255);}
    if (iCount >= 511) {red= (iCount-510); green = 0; blue = (765-iCount);}

    Adafruit_NeoPixel.setPixelColor(pixel, Adafruit_NeoPixel.Color(green, red, blue));           // it only takes effect if pixels.show() is called
  }
  iColor = iCount;
  Adafruit_NeoPixel.show();
}