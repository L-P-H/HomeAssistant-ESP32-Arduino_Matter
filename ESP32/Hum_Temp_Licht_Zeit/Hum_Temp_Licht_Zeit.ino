#include <DHT_Async.h>
#include <WiFi.h>
#include <sntp.h>
#include "secrets.h"

#define DHT_SENSOR_TYPE DHT_TYPE_11
#define DHT_SENSOR_PIN 3
#define Light_Sensor_Pin A5
#define Button_Pin A7 //24


DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// const char* ssid0 = "";
// const char* password0 = "";
// const char* ssid1 = "";
// const char* password1 = "";
const uint16_t port = 80;
const char* host = "192.168.4.1";  // ip or dns

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Berlin including daylight adjustment rules (optional)

int WIFI_intern = 0;

int Second = 0;
int Minute = 0;
int Hour = 0;
int Day = 0;
int Month = 0;
int Year = 0;
int Offset = 6000;

int Count = 0;
int Temp = 0;
int Temp_ = 0;
int Hum = 0;
int Hum_ = 0;
int brightness_led = 0;  // 0 is off
int fadeAmount_led = 1;
float temperature;
float humidity;

void setup() {
  // put your setup code here, to run once:
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(24, INPUT_PULLUP);
  Serial.begin(9600);

  // set notification call-back function
  sntp_set_time_sync_notification_cb(timeavailable);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  WIFI(0);

  while (WIFI_intern != 1) {
    Serial.println("Wait for time Setup");
    delay(1000);
  }
  Serial.println("Setting intern WIFI!");

  WIFI(1);

  while (true) {
    if (dht_sensor.measure(&temperature, &humidity)) {
      Temp_ = temperature;
      Hum_ = humidity;
      Temp = Temp_;
      Hum = Hum_;
      Serial.print("OK");
      print();
      break;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Count = 0;
  while ((Temp == Temp_ && Hum == Hum_) && Count <= 30000) { //alle 5 min
    if (analogRead(Light_Sensor_Pin) > 4000) {
      send(1);
      while (analogRead(Light_Sensor_Pin) > 4000 && digitalRead(Button_Pin) == HIGH) {
        delay(10000);
        analogWrite(14, 255);
        analogWrite(15, 255);
        analogWrite(16, 255);
        Serial.print("Lichtsensor: ");
        Serial.println(analogRead(Light_Sensor_Pin));
        printLocalTime();
      }
    }

    if (dht_sensor.measure(&temperature, &humidity)) {
      Temp_ = temperature;
      Hum_ = humidity;
      Serial.println("Measurement");
      printLocalTime();
    }
    if (digitalRead(24) == LOW) {
      send(1000000000 + Year * 10000 + Month * 100 + Day);
      delay(1000);
      send(1100000000 + Hour * 3600000 + Minute * 60000 + Second * 1000 + Offset);
      delay(1000);
      print();
    }

    analogWrite(14, brightness_led);
    analogWrite(15, brightness_led);
    analogWrite(16, 255);
    brightness_led += fadeAmount_led;
    if (brightness_led <= 0) { fadeAmount_led = -fadeAmount_led; }
    if (brightness_led >= 255) { fadeAmount_led = -fadeAmount_led; }
    Count++;

    if (millis() % 900000 <= 1000) {
      send(1000000000 + Year * 10000 + Month * 100 + Day);
      delay(1000);
      send(1100000000 + Hour * 3600000 + Minute * 60000 + Second * 1000 + Offset);
    }  //=15min

    if (millis() % 14400000 <= 1000) { WIFI_intern = 0; }  //=4h
    if (WIFI_intern == 0 || WIFI_intern == 1) { WIFI(WIFI_intern); }

    delay(10);
  }
  Temp = Temp_;
  Hum = Hum_;
  print();
}

void print() {
  analogWrite(14, 255);
  analogWrite(15, 0);
  analogWrite(16, 255);
  if (digitalRead(24) == LOW) {
    analogWrite(15, 255);
    analogWrite(16, 0);
  }

  Serial.print("  Temp: ");
  Serial.print(Temp);
  Serial.print("  Hum: ");
  Serial.println(Hum);

  send(1000000 + 10000 * Temp + 10 * Hum);
  Serial.println(1000000 + 10000 * Temp + 10 * Hum);

  analogWrite(15, 255);
  analogWrite(16, 255);
}


void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  Serial.print(&timeinfo, "%Y-%m-%d %H:%M:%S");
  Serial.print("  ");
  Serial.println(millis() % 14400000);  //=4h

  Second = timeinfo.tm_sec;
  Minute = timeinfo.tm_min;
  Hour = timeinfo.tm_hour;
  Day = timeinfo.tm_mday;
  Month = 1 + timeinfo.tm_mon;
  Year = 1900 + timeinfo.tm_year;
}


// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval* t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
  delay(1000);
  WIFI_intern = 1;
}


void send(int num) {
  // Use WiFiClient class to create TCP connections
  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println("Waiting 5 seconds before retrying...");
    WIFI(1);
    analogWrite(14, 0);
    analogWrite(15, 255);
    analogWrite(16, 255);
    delay(5000);
    return;
  }

  client.print("GET /");
  client.print(num);
  client.print(" HTTP/1.1");
  client.print("\n\n");

  Serial.print("GET /");
  Serial.print(num);
  Serial.print(" HTTP/1.1");
  Serial.print("\n\n");

  int maxloops = 0;

  //wait for the server's reply to become available
  while (!client.available() && maxloops < 1000) {
    maxloops++;
    delay(1);  //delay 1 msec
  }
  while (client.available()) {
    if (client.available() > 0) {
      //read back one line from the server
      String line = client.readStringUntil('\n');
      Serial.println(line);
    } else {
      Serial.println("client.available() timed out ");
      analogWrite(14, 0);
      analogWrite(15, 255);
      analogWrite(16, 255);
      break;
    }
  }

  Serial.println("Closing connection.");
  client.stop();
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
      count++;
      if (count >= 20) { goto WIFI; }
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
  } else if (intern == 1) {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(500);
    WiFi.begin(ssid1, password1);
    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi... ");
    while (WiFi.status() != WL_CONNECTED) {
      count++;
      if (count >= 20) { goto WIFI; }
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

    send(1000000000 + Year * 10000 + Month * 100 + Day);
    send(1100000000 + Hour * 3600000 + Minute * 60000 + Second * 1000 + Offset);

    WIFI_intern = -1;
  }
}
