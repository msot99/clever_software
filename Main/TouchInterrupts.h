/*
Developer 1 name : Jesse McDonald
Course : ENGR 2704
Project : CLEVER
File name : TouchInterrupts.h
Target Machine : ESP32
Operating System : freeRTOS
Integrated Development Environment(IDE) : Visual Studio Code and Arduino
Language : C++
Operational Status : Under Development
*/
#include "Arduino.h"

extern SemaphoreHandle_t xTouchSemaphore;

extern bool leftTouchDetected;
extern bool rightTouchDetected;
extern bool upTouchDetected;
extern bool downTouchDetected;
extern bool centerTouchDetected;

void interruptsInit();

void gotTouchLeft();
void gotTouchRight();
void gotTouchUp();
void gotTouchDown();
void gotTouchCenter();
