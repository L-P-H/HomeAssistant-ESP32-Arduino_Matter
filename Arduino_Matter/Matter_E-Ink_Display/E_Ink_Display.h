#include "api/Compat.h"
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#define EPD_SS 10
#define EPD_DC 9
#define EPD_RST 8
#define EPD_BUSY 7
#define MAX_DISPLAY_BUFFER_SIZE 800
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
GxEPD2_3C<GxEPD2_290_C90c, MAX_HEIGHT(GxEPD2_290_C90c)>
display(GxEPD2_290_C90c(EPD_SS, EPD_DC, EPD_RST, EPD_BUSY));



void E_Ink_setup() {
  display.init(9600, true, 2, false);
}

void E_Ink_loop() {
  Serial.println("E-Ink Display start");
  iE_Ink_Raumtemperatur = iAktuelle_Raumtemperatur;
  iE_Ink_Raumluftfeuchtigkeit = iAktuelle_Raumluftfeuchtigkeit;
  iE_Ink_Aussentemperatur = iAktuelle_Aussentemperatur;
  iE_Ink_Aussenluftfeuchtigkeit = iAktuelle_Aussenluftfeuchtigkeit;
  display.setRotation(3);
  display.setTextSize(2);
  display.setFont(&FreeMonoBold9pt7b);
  int16_t tbx, tby; uint16_t tbw, tbh;

  display.getTextBounds("Placeholder", 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t ax = (0);
  uint16_t ay = (tbh);
  display.firstPage();

  do {
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(ax, ay);

    display.setTextColor(GxEPD_BLACK);

    display.print("Temp: ");
    display.print(iE_Ink_Raumtemperatur);
    display.print("C/");
    display.print(iE_Ink_Aussentemperatur);
    display.println("C");

    display.print("Hum:  ");
    display.print(iE_Ink_Raumluftfeuchtigkeit);
    display.print("%/");
    display.print(iE_Ink_Aussenluftfeuchtigkeit);
    display.println("%");

    display.setTextColor(GxEPD_BLACK);
    display.println("[Regen]");
  } while (display.nextPage());

  display.hibernate();
  Serial.println("E-Ink Display hibernate");
}