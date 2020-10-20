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

void setup()
{

  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
      taskProcessSensors, "taskProcessSensors" // A name just for humans
      ,
      8000 // This stack size can be checked & adjusted by reading the Stack Highwater
      ,
      NULL, 2 // Priority
      ,
      NULL, 1); // Core used

  xTaskCreatePinnedToCore(
      taskDisplay, "taskDisplay",
      2048 // Stack size
      ,
      NULL,
      1 // Priority
      ,
      NULL, 1);
}

void loop()
{
  // Empty. Things are done in Tasks.
}
