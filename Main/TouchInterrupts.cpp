/*
Developer 1 name : Jesse McDonald
Course : ENGR 2704
Project : CLEVER
File name : TouchInterrupts.cpp
Target Machine : ESP32
Operating System : freeRTOS
Integrated Development Environment(IDE) : Visual Studio Code and Arduino
Language : C++
Operational Status : Under Development
*/
#include "TouchInterrupts.h"

bool leftTouchDetected = false;
bool rightTouchDetected = false;
bool upTouchDetected = false;
bool downTouchDetected = false;
bool centerTouchDetected = false;

// Semaphore for touch
SemaphoreHandle_t xTouchSemaphore = xSemaphoreCreateBinary();

//Attaches interrupts to the the touch pins
void interruptsInit()
{
  touchAttachInterrupt(T8, gotTouchLeft, touchThreshold);
  touchAttachInterrupt(T7, gotTouchRight, touchThreshold);
  touchAttachInterrupt(T5, gotTouchDown, touchThreshold);
  touchAttachInterrupt(T9, gotTouchUp, touchThreshold);
  touchAttachInterrupt(T6, gotTouchCenter, touchThreshold);
  xTouchSemaphore = xSemaphoreCreateBinary();
}

// Handlers for touch interface interrupts
void gotTouchLeft()
{
  leftTouchDetected = true;
  xSemaphoreGive(xTouchSemaphore);
}

void gotTouchRight()
{
  rightTouchDetected = true;
  xSemaphoreGive(xTouchSemaphore);
}

void gotTouchUp()
{
  upTouchDetected = true;
  xSemaphoreGive(xTouchSemaphore);
}

void gotTouchDown()
{
  downTouchDetected = true;
  xSemaphoreGive(xTouchSemaphore);
}

void gotTouchCenter()
{
  centerTouchDetected = true;
  xSemaphoreGive(xTouchSemaphore);
}
