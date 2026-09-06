void vBreathe() {
  int iBreath_bright;

  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {                   // for each pixel
    if (iColor <= 255) {red = (255-iColor); green = iColor; blue = 0;}
    if (iColor >= 256 && iColor <= 510) {red = 0; green = (510-iColor); blue = (iColor-255);}
    if (iColor >= 511) {red= (iColor-510); green = 0; blue = (765-iColor);}

    Adafruit_NeoPixel.setPixelColor(pixel, Adafruit_NeoPixel.Color(green, red, blue));           // it only takes effect if pixels.show() is called
  }

  if (iCount > 200) {delay(150*(1-iSpeed)); iCount = 0;}
  if (iCount <=  100) {iBreath_bright = 100-iCount;}           // it only takes effect if pixels.show() is called
  else                {iBreath_bright = iCount-100;}
  iBreath_bright = iBreath_bright * iBrightness / 100;
  Adafruit_NeoPixel.setBrightness(iBreath_bright);
  Adafruit_NeoPixel.show();
}