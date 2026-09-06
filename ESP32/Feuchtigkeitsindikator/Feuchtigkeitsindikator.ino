#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include "secrets.h"

#define PIN_WS2812B 17  // The ESP32 pin GPIO17 connected to WS2812B (Pin 8)
#define NUM_PIXELS 12  // The number of LEDs (pixels) on WS2812B LED strip
#define Rotate 2       // Rotation of the left end (only positive numbers)
#define red_part 4     //number has to be even
#define yellow_part 4  //number has to be even
#define green_part 3   //number has to be odd

#define Moist_Sensor_Pin A1
#define Moist_Sensor_VCC 13

#define Pump_Pin 3

#define Light_Sensor_Pin A5

#define Button_Pin 12

int used_PIXELS = red_part + yellow_part + green_part;
int unused_PIXELS = NUM_PIXELS - used_PIXELS;

int moist = 0;
int moist_min = 2280;
int moist_max = 880;
int moist_percent = 1;
int moist_pixel = 0;
int moist_pixel_r = 0;
int moist_pixel_g = 0;

int pump_count = 0;
int WIFI_count = 0;
int send_count = 0;
int main_count = 0;

Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

// const char* ssid1     = "";
// const char* password1 = "";
const uint16_t port = 80;
const char * host = "192.168.4.1"; // ip or dns

void setup() {
  Serial.begin(9600);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(Moist_Sensor_Pin, INPUT);
  pinMode(Moist_Sensor_VCC, OUTPUT);
  pinMode(Button_Pin, INPUT_PULLUP);
  pinMode(Pump_Pin, OUTPUT);

  digitalWrite(Pump_Pin, HIGH);

  ws2812b.begin();
  ws2812b.setBrightness(30);
  ws2812b.clear();

  WIFI();
}

void loop() {
  if (analogRead(Light_Sensor_Pin) > 4000) {
    ws2812b.clear();
    ws2812b.show();
    send(2);
    analogWrite(14, 255);
    analogWrite(15, 255);
    analogWrite(16, 255);
    while (analogRead(Light_Sensor_Pin) > 4000 && digitalRead(Button_Pin) == HIGH) {Serial.print("Lichtsensor: "); Serial.println(analogRead(Light_Sensor_Pin)); delay(10000);}
    if (digitalRead(Button_Pin) == LOW) {analogWrite(16, 0); delay(1000);}
  }

  digitalWrite(Moist_Sensor_VCC, HIGH);
  delay(100);
  moist = analogRead(Moist_Sensor_Pin);
  delay(100);
  digitalWrite(Moist_Sensor_VCC, LOW);

  calculate_moist();
  updatePIXELS();
  pump_control();

  main_count--;
  if (main_count <= 0) {
    send(1000 + moist_percent * 10);
    main_count = 15;
  }
  delay(20000);
}


void pump_control() {
  if (moist_percent < 48 && pump_count <= -1) {pump_count = 0;}

  if (moist_percent < 50 && pump_count >= 0 && main_count % 5 == 0) {pump_count++; digitalWrite(Pump_Pin, LOW); delay(1000); digitalWrite(Pump_Pin, HIGH);}

  if (moist_percent >= 50) {pump_count = -1;}

  if (pump_count >= 10) {
    while (true) {
      analogWrite(14, 0);
      analogWrite(15, 255);
      analogWrite(16, 255);
      for (int i = 0; i < NUM_PIXELS; i++) {ws2812b.setPixelColor(i, ws2812b.Color(255, 0, 0));}
      ws2812b.show();

      delay(1000);

      analogWrite(14, 255);
      analogWrite(15, 255);
      analogWrite(16, 255);
      for (int i = 0; i < NUM_PIXELS; i++) {ws2812b.setPixelColor(i, ws2812b.Color(0, 0, 0));}
      ws2812b.show();

      delay(1000);
    }
  }

  Serial.print("Pump Count: ");
  Serial.println(pump_count);
  digitalWrite(Pump_Pin, HIGH);
}


