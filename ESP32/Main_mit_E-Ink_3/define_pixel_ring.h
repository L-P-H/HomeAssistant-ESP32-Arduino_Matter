#define PIN_WS2812B 8  // The ESP32 pin GPIO16 connected to WS2812B
#define NUM_PIXELS 12  // The number of LEDs (pixels) on WS2812B LED strip
#define Rotate 1       // Rotation of 12 o'clock
Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);