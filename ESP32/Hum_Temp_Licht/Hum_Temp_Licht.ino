#include <DHT_Async.h>
#include <WiFi.h>
#include "secrets.h"
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 3;
DHT_Async dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

// const char* ssid     = ""; // Change this to your WiFi SSID
// const char* password = ""; // Change this to your WiFi password
const uint16_t port = 80;
const char * host = "192.168.4.1"; // ip or dns


int sensorPin = A5; // select the input pin for the potentiometer 
int Count = 0;
int Temp = 0;
int Temp_ = 0;
int Hum = 0;
int Hum_ = 0;
int brightness_led = 0;   // 0 is off
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
  
  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");
  analogWrite(14, 0);
  analogWrite(15, 0);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);

  while(true){
    if(dht_sensor.measure(&temperature, &humidity)){
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
  while ((Temp == Temp_ && Hum == Hum_) || Count <= 30000) {
    if (analogRead(sensorPin) > 4000) {
      WIFI(1);
      while (analogRead(sensorPin) > 4000) {delay(10000);}
    }

    if(dht_sensor.measure(&temperature, &humidity)){
      Temp_ = temperature;
      Hum_ = humidity;
      Serial.println("");
    }
    if (digitalRead(24) == LOW) {print();}

    analogWrite(14, brightness_led);
    analogWrite(15, brightness_led);
    analogWrite(16, 255);
    brightness_led += fadeAmount_led;
    if (brightness_led <= 0) {fadeAmount_led = -fadeAmount_led;}
    if (brightness_led >= 255) {fadeAmount_led = -fadeAmount_led;}
    Count ++;
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

  WIFI(1000000 + 10000*Temp + 10*Hum);
  Serial.println(1000000 + 10000*Temp + 10*Hum);
  
  analogWrite(15, 255);
  analogWrite(16, 255);
}

void WIFI(int num) {
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