void calculate_moist() {
  if ((moist_max - moist_min) == 0) {
    moist_percent = 0;
  } else {
    moist_percent = 100 * (moist - moist_min) / (moist_max - moist_min);
  }

  if (moist_percent > 100) {moist_percent = 100;}
  if (moist_percent < 0) {moist_percent = 0;}

  Serial.print("Moist: ");
  Serial.print(moist);
  Serial.print(" | ");
  Serial.print(moist_percent);
  Serial.println("%");
}


void updatePIXELS() {
  ws2812b.clear();

  for (int i = 0; i < red_part / 2; i++) {
    ws2812b.setPixelColor(i + Rotate, ws2812b.Color(255, 0, 0));
  }
  for (int i = 0; i < yellow_part / 2; i++) {
    int t = i + Rotate + (red_part / 2);
    while (t >= NUM_PIXELS) { t -= NUM_PIXELS; }
    ws2812b.setPixelColor(t, ws2812b.Color(255, 255, 0));
  }
  for (int i = 0; i < green_part; i++) {
    int t = i + Rotate + (red_part / 2) + (yellow_part / 2);
    while (t >= NUM_PIXELS) { t -= NUM_PIXELS; }
    ws2812b.setPixelColor(t, ws2812b.Color(0, 255, 0));
  }
  for (int i = 0; i < yellow_part / 2; i++) {
    int t = i + Rotate + (red_part / 2) + (yellow_part / 2) + green_part;
    while (t >= NUM_PIXELS) { t -= NUM_PIXELS; }
    ws2812b.setPixelColor(t, ws2812b.Color(255, 255, 0));
  }
  for (int i = 0; i < red_part / 2; i++) {
    int t = i + Rotate + (red_part / 2) + (yellow_part / 2) + green_part + (yellow_part / 2);
    while (t >= NUM_PIXELS) { t -= NUM_PIXELS; }
    ws2812b.setPixelColor(t, ws2812b.Color(255, 0, 0));
  }

  moist_pixel = round(float (used_PIXELS-1) * moist_percent / 100) + Rotate;
  while (moist_pixel >= NUM_PIXELS) {moist_pixel -= NUM_PIXELS;}
  if (((ws2812b.getPixelColor(moist_pixel) >> 16) & 0xFF) > 200) {
    moist_pixel_r = 255;
  } else {
    moist_pixel_r = 0;
  }
  if (((ws2812b.getPixelColor(moist_pixel) >> 8) & 0xFF) > 200) {
    moist_pixel_g = 255;
  } else {
    moist_pixel_g = 0;
  }
  ws2812b.setPixelColor(moist_pixel, ws2812b.Color(moist_pixel_r, moist_pixel_g, 255));

  ws2812b.show();
}


void send(int num) {
  int maxloops = 0;
  send_count = 0;
  send:
  Serial.println(num);
  // Use WiFiClient class to create TCP connections
  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println("Waiting 5 seconds before retrying...");
    WIFI();
    analogWrite(14, 0);
    analogWrite(15, 255);
    analogWrite(16, 255);
    delay(5000);
    return;
    send_count++;
    if (send_count <= 2) {goto send;}
    else {goto send_exit;}
  }

  client.print("GET /");
  client.print(num);
  client.print(" HTTP/1.1");
  client.print("\n\n");

  //wait for the server's reply to become available
  while (!client.available() && maxloops < 1000) {
    maxloops++;
    delay(1); //delay 1 msec
  }
  while (client.available()) {
    if (client.available() > 0) {
      //read back one line from the server
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
    else {
      Serial.println("client.available() timed out ");
      analogWrite(14, 0);
      analogWrite(15, 255);
      analogWrite(16, 255);
      break;
    }
  }

  Serial.println("Closing connection.");
  client.stop();
  send_exit:
  delay(0);
}

void WIFI() {
  WIFI_count = 0;
  WIFI:
  analogWrite(14, 0);
  analogWrite(15, 255);
  analogWrite(16, 255);
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(500);
  WiFi.begin(ssid1, password1);
  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");
  while (WiFi.status() != WL_CONNECTED) {
    WIFI_count ++;
    if (WIFI_count % 20 == 0) {goto WIFI;}
    if (WIFI_count >= 60) {goto WIFI_exit;}
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
  WIFI_exit:
  delay(0);
}
