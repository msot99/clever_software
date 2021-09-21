/*
  Developer 1 name : Jesse McDonald
  Course : ENGR 2704
  Project : CLEVER
  File name : SensorIO.cpp
  Target Machine : ESP32
  Operating System : freeRTOS
  Integrated Development Environment(IDE) : Visual Studio Code and Arduino
  Language : C++
  Operational Status : Under Development
*/
#include "SensorIO.h"
#include <Wire.h>




// Timer Object
hw_timer_t *timer = NULL;

// Semaphore Objects
SemaphoreHandle_t sensorTimerSemaphore = NULL;

hw_timer_t *configureSensorTimer(uint64_t micro)
{

  sensorTimerSemaphore = xSemaphoreCreateBinary();

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &SensorTimerInterrupt, true);
  timerAlarmWrite(timer, micro, true);
  timerAlarmEnable(timer);
  return timer;
}

void IRAM_ATTR SensorTimerInterrupt()
{
  BaseType_t woken = pdFALSE;
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
    xSemaphoreGiveFromISR(sensorTimerSemaphore, &woken);
  }
}
