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
#include "Adafruit_MPRLS.h"
#include "MAX30105.h"
#include "spo2_algorithm.h"

// Sensor variables
MAX30105 particleSensor;

#define MAX_BRIGHTNESS 255

// HR sensor vars
uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data

int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

byte pulseLED = 11; //Must be on PWM pin
byte readLED = 13; //Blinks with each data read

int HR_SPO2();

// TODO3 Add decleration
  //Initialize HR LEDS
  pinMode(pulseLED, OUTPUT);
  pinMode(readLED, OUTPUT);

  //Settings for HR sensor
  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  
  //Intialize HR Sensor
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

// Timer Object
hw_timer_t *timer = NULL;

// Semaphore Objects
SemaphoreHandle_t sensorTimerSemaphore = NULL;
SemaphoreHandle_t sensorVarSemaphore = NULL;

hw_timer_t *configureSensorTimer(uint64_t micro)
{

  sensorTimerSemaphore = xSemaphoreCreateBinary();
  sensorVarSemaphore = xSemaphoreCreateMutex();

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &SensorTimerInterrupt, true);
  timerAlarmWrite(timer, micro, true);
  timerAlarmEnable(timer);
  return timer;
}

void IRAM_ATTR SensorTimerInterrupt()
{

  //-----------Read Sensor Data
    bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

  //read the first 100 samples, and determine the signal range
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) //do we have new data?
      particleSensor.check(); //Check the sensor for new data

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); //We're finished with this sample so move to next sample

  }

  //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  
  xSemaphoreTakeFromISR(sensorVarSemaphore);
  // TODO4 Add sensor polling.

  xSemaphoreGiveFromISR(sensorVarSemaphore);

  //-----------Send Semaphore to task
  BaseType_t woken = pdFALSE;
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
    xSemaphoreGiveFromISR(sensorTimerSemaphore, &woken);
  }
}
