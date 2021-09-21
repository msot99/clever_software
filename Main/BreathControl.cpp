/*
  Developer 1 name : Jesse McDonald
  Course : ENGR 2704
  Project : CLEVER
  File name : BreathControl.cpp
  Target Machine : ESP32
  Operating System : freeRTOS
  Integrated Development Environment(IDE) : Visual Studio Code and Arduino
  Language : C++
  Operational Status : Under Development
*/

#include "BreathControl.h"
#include "DisplayTask.h"
#include <WiFi.h>

const int dirPin = 4;
const int stepPin = 15;
const int stepsPerRevolution = 400;

//Bring Struct From DisplayTask
extern struct pumpData pump;
extern SemaphoreHandle_t xPumpDataSemaphore;

// Variables for controlled breath
int degOfVolumes[] = {195, 390, 584};
int stepsToTake;
float partOfBreath; //convert to period in us.

int bpm;
int tidal;
bool controlled;
bool off;
bool triggeredBreath;

//Timer code for emergency
hw_timer_t *timer2 = NULL;
SemaphoreHandle_t xEmergencySemaphore = xSemaphoreCreateBinary();

void controlledBreath()
{

  partOfBreath = 60.0 / (3 * pump.bpm) * 1000000;
  stepsToTake = degOfVolumes[pump.tidalVolume] * 400 / 360;

  //Breath In
  digitalWrite(dirPin, HIGH);
  for (int i = 0; i < stepsToTake; i++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(partOfBreath / stepsToTake);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(partOfBreath / stepsToTake);

    // Let the Scheduler have some time
    if (i % 50 == 0)
      vTaskDelay(10);
  }

  //Breath Out
  digitalWrite(dirPin, LOW);
  for (int i = 0; i < stepsToTake; i++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(partOfBreath / (2 * stepsToTake));
    digitalWrite(stepPin, LOW);
    delayMicroseconds(partOfBreath / (2 * stepsToTake));

    // Let the Scheduler have some time
    if (i % 50 == 0)
      vTaskDelay(10);
  }
}

// Task to Run the display
void taskBreath(void *pvParameters)
{
  // Wait a hot minute to let the other tasks setup
  vTaskDelay(200 / portTICK_PERIOD_MS);

  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Loop to repeat processing data endlessly.
  while (1)
  {
    if (xPumpDataSemaphore != NULL)
    {

      xSemaphoreTake(xPumpDataSemaphore, portMAX_DELAY);

      bpm = pump.bpm;
      tidal = pump.tidalVolume;
      controlled = pump.controlledMode;
      off = pump.off;
      triggeredBreath = pump.trigger;
      pump.trigger = false;

      xSemaphoreGive(xPumpDataSemaphore);

      // Off mode
      if (off)
      {
        Serial.println("Off");
        vTaskDelay(10);
      }
      // Controlled Mode
      else if (controlled)
      {
        Serial.println("Controlled105");
        controlledBreath();
        timerWrite(timer2, 0);
      }
      // Triggered Mode
      else if (triggeredBreath)
      {
        Serial.println("Triggered");
        controlledBreath();

        timerWrite(timer2, 0);
      }
    }
    vTaskDelay(10);
  }
}

void taskAlert(void *pvParameters)
{
  //Setup Timer
  timer2 = timerBegin(1, 80, true);
  timerAttachInterrupt(timer2, &SensorTime2rInterrupt, true);
  while (1)
  {

    timerAlarmWrite(timer2, 30000000, true);
    timerAlarmEnable(timer2);

    xSemaphoreTake(xEmergencySemaphore, portMAX_DELAY);

    xSemaphoreTake(xPumpDataSemaphore, portMAX_DELAY);
    pump.trigger = true;
    xSemaphoreGive(xPumpDataSemaphore);

    Serial.println("SENDING SMS");

    WiFi.begin("letnet-connect", "letnet-connect");

    while (WiFi.status() != WL_CONNECTED)
      ;

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    WiFiClient client;
    const int smtp = 25;
    if (!client.connect("smtpin01.vzw.a.cloudfilter.net", smtp))
    {
      Serial.println("connection failed");
      return;
    }

    Serial.print(client.readStringUntil('\r'));
    client.print("HELO vtext.com\r\n");
    Serial.print(client.readStringUntil('\r'));
    client.print("MAIL FROM: <clever@msot99.com>\r\n");
    Serial.print(client.readStringUntil('\r'));
    client.print("RCPT TO: <3023042375@vtext.com>\r\n");
    Serial.print(client.readStringUntil('\r'));
    client.print("DATA\r\n");
    Serial.print(client.readStringUntil('\r'));
    client.print("\n   Patient 1 has stopped breathing.\r\n");
    client.print("\r\n.\r\n");
    Serial.print(client.readStringUntil('\r'));
    client.print("QUIT");
  }
}

void IRAM_ATTR SensorTime2rInterrupt()
{
  BaseType_t woken = pdFALSE;
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
    xSemaphoreGiveFromISR(xEmergencySemaphore, &woken);
  }
}
