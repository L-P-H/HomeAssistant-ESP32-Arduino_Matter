/*
Light dependend Resistor:
 *int sensorPin = A0

LCD Display:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 2K resistor:
 * ends to V0(pin 3) and ground
 * 220 resistor:
 * ends to A and 5V
 * K to ground
*/


#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int Round = 0;
int Count = 0;
int Bit = 0;
int Digit = 0;
int Digit_Value = 0;
int sensorPin = A0;
int Sensor = 0;
float Temp = 0.0;
int TempZ = 0;
int TempE = 0;
int TempK = 0;
float Hum = 0.0;
int HumZ = 0;
int HumE = 0;
int HumK = 0;
int brightness_led = 0;   // 0 is off
int fadeAmount_led = 1;
int Sensor_count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT);
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if (digitalRead(2) == 0){ //(analogRead(sensorPin) < 2000){
    analogWrite(LED_BUILTIN, 255);
    Serial.print("1");
    Sensor = 1;
  }
  else{
    analogWrite(LED_BUILTIN, 0);
    Serial.print("0");
    Sensor = 0;
  }
  */
  Sensor = 0;
  while (Sensor_count != 100) {
    Sensor_count += 1;
    if (digitalRead(2) == 0) {
      Sensor = 1;
    }
    delay(1);
  }
  Sensor_count =0;

  if (Sensor == 1) {
    Serial.print("1");
    analogWrite(LED_BUILTIN, 255);
  }
  else {
    Serial.print("0");
    analogWrite(LED_BUILTIN, 0);
  }

  Serial.print("  Round: ");
  Serial.print(Round);
  Serial.print("  Count: ");
  Serial.print(Count);
  Serial.print("  Bit: ");
  Serial.print(Bit);
  Serial.print("  Digit: ");
  Serial.print(Digit);
  Serial.print("  Digit_Value: ");
  Serial.println(Digit_Value);


//-------------------------------------------------------------------------------


  if (Round >= 9){
    if (Count >= 3){
      Bit = 1;
      Digit_Value += 1;
    }
    else {
      if (Digit == 1){
        Temp = Digit_Value * 10;
      }
      if (Digit == 2){
        Temp += Digit_Value;
      }
      if (Digit == 3){
        Temp += Digit_Value * 0.1;
      }
      if (Digit == 4){
        Hum = Digit_Value * 10;
      }
      if (Digit == 5){
        Hum += Digit_Value;
      }
      if (Digit == 6){
        Hum += Digit_Value * 0.1;
      }

      Bit = 0;
      Digit += 1;
      Digit_Value = 0;
    }
    Round = 0;
    Count = 0;
    Serial.println("");
  }

  if (Sensor == 1){
    Count += 1;
  }

  Round += 1;


//-------------------------------------------------------------------------------


//  delay(100);
  if (Digit > 6){
    Serial.print(Temp);
    Serial.print("   ");
    Serial.println(Hum);
    Serial.println("Wait for signal");

    // set the cursor to column 0, line 0
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 0);
    lcd.print("----------------");
    lcd.setCursor(0, 1);
    lcd.print("----------------");
    delay(50);
    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("T = " );
    lcd.print(Temp, 1 );
    lcd.print(char(0xDF));   //=char(223)=°
    lcd.print("C");

    lcd.setCursor(0, 1);

    lcd.print("H = " );
    lcd.print(Hum, 1 );
    lcd.print("%" );

    while (digitalRead(2) == HIGH){ //(analogRead(sensorPin) > 2000){
      Digit = 1;
      Round = 1;
      analogWrite(LED_BUILTIN, brightness_led);
      brightness_led += fadeAmount_led;
      if (brightness_led <= 0) {fadeAmount_led = -fadeAmount_led;}
      if (brightness_led >= 255) {fadeAmount_led = -fadeAmount_led;}
      delay(10);
    }
  }
}
