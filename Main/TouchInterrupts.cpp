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

// Variable for Threshold
int touchThreshold = 0;

// Semaphore for touch
extern SemaphoreHandle_t xTouchSemaphore = xSemaphoreCreateBinary();

//Attaches interrupts to the the touch pins
void interruptsInit()
{
  vTaskDelay(500/portTICK_PERIOD_MS);
  touchThreshold = touchRead(T5)-10;
  touchAttachInterrupt(T8, gotTouchLeft, touchThreshold);
  touchAttachInterrupt(T6, gotTouchRight, touchThreshold);
  touchAttachInterrupt(T5, gotTouchDown, touchThreshold);
  touchAttachInterrupt(T9, gotTouchUp, touchThreshold);
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
