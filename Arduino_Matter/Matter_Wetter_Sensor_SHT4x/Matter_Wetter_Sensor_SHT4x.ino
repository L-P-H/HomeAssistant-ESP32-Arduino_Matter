#include <Matter.h>
#include <MatterFlow.h>
#include <MatterOccupancy.h>
#include <MatterHumidity.h>
#include <MatterTemperature.h>
#include <MatterOnOffPluginUnit.h>
#include "Wire.h"
#include "SHT4x.h"

const int iSensorAPin = A2;
const int iSensorDPin = A3;
const int iSensorVCCPin = D2;
const int iSensorWPin = D3;
#ifndef BTN_BUILTIN
  #define BTN_BUILTIN PA0
#endif
#define SHT_DEFAULT_ADDRESS   0x44

MatterFlow matter_rain_analog;
MatterOccupancy matter_rain_digital;
MatterHumidity matter_humidity_sensor;
MatterTemperature matter_temp_sensor;
MatterOnOffPluginUnit matter_heat_on;
MatterOccupancy matter_rain_wippe;
SHT4x sht;

static int iSensorValue = 0;
static bool bSensorON = 0;
static float fLuftfeuchtigkeit_im_Moment = 0.0f;
static float fTemperatur_im_Moment = 0.0f;
static int iErrorCount = 3;
int long iWippe_millis = 0;

void decommission_handler();

void setup() {
  delay(1000);
  Serial.begin(9600);
  Matter.begin();

  matter_rain_analog.begin();
  matter_rain_digital.begin();
  matter_humidity_sensor.begin();
  matter_temp_sensor.begin();
  matter_heat_on.begin();
  matter_rain_wippe.begin();

  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);
  pinMode(iSensorAPin, INPUT);
  pinMode(iSensorDPin, INPUT);
  pinMode(iSensorVCCPin, OUTPUT);
  pinMode(iSensorWPin, INPUT_PULLUP);

  Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
  Serial.printf("Device Uniqe ID: %s\n", getDeviceUniqueIdStr().c_str());
  Serial.println("Matter outdoor sensor...");
  Serial.println("temperature/humidity sensor...");
  Serial.print("SHT4x_LIB_VERSION: \t");
  Serial.println(SHT4x_LIB_VERSION);

  Wire.begin();
  Wire.setClock(100000);
  sht.begin();
  while (true) {
    if (sht.requestData()) {
      Serial.println("OK");
      break;
    } else if (millis() > 10000) {
      Serial.println("Failed to connect to SHT4x");
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
  while (!matter_rain_analog.is_online() || !matter_rain_digital.is_online() || !matter_humidity_sensor.is_online() || !matter_temp_sensor.is_online() || !matter_heat_on.is_online() || !matter_rain_wippe.is_online()) {
    decommission_handler();
    delay(200);
  }
  matter_heat_on.set_onoff(0);
  sht.setHeatProtection(true);
  Serial.println("Matter devices are now online");
}

void loop() {
  if (millis() % 25000 == 0) {
    if (matter_heat_on.get_onoff()) {
      sht.requestData(SHT4x_MEASUREMENT_LONG_HIGH_HEAT);
      matter_heat_on.set_onoff(0);
      Serial.println("Heat on");
    } else{
      sht.requestData();
    }
  }
  if (sht.readData()) {
    if (sht.getHumidity() < fLuftfeuchtigkeit_im_Moment + 10 && sht.getHumidity() > fLuftfeuchtigkeit_im_Moment - 10) {
      fLuftfeuchtigkeit_im_Moment = sht.getHumidity();
    } else {iErrorCount ++;}
    if (sht.getTemperature() < fTemperatur_im_Moment + 10 && sht.getTemperature() > fTemperatur_im_Moment - 10) {
      fTemperatur_im_Moment = sht.getTemperature();
    } else {iErrorCount ++;}
    if (iErrorCount >= 3) {
      fLuftfeuchtigkeit_im_Moment = sht.getHumidity();
      fTemperatur_im_Moment = sht.getTemperature();
    }
    Serial.println("Measurement");
  }

  if (millis() % 25000 == 5000) {
    matter_humidity_sensor.set_measured_value(fLuftfeuchtigkeit_im_Moment);
    Serial.printf("Current humidity: %.01f%%\n", fLuftfeuchtigkeit_im_Moment);
  }
  if (millis() % 25000 == 10000) {
    matter_temp_sensor.set_measured_value_celsius(fTemperatur_im_Moment);
    Serial.printf("Current temperature: %.01f °C\n", fTemperatur_im_Moment);
  }

  if (millis() % 25000 == 13000) {digitalWrite(iSensorVCCPin, true);}
  if (millis() % 25000 == 15000) {
    digitalWrite(iSensorVCCPin, true);
    iSensorValue = 4095-analogRead(iSensorAPin);
    matter_rain_analog.set_measured_value_cubic_meters_per_hour(iSensorValue);
    Serial.print("analog rain: ");
    Serial.print(iSensorValue);
    digitalWrite(iSensorVCCPin, false);
  }

  if (millis() % 25000 == 18000) {digitalWrite(iSensorVCCPin, true);}
  if (millis() % 25000 == 20000) {
    bSensorON = !digitalRead(iSensorDPin);
    matter_rain_digital.set_occupancy(bSensorON);
    Serial.print("     digital rain: ");
    Serial.println(bSensorON);
    digitalWrite(iSensorVCCPin, false);
  }

  if (!digitalRead(iSensorWPin)) {
    matter_rain_wippe.set_occupancy(true);
    Serial.println("     Wippe : true");
    iWippe_millis = millis() + 100;
  }

  if (iWippe_millis < millis() and iWippe_millis != 0) {
    matter_rain_wippe.set_occupancy(false);
    Serial.println("     Wippe : false");
    iWippe_millis = 0;
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
