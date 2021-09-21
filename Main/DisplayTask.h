/*
Developer 1 name : Jesse McDonald
Course : ENGR 2704
Project : CLEVER
File name : DisplayTask.h
Target Machine : ESP32
Operating System : freeRTOS
Integrated Development Environment(IDE) : Visual Studio Code and Arduino
Language : C++
Operational Status : Under Development
*/
#include <Arduino.h>
#include <SPI.h>
#include <SSD_13XX.h>
#include "TouchInterrupts.h"

//Pinout for Waveshare 1.5inch RGB OLED Display
#define _cs 17   // goes to disp CS
#define _dc 16   // goes to disp DC
#define _mosi 23 // goes to disp MOSI
#define _sclk 18 // goes to disp SCK/CLK
#define _RST 5   // ESP RST to disp RESET
#define _miso    // Not connected

void taskDisplay(void *pvParameter);
void taskUpdateDisplay(void *pvParameter);
void updateDisplay();
void printToDisplay(String to_print);

// Struct for displaydata
struct displayData
{
    uint16_t color = WHITE;
    String value = "000";
    bool selected = false;
    String what = "";
};

//Struct for data shared between controlledBreath task and Display task
struct fingerData
{
    int hr = 0;
    int o2 = 0;
};

//Struct for data shared between controlledBreath task and Display task
struct pumpData
{
    int bpm = 5;
    bool controlledMode = false;
    int tidalVolume = 0;
    bool off = false;
    bool trigger = false;
};
