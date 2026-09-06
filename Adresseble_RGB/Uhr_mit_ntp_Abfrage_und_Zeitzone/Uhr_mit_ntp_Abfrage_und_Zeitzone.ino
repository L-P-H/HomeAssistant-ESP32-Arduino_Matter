#include <WiFi.h>
#include <time.h>
#include <sntp.h>
#include <Adafruit_NeoPixel.h>
#include "secrets.h"

#define PIN_WS2812B 9  // The ESP32 pin GPIO16 connected to WS2812B
#define NUM_PIXELS 12  // The number of LEDs (pixels) on WS2812B LED strip
#define Rotate 7       // Rotation of 12 o'clock

Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

// const char* ssid0       = "";
// const char* password0   = "";
// const char* ssid1     = "";
// const char* password1 = "";
const uint16_t port = 80;
const char * host = "192.168.4.1"; // ip or dns

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Berlin including daylight adjustment rules (optional)

int WIFI_intern = 0;

int Second = 0;
int Minute = 0;
int Hour = 0;
int Day = 0;
int Month = 0;
int Year = 0;


void setup() {
  Serial.begin(9600);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  delay(1000);

  ws2812b.begin();
  ws2812b.setBrightness(30);
  ws2812b.clear();

  // set notification call-back function
  sntp_set_time_sync_notification_cb( timeavailable );
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  WIFI(0);
}


void loop() {
  delay(1000);
  printLocalTime();     // it will take some time to sync time :)

  updateClock();

  if (millis()%14400000 <= 1000) {WIFI_intern = 0;}  //=4h

  if (WIFI_intern == 0 || WIFI_intern == 1) {WIFI(WIFI_intern);}
}


void printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }
  Serial.print(&timeinfo, "%Y-%m-%d %H:%M:%S");
  Serial.print("  ");
  Serial.println(millis()%14400000);  //=4h

  Second = timeinfo.tm_sec;
  Minute = timeinfo.tm_min;
  Hour = timeinfo.tm_hour;
  Day = timeinfo.tm_mday;
  Month = timeinfo.tm_mon;
  Year = timeinfo.tm_year;
}


// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
  WIFI_intern = 1;
}


void updateClock() {
  ws2812b.clear();

  int Second_ = Rotate + (Second / 5);
  while (Second_ >= NUM_PIXELS) {Second_ -= NUM_PIXELS;}
  ws2812b.setPixelColor(Second_, ws2812b.Color(255, 0, 0));  // it only takes effect if pixels.show() is called

  int Minute_ = Rotate + (Minute / 5);
  while (Minute_ >= NUM_PIXELS) {Minute_ -= NUM_PIXELS;}
  ws2812b.setPixelColor(Minute_, ws2812b.Color(0, 255, 0));  // it only takes effect if pixels.show() is called

  int Hour_ = Rotate + Hour;
  while (Hour_ >= NUM_PIXELS) {Hour_ -= NUM_PIXELS;}
  ws2812b.setPixelColor(Hour_, ws2812b.Color(0, 0, 255));  // it only takes effect if pixels.show() is called

  if (Second_ == Minute_) {ws2812b.setPixelColor(Minute_, ws2812b.Color(128, 128, 0));}
  if (Second_ == Hour_) {ws2812b.setPixelColor(Hour_, ws2812b.Color(128, 0, 128));}
  if (Minute_ == Hour_) {ws2812b.setPixelColor(Hour_, ws2812b.Color(0, 128, 128));}
  if (Second_ == Minute_ && Minute_ == Hour_) {ws2812b.setPixelColor(Hour_, ws2812b.Color(85, 85, 85));}

  ws2812b.show();
}


void WIFI(int intern) {
  WIFI:
  int count = 0;
  analogWrite(14, 0);
  analogWrite(15, 255);
  analogWrite(16, 255);
  // We start by connecting to a WiFi network
  if (intern == 0) {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(500);
    WiFi.begin(ssid0, password0);
    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi... ");
    while (WiFi.status() != WL_CONNECTED) {
      count ++;
      if (count >= 20) {goto WIFI;}
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    analogWrite(14, 0);
    analogWrite(15, 0);
    analogWrite(16, 255);
    
    sntp_restart();
    WIFI_intern = -1;
  }
  else if (intern == 1) {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(500);
    WiFi.begin(ssid1, password1);
    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi... ");
    while (WiFi.status() != WL_CONNECTED) {
      count ++;
      if (count >= 20) {goto WIFI;}
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    analogWrite(14, 255);
    analogWrite(15, 0);
    analogWrite(16, 255);

    WIFI_intern = -1;
  }
}
