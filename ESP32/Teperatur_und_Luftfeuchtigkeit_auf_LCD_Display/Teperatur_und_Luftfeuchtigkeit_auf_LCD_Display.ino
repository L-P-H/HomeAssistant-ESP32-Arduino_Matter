/*
DHT Sensor:
 *DHT_SENSOR_PIN = 2

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


#include <DHT_Async.h>
#include <LiquidCrystal.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
//#define DHT_SENSOR_TYPE DHT_TYPE_21
//#define DHT_SENSOR_TYPE DHT_TYPE_22

static const int DHT_SENSOR_PIN = 3;
DHT_Async dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup( )
{
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop( )
{
  float temperature;
  float humidity;

  if(dht_sensor.measure(&temperature, &humidity)){
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( "°C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );
  }


  // set the cursor to column 0, line 0
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);

  lcd.print( "T = " );
  lcd.print( temperature, 1 );
  lcd.print(char(0xDF));   //=char(223)=°
  lcd.print( "C");

  lcd.setCursor(0, 1);

  lcd.print( "H = " );
  lcd.print( humidity, 1 );
  lcd.print( "%" );
}

