#include "Arduino.h"

// Timer object and Semaphores
extern hw_timer_t *timer;
extern SemaphoreHandle_t sensorTimerSemaphore;
extern SemaphoreHandle_t sensorVarSemaphore;

//Sensor Variables
//TODO add sensor ISR storage

hw_timer_t *configureSensorTimer(uint64_t micro);
void IRAM_ATTR SensorTimerInterrupt();
