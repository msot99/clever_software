
#include "SensorMath.h"
#include "SensorIO.h"

// Period to poll sensors in micros
#define SENSOR_READ_PERIOD 120000

// Task to Run the display
void taskProcessSensors(void *pvParameters)
{
  //Configure SensorIO which will read sensors and send data over.
  hw_timer_t *sensorTimer = configureSensorTimer(SENSOR_READ_PERIOD);
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
