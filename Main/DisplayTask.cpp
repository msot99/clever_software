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

//Project imports
#include "DisplayTask.h"

SSD_13XX disp = SSD_13XX(_cs, _dc);
struct displayData data[2][3];

int selected = 2;

// Function to initialize the display.
void updateDisplay()
{
    //TODO1 Build display

    // Inital Settings
    disp.setBackground(BLACK);
    disp.clearScreen();
    disp.setTextScale(1);

    // String Objects
    String firstRowText[][3] = {{" T. Vol.", "     Prs", "   B/M"}, {"Cur Vol", "     HR", "O2 Sat"}};
    String secondRowText[][3] = {{"  (L)", "(cmH20)", ""}, {"  (L)", "(BPM)", "(%02)"}};

    // Number objects

    //Location Variables
    short bufh = 3;
    short bufw = 10;
    short wid = (128 - (4 * bufw)) / 3;
    short textHeight = 29;
    short firstRowHeight = 0;
    short secondRowHeight = firstRowHeight + bufh * 2 + textHeight + wid;

    for (int i = 0; i < 3; i++)
    {

        // Create rows of squares
        disp.drawRect(bufw * (i + 1) + wid * i, firstRowHeight, wid, wid, data[0][i].color, data[0][i].color, false);
        disp.drawRect(bufw * (i + 1) + wid * i, secondRowHeight, wid, wid, 15000, 45000, false);

        // Write first two rows of text
        disp.setCursor(bufw * (i + 1) + wid * i, firstRowHeight + bufh + wid);
        printToDisplay(firstRowText[0][i]);
        disp.setCursor(bufw * (i + 1) + wid * i, firstRowHeight + bufh + wid + 15);
        printToDisplay(secondRowText[0][i]);

        // Write second two rows of text
        disp.setCursor(bufw * (i + 1) + wid * i, secondRowHeight + bufh + wid);
        printToDisplay(firstRowText[1][i]);
        disp.setCursor(bufw * (i + 1) + wid * i, secondRowHeight + bufh + wid + 15);
        printToDisplay(secondRowText[1][i]);

        // fill in squares
        disp.setCursor(bufw * (i + 1) + wid * i + 8, firstRowHeight + 10);
        printToDisplay(String(124));
        disp.setCursor(bufw * (i + 1) + wid * i + 8, secondRowHeight + 10);
        printToDisplay(String(124));
    }
}

void printToDisplay(String to_print)
{
    for (int i = 0; i < to_print.length(); i++)
        disp.write(to_print[i]);
}

void initializeNumberArrays()
{

    // initialize Tidal Volume
    data[0][0].color = RED;
}

// Task to Run the display
void taskDisplay(void *pvParameters)
{
    disp.begin(false);

    updateDisplay();

    //Enable touch interrupts
    interruptsInit();

    UBaseType_t uxHighWaterMark;

    /* Inspect our own high water mark on entering the task. */
    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    //    Serial.println(uxHighWaterMark);

    for (;;)
    {

        //TODO2 Add Display manipulation
        xSemaphoreTake(xTouchSemaphore, portMAX_DELAY);
        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);

        if (leftTouchDetected)
        {
            leftTouchDetected = false;
            if (selected > 0)
            {
                data[0][selected].color = WHITE;
                selected--;
                data[0][selected].color = RED;
                updateDisplay();
            }
            Serial.print("Left");
            Serial.println(selected);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            leftTouchDetected = false;
        }

        if (rightTouchDetected)
        {

            if (selected < 2)
            {
                data[0][selected].color = WHITE;
                selected++;
                data[0][selected].color = RED;
                updateDisplay();
            }
            Serial.print("Right");
            Serial.println(selected);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            rightTouchDetected = false;
        }

        if (upTouchDetected)
        {
            //            printToDisplay("up_pushed");
            upTouchDetected = false;
        }

        if (downTouchDetected)
        {
            //            printToDisplay("down_pushed");
            downTouchDetected = false;
        }
        if (centerTouchDetected)
        {
            //            printToDisplay("center_pushed");
            centerTouchDetected = false;
        }
        touch_pad_intr_enable();
    }
}
