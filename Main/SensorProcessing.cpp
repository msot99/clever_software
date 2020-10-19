
#include "SensorProcessing.h"
#include "SensorIO.h"

// Period to poll sensors in micros
#define SENSOR_READ_PERIOD 120000

// Task to Run the display
void taskProcessSensors(void *pvParameters)
{

  //TODO Sensor configuration

  //Configure SensorIO which will read sensors and send data over.
  hw_timer_t *sensorTimer = configureSensorTimer(SENSOR_READ_PERIOD);

  // Loop to repeat processing data endlessly.
  while (1)
  {
    // Loop to read a sensor a certian number of time before processing it.
    for (int i = 0; i < 1; i++)
    {

      //Semaphore to wait on timer
      xSemaphoreTake(sensorTimerSemaphore, portMAX_DELAY);

      xSemaphoreTake(sensorVarSemaphore);

      // TODO Copy Sensor Data

      xSemaphoreGive(sensorVarSemaphore);
    }

    //Process Data after a certian number of sensors are read.
    //TODO add sensor processing
  }
}