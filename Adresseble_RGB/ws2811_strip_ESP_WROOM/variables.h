#define PIN_WS2811 15  // The ESP32 pin A1 connected to WS2812B
#define NUM_PIXELS 2   // The number of LEDs (pixels) on WS2812B LED strip

/*int Out[]={13, 12, 9, 6, A2, A5};
int In[]={10, 7, 4, A0, A4, A7};
byte speed_down = 10;
byte normal_speed = 7;
byte speed_up = 4;
byte next_mode = A0;
byte darker = A4;
byte brighter = A7;*/

Adafruit_NeoPixel Adafruit_NeoPixel(NUM_PIXELS, PIN_WS2811, NEO_GRB + NEO_KHZ800);

byte red = 255;
byte green = 0;
byte blue = 0;

int iGap = 0;
int iCount = 0;
int iSpeed = 1;
int iBrightness = 100;
int iMode = 1;
int iColor = 0;