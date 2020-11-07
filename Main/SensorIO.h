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

#include "Arduino.h"

// Timer object and Semaphores
extern hw_timer_t *timer;
extern SemaphoreHandle_t sensorTimerSemaphore;
extern SemaphoreHandle_t sensorVarSemaphore;

//Sensor Variables
//TODO5 Add sensor declariations

hw_timer_t *configureSensorTimer(uint64_t micro);
void IRAM_ATTR SensorTimerInterrupt();
void sensorConfiguration();
