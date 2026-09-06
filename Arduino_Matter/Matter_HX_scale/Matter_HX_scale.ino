#include <HX711.h>
#include <EEPROM.h>
#include <Matter.h>
#include <MatterFan.h>
#include <MatterFlow.h>

HX711 scale;
MatterFan matter_Tare_Calibrate;
MatterFlow matter_Gewicht;

#define BTN_BUILTIN PA0
#define iDaten_Pin  6
#define iUhr_Pin 7

static float fWaage_Offset = 0.00f;
static float fWaage_Multiplikator = 0.00f;

void setup(){
  delay(1000);
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  Serial.begin(9600);
  Matter.begin();
  matter_Tare_Calibrate.begin();
  matter_Gewicht.begin();
  scale.begin(iDaten_Pin, iUhr_Pin);

  Serial.println();
  Serial.println(__FILE__);
  Serial.print("HX711_LIB_VERSION: ");
  Serial.println(HX711_LIB_VERSION);
  Serial.print("EEPROM Länge: ");
  Serial.println(EEPROM.length());
  Serial.println();

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
  while (!matter_Tare_Calibrate.is_online() || !matter_Gewicht.is_online()) {
    decommission_handler();
    delay(200);
  }
  Serial.println("Matter device is now online");

  matter_Tare_Calibrate.set_percent(100);
  matter_Tare_Calibrate.set_onoff(true);
  EEPROM.get(0, fWaage_Offset);
  EEPROM.get(100, fWaage_Multiplikator);
  Serial.print("Tare: ");
  Serial.println(fWaage_Offset);
  Serial.print("Calibrate: ");
  Serial.println(fWaage_Multiplikator);
  if (fWaage_Offset > 0 || fWaage_Offset < 0) {
    scale.set_offset(fWaage_Offset);
    Serial.println("Tare: OK");
  }
  if (fWaage_Multiplikator > 0 || fWaage_Multiplikator < 0) {
    scale.set_scale(fWaage_Multiplikator);
    Serial.println("Calibrate: OK");
  }
}


void loop(){
  if (millis() % 10000 == 0){
    Serial.print("UNITS: ");
    Serial.println(scale.get_units(10));
    matter_Gewicht.set_measured_value_cubic_meters_per_hour(scale.get_units(10)/1000);
  }

  if (matter_Tare_Calibrate.get_onoff() == false){
    scale.tare();
    matter_Tare_Calibrate.set_onoff(true);
    Serial.print("Tare: ");
    Serial.println(scale.get_offset());
    EEPROM.put(0, float(scale.get_offset()));
    EEPROM.get(0, fWaage_Offset);
    Serial.println(fWaage_Offset);
  }
  if (matter_Tare_Calibrate.get_percent() != 100){
    scale.calibrate_scale(matter_Tare_Calibrate.get_percent()*1000, 5);
    matter_Tare_Calibrate.set_percent(100);
    Serial.print("Calibrate: ");
    Serial.println(scale.get_scale());
    EEPROM.put(100, float(scale.get_scale()));
    EEPROM.get(100, fWaage_Multiplikator);
    Serial.println(fWaage_Multiplikator);
  }

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
    for (int i = 0 ; i < EEPROM.length() ; i++) {EEPROM.write(i, 0);}
    // This function will not return
    // The device will restart once decommissioning has finished
    Matter.decommission();
  }
}