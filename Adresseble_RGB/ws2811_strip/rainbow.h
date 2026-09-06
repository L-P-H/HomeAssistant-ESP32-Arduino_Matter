void vRainbow() {
  if (iCount >= (256+255+255)) {iCount = 1;}
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {                   // for each pixel

    //iGap=iCount+(765/NUM_PIXELS*pixel);
    //if (iGap > 765) {iGapp -= 765;}
    iGap=iCount-(765/NUM_PIXELS*pixel);
    while (iGap < 1) {iGap += 765;}

    if (iGap <= 255) {red = (255-iGap); green = iGap; blue = 0;}
    if (iGap >= 256 && iGap <= 510) {red = 0; green = (510-iGap); blue = (iGap-255);}
    if (iGap >= 511) {red= (iGap-510); green = 0; blue = (765-iGap);}

    Adafruit_NeoPixel.setPixelColor(pixel, Adafruit_NeoPixel.Color(green, red, blue));           // it only takes effect if pixels.show() is called
  }
  Adafruit_NeoPixel.show();
}