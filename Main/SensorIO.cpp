#include "SensorIO.h"

// Sensor variables
// TODO Add decleration

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
  xSemaphoreTakeFromISR(sensorVarSemaphore);
  // TODO Add sensor polling.

  xSemaphoreGiveFromISR(sensorVarSemaphore);

  //-----------Send Semaphore to task
  BaseType_t woken = pdFALSE;
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
    xSemaphoreGiveFromISR(sensorTimerSemaphore, &woken);
  }
}
