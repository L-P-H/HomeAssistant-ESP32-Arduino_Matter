#define IR_SEND_PIN 5
#include <IRremote.h>
#include <DHT_Async.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
static const int DHT_SENSOR_PIN = 3;
DHT_Async dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
//#define ONE_WIRE_BUS 6 //GPIO pin 6 = D3
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);

int sensorPin = A5; // select the input pin for the potentiometer 

int Count = 0;
int Bit = 0;
int Digit = 1;
int Temp = 0;
int Temp_ = 0;
int Hum = 0;
int Hum_ = 0;
int brightness_led = 0;   // 0 is off
int fadeAmount_led = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(24, INPUT_PULLUP);
  Serial.begin(9600);
  IrSender.begin(); // Start the sender
  //sensors.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  float temperature;
  float humidity;
  while (Temp == Temp_ && Hum == Hum_) {
    while (analogRead(sensorPin) > 4000) {IrSender.sendNECRaw(1); delay(10000);}

    if(dht_sensor.measure(&temperature, &humidity)){
      Temp_ = temperature * 10;
      Hum_ = humidity * 10;
      Serial.println("");
    }
    if (digitalRead(24) == LOW) {goto print;}
    analogWrite(14, brightness_led);
    analogWrite(15, brightness_led);
    brightness_led += fadeAmount_led;
    if (brightness_led <= 0) {fadeAmount_led = -fadeAmount_led;}
    if (brightness_led >= 255) {fadeAmount_led = -fadeAmount_led;}
    delay(10);
  }
  Temp = Temp_;
  Hum = Hum_;

  print:
  analogWrite(14, 255);
  analogWrite(15, 0);
  analogWrite(16, 255);
  if (digitalRead(24) == LOW) {
    analogWrite(15, 255);
    analogWrite(16, 0);
  }
  IrSender.sendNECRaw(Temp + 10000);
  delay(1000);
  IrSender.sendNECRaw(Hum + 15000);

  Serial.print("  Temp: ");
  Serial.print(Temp);
  Serial.print("  Hum: ");
  Serial.println(Hum);

  delay(1000);
  analogWrite(15, 255);
  analogWrite(16, 255);
}
