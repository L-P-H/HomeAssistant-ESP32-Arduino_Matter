/*
E-Ink Display:
 * GND to GND
 * VCC to 3,3V
 * SDI to digital pin 11
 * SCLK to digital pin 13
 * CS to digital pin 10
 * DC to digital pin 9
 * RST to digital pin 8
 * BUSY to digital pin 7
*/

#define EPD_SS 10
#define EPD_DC 9
#define EPD_RST 8
#define EPD_BUSY 7
#define MAX_DISPLAY_BUFFER_SIZE 800
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
GxEPD2_3C<GxEPD2_290_C90c, MAX_HEIGHT(GxEPD2_290_C90c)>
display(GxEPD2_290_C90c(EPD_SS, EPD_DC, EPD_RST, EPD_BUSY));