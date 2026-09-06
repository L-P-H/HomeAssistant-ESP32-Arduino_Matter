#include <Matter.h>
#include <MatterFlow.h>
#include <MatterOccupancy.h>
#include <MatterHumidity.h>
#include <MatterTemperature.h>
#include <DHT_Async.h>

const int iSensorAPin = A2;
const int iSensorDPin = A3;
#define DHT_SENSOR_TYPE DHT_TYPE_11
#define DHT_SENSOR_PIN 3
#ifndef BTN_BUILTIN
  #define BTN_BUILTIN PA0
#endif

MatterFlow matter_rain_analog;
MatterOccupancy matter_rain_digital;
MatterHumidity matter_humidity_sensor;
MatterTemperature matter_temp_sensor;
DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

static int iSensorValue = 0;
static bool bSensorON = 0;
static float fLuftfeuchtigkeit_im_Moment = 0.0f;
static float fTemperatur_im_Moment = 0.0f;

void decommission_handler();

void setup() {
  delay(1000);
  Serial.begin(9600);
  Matter.begin();

  matter_rain_analog.begin();
  matter_rain_digital.begin();
  matter_humidity_sensor.begin();
  matter_temp_sensor.begin();

  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  pinMode(iSensorAPin, INPUT);
  pinMode(iSensorDPin, INPUT);

  Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
  Serial.printf("Device Uniqe ID: %s\n", getDeviceUniqueIdStr().c_str());
  Serial.println("Matter outdoor sensor...");
  Serial.println("temperature/humidity sensor...");
  while (true) {
    if (dht_sensor.measure(&fTemperatur_im_Moment, &fLuftfeuchtigkeit_im_Moment)) {
      Serial.println("OK");
      break;
    }
  }

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
  }
  while (!Matter.isDeviceCommissioned()) {
    delay(200);
  }
  
  Serial.println("Waiting for Thread network...");
  while (!Matter.isDeviceThreadConnected()) {
    decommission_handler();
    delay(200);
  }
  Serial.println("Connected to Thread network");

  Serial.println("Waiting for Matter device discovery...");
  while (!matter_rain_analog.is_online() || !matter_rain_digital.is_online() || !matter_humidity_sensor.is_online() || !matter_temp_sensor.is_online()) {
    decommission_handler();
    delay(200);
  }
  Serial.println("Matter devices are now online");
}

void loop() {
  if (dht_sensor.measure(&fTemperatur_im_Moment, &fLuftfeuchtigkeit_im_Moment)) {
    Serial.println("Measurement");
  }

  if (millis() % 20000 == 0) {
    matter_humidity_sensor.set_measured_value(fLuftfeuchtigkeit_im_Moment);
    Serial.printf("Current humidity: %.01f%%\n", fLuftfeuchtigkeit_im_Moment);
  }
  if (millis() % 20000 == 5000) {
    matter_temp_sensor.set_measured_value_celsius(fTemperatur_im_Moment);
    Serial.printf("Current temperature: %.01f C\n", fTemperatur_im_Moment);
  }

  if (millis() % 20000 == 10000) {
    iSensorValue = 4095-analogRead(iSensorAPin);
    matter_rain_analog.set_measured_value_cubic_meters_per_hour(iSensorValue);
    Serial.print("analog rain: ");
    Serial.print(iSensorValue);
  }

  if (millis() % 20000 == 15000) {
    bSensorON = !digitalRead(iSensorDPin);
    matter_rain_digital.set_occupancy(bSensorON);
    Serial.print("     digital rain: ");
    Serial.println(bSensorON);
  }

  // Handle the decommissioning process if requested
  decommission_handler();
}

void decommission_handler(){
  // If the button is not pressed or the device is not commissioned - return
  if (digitalRead(BTN_BUILTIN) != LOW || !Matter.isDeviceCommissioned()) {
    return;
  }

  // Store the time when the button was first pressed
  uint32_t start_time = millis();
  // While the button is being pressed
  while (digitalRead(BTN_BUILTIN) == LOW) {
    // Calculate the elapsed time
    uint32_t elapsed_time = millis() - start_time;
    // If the button has been pressed for less than 10 seconds, continue
    if (elapsed_time < 10000u) {
      yield();
      continue;
    }

    // Blink the LED to indicate the start of the decommissioning process
    for (uint8_t i = 0u; i < 10u; i++) {
      digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
      delay(100);
    }

    Serial.println("Starting decommissioning process, device will reboot...");
    Serial.println();
    digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
    // This function will not return
    // The device will restart once decommissioning has finished
    Matter.decommission();
  }
}
