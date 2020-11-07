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
  touchAttachInterrupt(T9, gotTouchLeft, touchThreshold);
  touchAttachInterrupt(T6, gotTouchRight, touchThreshold);
  touchAttachInterrupt(T5, gotTouchDown, touchThreshold);
  touchAttachInterrupt(T8, gotTouchUp, touchThreshold);
  touchAttachInterrupt(T7, gotTouchCenter, touchThreshold);
  xTouchSemaphore = xSemaphoreCreateBinary();
}

// Handlers for touch interface interrupts
void gotTouchLeft()
{
  touch_pad_intr_disable();
  leftTouchDetected = true;
  xSemaphoreGive(xTouchSemaphore);
  

}

void gotTouchRight()
{
  touch_pad_intr_disable();
  rightTouchDetected = true;
  xSemaphoreGive(xTouchSemaphore);
 
  
}

void gotTouchUp()
{
  touch_pad_intr_disable();
  upTouchDetected = true;
  xSemaphoreGive(xTouchSemaphore);
}

void gotTouchDown()
{
  touch_pad_intr_disable();
  downTouchDetected = true;
  xSemaphoreGive(xTouchSemaphore);
}

void gotTouchCenter()
{
  touch_pad_intr_disable();
  centerTouchDetected = true;
  xSemaphoreGive(xTouchSemaphore);
}
