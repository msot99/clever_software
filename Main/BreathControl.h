/*
Developer 1 name : Jesse McDonald
Course : ENGR 2704
Project : CLEVER
File name : BreathControl.h
Target Machine : ESP32
Operating System : freeRTOS
Integrated Development Environment(IDE) : Visual Studio Code and Arduino
Language : C++
Operational Status : Under Development
*/

#include "Arduino.h"
void taskBreath(void *pvParameters);
void IRAM_ATTR SensorTime2rInterrupt();
void taskAlert(void *pvParameters);
