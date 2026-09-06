#include <Matter.h>
#include <MatterFlow.h>
#include <MatterHumidity.h>
#include <MatterAirQuality.h>
#include <MatterTemperature.h>
#include <DHT_Async.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
#define DHT_SENSOR_PIN 3
#ifndef BTN_BUILTIN
  #define BTN_BUILTIN PA0
#endif

MatterHumidity matter_Luftfeuchtigkeit;
MatterTemperature matter_Temperatur;
MatterAirQuality matter_Luftqualität;
MatterFlow matter_Luftqualität_debug;
DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

static float fLuftfeuchtigkeit_im_Moment = 0.0f;
static float fTemperatur_im_Moment = 0.0f;
static bool bGas_erkannt = 0;
static int iGas_Stärke = 0;

void decommission_handler();

void setup() {
  delay(1000);
  Serial.begin(9600);
  Matter.begin();

  matter_Luftfeuchtigkeit.begin();
  matter_Temperatur.begin();
  matter_Luftqualität.begin();
  matter_Luftqualität_debug.begin();

  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT_PULLDOWN);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
  Serial.printf("Device Uniqe ID: %s\n", getDeviceUniqueIdStr().c_str());
  Serial.println("Matter humidity sensor...");
  Serial.print("Matter temperature sensor...");
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
  while (!matter_Luftfeuchtigkeit.is_online() || !matter_Temperatur.is_online() || !matter_Luftqualität.is_online() || !matter_Luftqualität_debug.is_online()) {
    decommission_handler();
    delay(200);
  }
  Serial.println("Matter devices are now online");
}

void loop() {
  if (dht_sensor.measure(&fTemperatur_im_Moment, &fLuftfeuchtigkeit_im_Moment)) {
    Serial.println("Measurement");
  }

  // Wait 10 seconds
  // Publish the humidity value - you can also use 'matter_Luftfeuchtigkeit.set_measured_value(fLuftfeuchtigkeit_im_Moment)'
  //matter_Luftfeuchtigkeit = fLuftfeuchtigkeit_im_Moment;
  if (millis() % 20000 == 0) {
    matter_Luftfeuchtigkeit.set_measured_value(fLuftfeuchtigkeit_im_Moment);
    Serial.printf("Current humidity: %.01f%%\n", fLuftfeuchtigkeit_im_Moment);
  }
  if (millis() % 20000 == 5000) {
    matter_Temperatur.set_measured_value_celsius(fTemperatur_im_Moment);
    Serial.printf("Current temperature: %.01f C\n", fTemperatur_im_Moment);
  }
  if (millis() % 20000 == 10000) {
    bGas_erkannt = !digitalRead(A3);
    matter_Temperatur.set_measured_value_celsius(fTemperatur_im_Moment);
    Serial.printf("Current gas detection: %d\n", bGas_erkannt);
  }
  if (millis() % 20000 == 15000) {
    iGas_Stärke = analogRead(A4);
    if (iGas_Stärke <= 0 || iGas_Stärke > 4095) {
      matter_Luftqualität.set_air_quality(MatterAirQuality::AirQuality_t::UNKNOWN);
      Serial.println("Current air quality: out of range");
    } else if (iGas_Stärke < 683) {
      matter_Luftqualität.set_air_quality(MatterAirQuality::AirQuality_t::GOOD);
      Serial.println("Current air quality: good");
    } else if (iGas_Stärke < 1365) {
      matter_Luftqualität.set_air_quality(MatterAirQuality::AirQuality_t::FAIR);
      Serial.println("Current air quality: fair");
    } else if (iGas_Stärke < 2048) {
      matter_Luftqualität.set_air_quality(MatterAirQuality::AirQuality_t::MODERATE);
      Serial.println("Current air quality: moderate");
    } else if (iGas_Stärke < 2730) {
      matter_Luftqualität.set_air_quality(MatterAirQuality::AirQuality_t::POOR);
      Serial.println("Current air quality: poor");
    } else if (iGas_Stärke < 3413) {
      matter_Luftqualität.set_air_quality(MatterAirQuality::AirQuality_t::VERY_POOR);
      Serial.println("Current air quality: very poor");
    } else if (iGas_Stärke <= 4095) {
      matter_Luftqualität.set_air_quality(MatterAirQuality::AirQuality_t::EXTREMELY_POOR);
      Serial.println("Current air quality: extremely poor");
    } else {
      matter_Luftqualität.set_air_quality(MatterAirQuality::AirQuality_t::UNKNOWN);
      Serial.println("Current air quality: unknown");
    }
    matter_Luftqualität_debug.set_measured_value_cubic_meters_per_hour(iGas_Stärke);
    Serial.printf("Current gas rate: %d\n", iGas_Stärke);
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