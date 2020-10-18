#include "Arduino.h"


extern hw_timer_t *timer;
extern SemaphoreHandle_t sensorTimerSemaphore;

hw_timer_t* configureSensorTimer(uint64_t micro);
void IRAM_ATTR SensorTimerInterrupt();
