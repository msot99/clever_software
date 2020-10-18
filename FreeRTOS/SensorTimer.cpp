#include "SensorTimer.h"


hw_timer_t * timer = NULL;
SemaphoreHandle_t sensorTimerSemaphore = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

hw_timer_t* configureSensorTimer(uint64_t micro) {

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

  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    xSemaphoreGiveFromISR(sensorTimerSemaphore, &woken); // give the sensorTimerSemaphore semaphore
  }


}
