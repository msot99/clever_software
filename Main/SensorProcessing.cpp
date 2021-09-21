/*
  Developer 1 name : Jesse McDonald
  Course : ENGR 2704
  Project : CLEVER
  File name : SensorProcessing.cpp
  Target Machine : ESP32
  Operating System : freeRTOS
  Integrated Development Environment(IDE) : Visual Studio Code and Arduino
  Language : C++
  Operational Status : Under Development
*/

#include "SensorProcessing.h"
#include "SensorIO.h"
#include "DisplayTask.h"

//Non custom imports
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

#include "Adafruit_MPRLS.h"

// I2C Semaphore
SemaphoreHandle_t xi2cSemaphore = xSemaphoreCreateMutex();

// Period to poll sensors in micros
#define SENSOR_READ_PERIOD 100000

// Data shared with DisplayTask
extern struct fingerData finger;
extern SemaphoreHandle_t xFingerSemaphore;

// ---------------------------------------------Heart Rate Code--------------------
#define MAX_BRIGHTNESS 255

//Settings for HR sensor
byte ledBrightness = 60; //Options: 0=Off to 255=50mA
byte sampleAverage = 4;  //Options: 1, 2, 4, 8, 16, 32
byte ledMode = 2;        //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
byte sampleRate = 100;   //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
int pulseWidth = 411;    //Options: 69, 118, 215, 411
int adcRange = 4096;     //Options: 2048, 4096, 8192, 16384

// Sensor variables
MAX30105 particleSensor;

// HR sensor vars
uint32_t irBuffer[100];  //infrared LED sensor data
uint32_t redBuffer[100]; //red LED sensor data

int32_t bufferLength;  //data length
int32_t spo2;          //SPO2 value
int8_t validSPO2;      //indicator to show if the SPO2 calculation is valid
int32_t heartRate;     //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

int HR_SPO2();

// Pressure Sensor Vars
#define RESET_PIN -1 // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN -1   // set to any GPIO pin to read end-of-conversion by pin
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);
extern float pressure;
extern SemaphoreHandle_t xPressureSemaphore;

// Task to getPressure Sensor
void taskPressureSensor(void *pvParameters)
{

  // Sensor configuration
  mpr.begin(0x18);

  //Configure SensorIO which will read sensors and send data over.
  hw_timer_t *sensorTimer = configureSensorTimer(SENSOR_READ_PERIOD);

  // Loop to repeat processing data endlessly.
  while (1)
  {

    xSemaphoreTake(sensorTimerSemaphore, portMAX_DELAY);
    xSemaphoreTake(xPressureSemaphore, portMAX_DELAY);
    xSemaphoreTake(xi2cSemaphore, portMAX_DELAY);
    pressure = mpr.readPressure();
    xSemaphoreGive(xi2cSemaphore);
    xSemaphoreGive(xPressureSemaphore);
    vTaskDelay(10);
  }
}

void taskFingerSensor(void *pvParameters)
{

  //Initialize Sensor
  particleSensor.begin(Wire, I2C_SPEED_FAST);

  //Intialize HR Sensor
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

  bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps
  while (1)
  {
    //read the first 100 samples, and determine the signal range
    for (byte i = 0; i < bufferLength; i++)
    {
      xSemaphoreTake(xi2cSemaphore, portMAX_DELAY);
      while (particleSensor.available() == false) //do we have new data?
      {
        particleSensor.check(); //Check the sensor for new data
        xSemaphoreGive(xi2cSemaphore);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        xSemaphoreTake(xi2cSemaphore, portMAX_DELAY);
      }

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample
      xSemaphoreGive(xi2cSemaphore);
      vTaskDelay(10);
    }

    //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    if (xFingerSemaphore != NULL)
    {
      xSemaphoreTake(xFingerSemaphore, portMAX_DELAY);

      if (validHeartRate == 1)
        finger.hr = heartRate;

      if (validSPO2 == 1)
        finger.o2 = spo2;

      xSemaphoreGive(xFingerSemaphore);
    }
  }
}
