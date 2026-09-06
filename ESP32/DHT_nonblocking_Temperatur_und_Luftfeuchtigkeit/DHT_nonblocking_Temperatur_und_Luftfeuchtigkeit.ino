#include <DHT_Async.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
//#define DHT_SENSOR_TYPE DHT_TYPE_21
//#define DHT_SENSOR_TYPE DHT_TYPE_22

static const int DHT_SENSOR_PIN = 12;
DHT_Async dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

void setup( )
{
  Serial.begin(9600);
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
}

