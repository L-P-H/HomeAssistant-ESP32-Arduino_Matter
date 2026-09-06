/*
   Matter fan example

   The example shows how to create a fan device with the Arduino Matter API.

   The example lets users control the fan through Matter and displays the values set by the user.
   The device has to be commissioned to a Matter hub first.

   Compatible boards:
   - Arduino Nano Matter
   - SparkFun Thing Plus MGM240P
   - xG24 Explorer Kit
   - xG24 Dev Kit
   - Seeed Studio XIAO MG24 (Sense)

   Author: Tamas Jozsi (Silicon Labs)
 */

uint8_t iLetzte_Raumtemperatur = 0;
uint8_t iLetzte_Raumluftfeuchtigkeit = 0;
uint8_t iLetzte_Aussentemperatur = 0;
uint8_t iLetzte_Aussenluftfeuchtigkeit = 0;
uint8_t iLetzte_Hour = 0;
uint8_t iLetzte_Minute = 0;
uint8_t iAktuelle_Raumtemperatur = 0;
uint8_t iAktuelle_Raumluftfeuchtigkeit = 0;
uint8_t iAktuelle_Aussentemperatur = 0;
uint8_t iAktuelle_Aussenluftfeuchtigkeit = 0;
uint8_t iAktuelle_Hour = 0;
uint8_t iAktuelle_Minute = 0;
uint8_t iE_Ink_Raumtemperatur = 0;
uint8_t iE_Ink_Raumluftfeuchtigkeit = 0;
uint8_t iE_Ink_Aussentemperatur = 0;
uint8_t iE_Ink_Aussenluftfeuchtigkeit = 0;
unsigned long iAktuelle_Sekunde_in_miilis = 0;
unsigned long iWarte_millis = 0;

#define BTN_BUILTIN PA0

#include <Matter.h>
#include <MatterFan.h>
#include <MatterLightbulb.h>
#include <ezWS2812.h>
#include <Wire.h>
#include "E_Ink_Display.h"
#include "Pixel_Ring.h"

MatterFan Raumtemperatur;
MatterFan Raumluftfeuchtigkeit;
MatterFan Aussentemperatur;
MatterFan Aussenluftfeuchtigkeit;
MatterFan Hour;
MatterFan Minute;
MatterLightbulb LED;

void setup() {
  delay(1000);
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A7, OUTPUT);

  digitalWrite(LED_BUILTIN, LED_BUILTIN_INACTIVE);

  Serial.begin(9600);
  Matter.begin();
  Raumtemperatur.begin();
  Raumluftfeuchtigkeit.begin();
  Aussentemperatur.begin();
  Aussenluftfeuchtigkeit.begin();
  Hour.begin();
  Minute.begin();
  LED.begin();

  Serial.println("E-Ink Display");
  E_Ink_setup();
  //Pixel_Ring_setup();

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
  while (!Raumtemperatur.is_online() || !Raumluftfeuchtigkeit.is_online() || !Aussentemperatur.is_online() || !Aussenluftfeuchtigkeit.is_online() || !Hour.is_online() || !Minute.is_online() || !LED.is_online()) {
    decommission_handler();
    delay(200);
  }
  Serial.println("Matter device is now online");
}

void loop() {
  //Pixel_Ring_loop();
  if (millis() > (iWarte_millis + 10000) && (iAktuelle_Raumtemperatur != iE_Ink_Raumtemperatur || iAktuelle_Raumluftfeuchtigkeit != iE_Ink_Raumluftfeuchtigkeit || iAktuelle_Aussentemperatur != iE_Ink_Aussentemperatur || iAktuelle_Aussenluftfeuchtigkeit != iE_Ink_Aussenluftfeuchtigkeit)) {E_Ink_loop();}
  if (Raumtemperatur.get_onoff() == false) {Raumtemperatur.set_onoff(true);}
  if (Raumluftfeuchtigkeit.get_onoff() == false) {Raumluftfeuchtigkeit.set_onoff(true);}
  if (Aussentemperatur.get_onoff() == false) {Aussentemperatur.set_onoff(true);}
  if (Aussenluftfeuchtigkeit.get_onoff() == false) {Aussenluftfeuchtigkeit.set_onoff(true);}
  if (Hour.get_onoff() == false) {Hour.set_onoff(true);}
  if (Minute.get_onoff() == false) {Minute.set_onoff(true);}

  iAktuelle_Raumtemperatur = Raumtemperatur.get_percent();
  iAktuelle_Raumluftfeuchtigkeit = Raumluftfeuchtigkeit.get_percent();
  iAktuelle_Aussentemperatur = Aussentemperatur.get_percent();
  iAktuelle_Aussenluftfeuchtigkeit = Aussenluftfeuchtigkeit.get_percent();
  iAktuelle_Hour = Hour.get_percent();
  iAktuelle_Minute = Minute.get_percent();

  if (iAktuelle_Raumtemperatur != iLetzte_Raumtemperatur) {
    iLetzte_Raumtemperatur = iAktuelle_Raumtemperatur;
    Serial.print("Raumtemperatur: ");
    Serial.print(iAktuelle_Raumtemperatur);
    Serial.println(" C");
    iWarte_millis = millis();
  }
  if (iAktuelle_Raumluftfeuchtigkeit != iLetzte_Raumluftfeuchtigkeit) {
    iLetzte_Raumluftfeuchtigkeit = iAktuelle_Raumluftfeuchtigkeit;
    Serial.print("Raumluftfeuchtigkeit: ");
    Serial.print(iAktuelle_Raumluftfeuchtigkeit);
    Serial.println(" %");
    iWarte_millis = millis();
  }
  if (iAktuelle_Aussentemperatur != iLetzte_Aussentemperatur) {
    iLetzte_Aussentemperatur = iAktuelle_Aussentemperatur;
    Serial.print("Aussentemperatur: ");
    Serial.print(iAktuelle_Aussentemperatur);
    Serial.println(" C");
    iWarte_millis = millis();
  }
  if (iAktuelle_Aussenluftfeuchtigkeit != iLetzte_Aussenluftfeuchtigkeit) {
    iLetzte_Aussenluftfeuchtigkeit = iAktuelle_Aussenluftfeuchtigkeit;
    Serial.print("Aussenluftfeuchtigkeit: ");
    Serial.print(iAktuelle_Aussenluftfeuchtigkeit);
    Serial.println(" %");
    iWarte_millis = millis();
  }
  if (iAktuelle_Hour != iLetzte_Hour) {
    iLetzte_Hour = iAktuelle_Hour;
    Serial.print("Hour: ");
    Serial.print(iAktuelle_Hour);
    Serial.println(" h");
  }
  if (iAktuelle_Minute != iLetzte_Minute) {
    iLetzte_Minute = iAktuelle_Minute;
    iAktuelle_Sekunde_in_miilis = millis();
    Serial.print("Minute: ");
    Serial.print(iAktuelle_Minute);
    Serial.println(" min");
  }

  
  static bool bLED_Letzter_Status = false;
  bool bLED_Aktueller_Status = LED.get_onoff();

  if (bLED_Aktueller_Status && !bLED_Letzter_Status) {
    bLED_Letzter_Status = bLED_Aktueller_Status;
    digitalWrite(A7, HIGH);
    Serial.println("LED ON");
  }

  if (!bLED_Aktueller_Status && bLED_Letzter_Status) {
    bLED_Letzter_Status = bLED_Aktueller_Status;
    digitalWrite(A7, LOW);
    Serial.println("LED OFF");
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
    // This function will not return
    // The device will restart once decommissioning has finished
    Matter.decommission();
  }
}