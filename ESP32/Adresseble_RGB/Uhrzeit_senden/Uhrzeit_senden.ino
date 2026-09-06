#include <WiFi.h>
#include <sntp.h>
#include "secrets.h"

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

  // set notification call-back function
  sntp_set_time_sync_notification_cb( timeavailable );
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  WIFI(0);
}


void loop() {
  delay(1000);

  printLocalTime();     // it will take some time to sync time :)
  if (millis()%900000 <= 1000) {sendLocalTime(1000000000 + Year * 10000 + Month * 100 + Day); sendLocalTime(1100000000 + Hour * 3600000 + Minute * 60000 + Second * 1000 + 30000);}  //=15min

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
  Month = 1 + timeinfo.tm_mon;
  Year = 1900 + timeinfo.tm_year;
}


// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
  WIFI_intern = 1;
}


void sendLocalTime(int num) {
  Serial.println(num);
  // Use WiFiClient class to create TCP connections
  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println("Waiting 5 seconds before retrying...");
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

  int maxloops = 0;

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

    sendLocalTime(1000000000 + Year * 10000 + Month * 100 + Day);
    sendLocalTime(1100000000 + Hour * 3600000 + Minute * 60000 + Second * 1000 + 30000);

    WIFI_intern = -1;
  }
}
