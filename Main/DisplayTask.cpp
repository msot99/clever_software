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

//struct for display data
struct displayData data[2][3];
// Var for which of displayData's adjustiable items is selected
int selectedItem = 0;

// Varibles for breathing
struct pumpData pump;
extern SemaphoreHandle_t xPumpDataSemaphore = xSemaphoreCreateMutex();

//Variables for pressure
extern SemaphoreHandle_t xPressureSemaphore = xSemaphoreCreateMutex();
float pressure;

//Variables for finger
struct fingerData finger;
extern SemaphoreHandle_t xFingerSemaphore = xSemaphoreCreateMutex();

String tidalVolumeOptions[] = {".5", "1.0", "1.5"};

// Function to initialize the display.
void updateDisplay()
{

    //Update data from sensors
    xSemaphoreTake(xFingerSemaphore, portMAX_DELAY);
    data[1][1].value = String(finger.hr);
    data[1][2].value = String(finger.o2);
    xSemaphoreGive(xFingerSemaphore);

    xSemaphoreTake(xPressureSemaphore, portMAX_DELAY);
    char result[50];
    sprintf(result, "%.1f", pressure / 98.1);
    data[1][0].value = String(result);
    xSemaphoreGive(xPressureSemaphore);

    // Inital Settings
    disp.setBackground(BLACK);
    disp.clearScreen();
    disp.setTextScale(1);

    // String Objects
    String firstRowText[][3] = {{" T. Vol.", "     Prs", "   B/M"}, {"Cur Prs", "     HR", "O2 Sat"}};
    String secondRowText[][3] = {{"  (L)", "(cmH20)", ""}, {"  (cmH20)", "(BPM)", "(%02)"}};

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
        printToDisplay(String(data[0][i].value));
        disp.setCursor(bufw * (i + 1) + wid * i + 8, secondRowHeight + 10);
        printToDisplay(String(data[1][i].value));
    }
}

void printToDisplay(String to_print)
{
    for (int i = 0; i < to_print.length(); i++)
        disp.write(to_print[i]);
}

void initializeNumberArrays()
{
    xSemaphoreTake(xPumpDataSemaphore, portMAX_DELAY);
    // initialize Tidal Volume
    data[0][selectedItem].color = RED;
    data[0][0].value = tidalVolumeOptions[pump.tidalVolume];
    data[0][2].value = String(pump.bpm);
    xSemaphoreGive(xPumpDataSemaphore);
}

// Task to Run the display
void taskDisplay(void *pvParameters)
{

    //Create Semaphores
    xPumpDataSemaphore = xSemaphoreCreateMutex();

    disp.begin(false);

    initializeNumberArrays();

    updateDisplay();

    //Enable touch interrupts
    interruptsInit();

    for (;;)
    {

        xSemaphoreTake(xTouchSemaphore, portMAX_DELAY);
        Serial.println("Touch");
        //--------------Handles left touch-----------------------
        if (leftTouchDetected)
        {
            leftTouchDetected = false;
            if (selectedItem > 0)
            {
                data[0][selectedItem].color = WHITE;
                selectedItem--;
                data[0][selectedItem].color = RED;
            }
            updateDisplay();
            leftTouchDetected = false;
        }

        //--------------Handles right touch-----------------------
        if (rightTouchDetected)
        {

            if (selectedItem < 2)
            {
                data[0][selectedItem].color = WHITE;
                selectedItem++;
                data[0][selectedItem].color = RED;
            }

            updateDisplay();
            rightTouchDetected = false;
        }

        // Take the mutexs
        xSemaphoreTake(xPumpDataSemaphore, portMAX_DELAY);

        //--------------Handles UP touch-----------------------
        if (upTouchDetected)
        {
            // Change Tidal Volume
            if (selectedItem == 0 && pump.tidalVolume < 2)
            {
                pump.tidalVolume++;
                data[0][0].value = tidalVolumeOptions[pump.tidalVolume];
            }
            //change BPM
            else if (selectedItem == 2 && pump.bpm < 20)
            {
                pump.bpm++;
                data[0][2].value = String(pump.bpm);
            }

            updateDisplay();
            upTouchDetected = false;
        }

        //--------------Handles down touch-----------------------
        if (downTouchDetected)
        {
            // Change Tidal Volume
            if (selectedItem == 0 && pump.tidalVolume > 0)
            {
                pump.tidalVolume--;
                data[0][0].value = tidalVolumeOptions[pump.tidalVolume];
            }

            //change BPM
            else if (selectedItem == 2 && pump.bpm > 5)
            {
                pump.bpm--;
                data[0][2].value = String(pump.bpm);
            }

            updateDisplay();
            downTouchDetected = false;
        }

        //--------------Handles center touch-----------------------
        if (centerTouchDetected)
        {
            pump.trigger = true;
            int i = 0;
            while (touchRead(T7) < 10)
            {
                i++;
                vTaskDelay(5 / portTICK_PERIOD_MS);
            }

            //Switch to Controlled
            if (i > 200)
            {
                pump.trigger = false;
                pump.controlledMode = !pump.controlledMode;
            }

            // PowerOff
            if (i > 1000)
            {
                pump.trigger = false;
                pump.off = !pump.off;
            }
        }

        //Give mutexs
        xSemaphoreGive(xPumpDataSemaphore);

        vTaskDelay(100 / portTICK_PERIOD_MS);
        touch_pad_clear_status();
        touch_pad_intr_enable();
    }
}
