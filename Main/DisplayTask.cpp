/*
Developer 1 name : Jesse McDonald
Course : ENGR 2704
Project : CLEVER
File name : DisplayTask.cpp
Target Machine : ESP32
Operating System : freeRTOS
Integrated Development Environment(IDE) : Visual Studio Code and Arduino
Language : C++
Operational Status : Under Development
*/

#include <SPI.h>
#include <SSD_13XX.h>

//Project imports
#include "DisplayTask.h"
#include "TouchInterrupts.h"

SSD_13XX disp = SSD_13XX(_cs, _dc);

//// Function to initialize the display.
//void buildDisplay(){
//  //TODO Build display
//}

void printToDisplay(String to_print)
{
    disp.clearScreen();
    for (int i = 0; i < to_print.length(); i++)
        disp.write(to_print[i]);
}

// Task to Run the display
void taskDisplay(void *pvParameters)
{
    disp.begin(false);
    disp.setTextColor(WHITE);
    disp.setTextScale(3);

    //Enable touch interrupts
    interruptsInit();

    UBaseType_t uxHighWaterMark;

    /* Inspect our own high water mark on entering the task. */
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    //    Serial.println(uxHighWaterMark);

    for (;;)
    {
        xSemaphoreTake(xTouchSemaphore, portMAX_DELAY);

        disp.setCursor(0, 0);
        if (leftTouchDetected)
        {
            printToDisplay("left_pushed");
            leftTouchDetected = false;
        }

        if (rightTouchDetected)
        {
            printToDisplay("right_pushed");
            rightTouchDetected = false;
        }

        if (upTouchDetected)
        {
            printToDisplay("up_pushed");
            upTouchDetected = false;
        }

        if (downTouchDetected)
        {
            printToDisplay("down_pushed");
            downTouchDetected = false;
        }
        if (centerTouchDetected)
        {
            printToDisplay("center_pushed");
            centerTouchDetected = false;
        }
    }
}
