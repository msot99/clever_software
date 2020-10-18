
#include "SensorTask.h"
#include "SensorTimer.h"

// Period to poll sensors in micros
#define SENSOR_READ_PERIOD 120000



// Task to Run the display
void taskReadSensors(void *pvParameters)
{
    //Configure sensorTimer
    hw_timer_t* sensorTimer = configureSensorTimer(SENSOR_READ_PERIOD);
    UBaseType_t uxHighWaterMark;
  
    //TODO Sensor configuration

    for (;;)
    {
       Serial.println(uxTaskGetStackHighWaterMark(NULL));
      //Semaphore to wait on timer
      xSemaphoreTake(sensorTimerSemaphore, portMAX_DELAY);


      // TODO Add sensor polling.


       
    }
}
