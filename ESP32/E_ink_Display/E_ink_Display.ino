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


void setup()
{
Serial.begin(9600);
display.init(9600, true, 2, false);
display.setTextSize(2);
SendToDisplay(10,1, 15,1, 20,1);
}


void SendToDisplay(int Temp, int Temp_ , int Hum, int Hum_, int Moist, int Moist_)
{
// center the bounding box by transposition of the origin:
//uint16_t x = ((display.width() - tbw) / 2) - tbx;
//uint16_t y = ((display.height() - tbh) / 2) - tby;
display.setRotation(3);
display.setFont(&FreeMonoBold9pt7b);
int16_t tbx, tby; uint16_t tbw, tbh;

display.getTextBounds("Placeholder", 0, 0, &tbx, &tby, &tbw, &tbh);
uint16_t ax = (0);
uint16_t ay = (tbh);
display.firstPage();

do
{
display.fillScreen(GxEPD_WHITE);
display.setCursor(ax, ay);

if (Temp_ == 0) {display.setTextColor(GxEPD_WHITE);}
else if (Temp_ == 1) {display.setTextColor(GxEPD_BLACK);}
else {display.setTextColor(GxEPD_RED);}
display.print("Temp:  ");
display.print(Temp);
display.println("C");

if (Hum_ == 0) {display.setTextColor(GxEPD_WHITE);}
else if (Hum_ == 1) {display.setTextColor(GxEPD_BLACK);}
else {display.setTextColor(GxEPD_RED);}
display.print("Hum:   ");
display.print(Hum);
display.println("%");

if (Moist_ == 0) {display.setTextColor(GxEPD_WHITE);}
else if (Moist_ == 1) {display.setTextColor(GxEPD_BLACK);}
else {display.setTextColor(GxEPD_RED);}
display.print("Moist: ");
display.print(Moist);
display.println("%");
}
while (display.nextPage());
display.hibernate();
}


void loop() {};