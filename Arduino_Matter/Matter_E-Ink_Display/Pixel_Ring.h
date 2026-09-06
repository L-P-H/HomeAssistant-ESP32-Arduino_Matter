#define NUM_PIXELS 12  // The number of LEDs (pixels) on WS2812B LED strip
#define Rotate 1       // Rotation of 12 o'clock
#define Brightness 30
ezWS2812 ws2812b(NUM_PIXELS);

void Pixel_Ring_setup() {
  ws2812b.begin();
}

/*void Pixel_Ring_loop() {
  if (millis() % 1000 == 0) {
    ws2812b.set_all(0, 0, 0);

    int iSecond_ = Rotate + ((millis() - current_Second_in_miilis) / 1000 / 5);
    while (iSecond_ >= 12) {iSecond_ -= 12;}
    ws2812b.set_pixel(iSecond_, 255, 0, 0);  // it only takes effect if pixels.show() is called

    int iMinute_ = Rotate + (current_Minute / 5);
    while (iMinute_ >= 12) {iMinute_ -= 12;}
    ws2812b.set_pixel(iMinute_, 0, 255, 0);  // it only takes effect if pixels.show() is called

    int iHour_ = Rotate + current_Hour;
    while (iHour_ >= 12) {iHour_ -= 12;}
    ws2812b.set_pixel(iHour_, 0, 0, 255);  // it only takes effect if pixels.show() is called

    if (iSecond_ == iMinute_) {ws2812b.set_pixel(iMinute_, 128, 128, 0);}
    if (iSecond_ == iHour_)   {ws2812b.set_pixel(iHour_,   128, 0, 128);}
    if (iMinute_ == iHour_)   {ws2812b.set_pixel(iHour_,   0, 128, 128);}
    if (iSecond_ == iMinute_ && iMinute_ == iHour_) {ws2812b.set_pixel(iHour_, 85, 85, 85);}
  }
}*/

void Pixel_Ring_loop() {
  static uint32_t current_led = 0;

  // Go through all the LEDs
  noInterrupts();
  for (uint32_t i = 0; i < NUM_PIXELS; i++) {
    // If 'i' is at the current LED - we turn that LED to green
    if (i == current_led) {
      // Parameters are: number_of_leds_to_set, R, G, B, brightness, end_transfer
      ws2812b.set_pixel(1, 0, 255, 0, 100, false);
    } else {
      // Turn all other LEDs off
      ws2812b.set_pixel(1, 0, 0, 0, 0, false);
    }
  }
  // End the transfer
  ws2812b.end_transfer();
  interrupts();

  // Move the current LED up by one
  current_led++;
  if (current_led >= NUM_PIXELS) {
    current_led = 0;
  }
  // Wait a bit
  delay(100);
}