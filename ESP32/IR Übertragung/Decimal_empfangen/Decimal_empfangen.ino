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

#define IR_RECEIVE_PIN 2
#include <IRremote.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

float Data = 0;
float Temp = 0;
float Hum = 0;
int brightness_led = 0;   // 0 is off
int fadeAmount_led = 1;


void setup() {
  // put your setup code here, to run once:
  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK); // Start the receiver
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.decodedRawData, DEC); // Print "old" raw data
    //IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
    //IrReceiver.printIRSendUsage(&Serial);   // Print the statement required to send this data
    Data = IrReceiver.decodedIRData.decodedRawData, DEC;

    if (Data == 1) {lcd.clear(); goto end;}
    if (Data >= 10000 && Data <= 11000) {Data -= 10000; Temp = Data / 10; analogWrite(15, 250); goto print;}
    if (Data >= 15000 && Data <= 16000) {Data -= 15000; Hum = Data / 10; analogWrite(16, 250); goto print;}
    analogWrite(14, 0); delay(5000);

    print:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T = " );
    lcd.print(Temp, 1);
    lcd.print(char(0xDF));   //=char(223)=°
    lcd.print("C");
    Serial.print("  Temp: ");
    Serial.print(Temp, 1);
    Serial.println("°C");

    lcd.setCursor(0, 1);
    lcd.print("H = " );
    lcd.print(Hum, 1);
    lcd.print("%" );
    Serial.print("  Hum: ");
    Serial.print(Hum, 1);
    Serial.println("%");

    analogWrite(14, 255);
    analogWrite(15, 255);
    analogWrite(16, 255);

    end:
    IrReceiver.resume(); // Receive the next value
  }
}
