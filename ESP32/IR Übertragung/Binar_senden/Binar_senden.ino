#include <DHT_Async.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
static const int DHT_SENSOR_PIN = 3;
DHT_Async dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
//#define ONE_WIRE_BUS 6 //GPIO pin 6 = D3
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);

int Round = 0;
int Count = 0;
int Bit = 0;
int Digit = 1;
float Temp = 0;
int TempZ = 0;
int TempE = 0;
int TempK = 0;
float Hum = 0;
int HumZ = 0;
int HumE = 0;
int HumK = 0;
int brightness_led = 0;   // 0 is off
int fadeAmount_led = 1;
int Breake = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  //sensors.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Start:
  
  TempZ = Temp / 10;
  Serial.print(TempZ);
  TempE = Temp - (TempZ * 10);
  Serial.print(TempE);
  Serial.print(".");
  TempK = Temp * 10 - (TempZ * 100 + TempE * 10);
  Serial.print(TempK);
  Serial.print("  ");

  HumZ = Hum / 10;
  Serial.print(HumZ);
  HumE = Hum - (HumZ * 10);
  Serial.print(HumE);
  Serial.print(".");
  HumK = Hum * 10 - (HumZ * 100 + HumE * 10);
  Serial.print(HumK);
  
  if ((Digit == 6) && (Count == 0)){
    Digit = 1;
    Count = TempZ;

    Serial.println("Wait");
    Round = -600;
    Breake = 0;
    float temperature;
    float humidity;
    while (Breake == 0) {
      if(dht_sensor.measure(&temperature, &humidity)){
      Temp = temperature;
      Hum = humidity;
      Breake = 1;
      }
    }
    //sensors.requestTemperatures();
    //Temp = sensors.getTempCByIndex(0);
    while (Round < 0){
      analogWrite(LED_BUILTIN, brightness_led);
      brightness_led += fadeAmount_led;
      if (brightness_led <= 0) {fadeAmount_led = -fadeAmount_led;}
      if (brightness_led >= 255) {fadeAmount_led = -fadeAmount_led;}
      delay(10);
      Round += 1;
    }
    Round = 0;

    digitalWrite(LED_BUILTIN, HIGH);
    delay(10000);
    digitalWrite(2, HIGH);
    delay(15);
    digitalWrite(2, LOW);
    delay(85);
    digitalWrite(LED_BUILTIN, LOW);
  }
  if ((Digit == 1) && (Count == 0)){
    Digit = 2;
    Count = TempE;
    delay(900);
  }
  if ((Digit == 2) && (Count == 0)){
    Digit = 3;
    Count = TempK;
    delay(900);
  }

  if ((Digit == 3) && (Count == 0)){
    Digit = 4;
    Count = HumZ;
    delay(900);
  }
  if ((Digit == 4) && (Count == 0)){
    Digit = 5;
    Count = HumE;
    delay(900);
  }
  if ((Digit == 5) && (Count == 0)){
    Digit = 6;
    Count = HumK;
    delay(900);
  }


//-------------------------------------------------------------------------------


  if ((Round == 9) && (Count > 0)){
    Count -= 1;
  }

  if ((Round >= 3) && (Round <= 6) && (Count > 0)){
    digitalWrite(2, HIGH);
    //delay(10);
    //digitalWrite(2, LOW);
    //delay(50);
    analogWrite(LED_BUILTIN, 255);
    Bit = 1;
  }
  else{
    digitalWrite(2, LOW);
    analogWrite(LED_BUILTIN, 0);
    Bit = 0;
  }


//-------------------------------------------------------------------------------


  Serial.print("  Round: ");
  Serial.print(Round);
  Serial.print("  Count: ");
  Serial.print(Count);
  Serial.print("  Bit: ");
  Serial.print(Bit);
  Serial.print("  Digit: ");
  Serial.println(Digit);

  if (Round >= 9){
    Round = 0;
    Serial.println("");
  }

  Round += 1;
  delay(10);
  digitalWrite(2, LOW);
  delay(90);
}
