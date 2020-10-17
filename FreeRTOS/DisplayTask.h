#include <Arduino.h>

//Pinout for Waveshare 1.5inch RGB OLED Display
#define _cs 17   // goes to disp CS
#define _dc 16   // goes to disp DC
#define _mosi 23 // goes to disp MOSI
#define _sclk 18 // goes to disp SCK/CLK
#define _RST 5   // ESP RST to disp RESET
#define _miso    // Not connected

void taskDisplay(void *pvParameter);
