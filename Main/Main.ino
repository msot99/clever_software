/*
  Developer 1 name : Jesse McDonald
  Course : ENGR 2704
  Project : CLEVER
  File name : FreeRTOS.ino
  Target Machine : ESP32
  Operating System : freeRTOS
  Integrated Development Environment(IDE) : Visual Studio Code and Arduino
  Language : C++
  Operational Status : Under Development
*/

#include "DisplayTask.h"
#include "SensorProcessing.h"
#include "BreathControl.h"

void setup()
{

  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
      taskDisplay, "taskDisplay",
      2048 // Stack size
      ,
      NULL,
      2 // Priority
      ,
      NULL, 1);

  xTaskCreatePinnedToCore(
      taskFingerSensor, "taskFingerSensor" // A name just for humans
      ,
      2048 // This stack size can be checked & adjusted by reading the Stack Highwater
      ,
      NULL, 2 // Priority
      ,
      NULL, 1); // Core used

   xTaskCreatePinnedToCore(
       taskPressureSensor, "taskPressureSensor" // A name just for humans
       ,
       8000 // This stack size can be checked & adjusted by reading the Stack Highwater
       ,
       NULL, 2 // Priority
       ,
       NULL, 1); // Core used

  xTaskCreatePinnedToCore(
      taskBreath, "taskBreath" // A name just for humans
      ,
      2048 // This stack size can be checked & adjusted by reading the Stack Highwater
      ,
      NULL, 2 // Priority
      ,
      NULL, 0); // Core used

  xTaskCreatePinnedToCore(
      taskAlert, "taskSMS" // A name just for humans
      ,
      8000 // This stack size can be checked & adjusted by reading the Stack Highwater
      ,
      NULL, 2 // Priority
      ,
      NULL, 0); // Core used
}

void loop()
{
  // Empty. Things are done in Tasks.
}
