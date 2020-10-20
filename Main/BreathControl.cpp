/*
Developer 1 name : Jesse McDonald
Course : ENGR 2704
Project : CLEVER
File name : BreathControl.cpp
Target Machine : ESP32
Operating System : freeRTOS
Integrated Development Environment(IDE) : Visual Studio Code and Arduino
Language : C++
Operational Status : Under Development
*/
#include BreathControl.h

//Variables for breathing
int breathsPerMin = 20;

// Ventilator Targets
enum targets
{
    Volume,
    Pressure
} target;

// Ventilator Modes
enum modes
{
    Support,
    Control
} mode;

// Function for performing support breathing
void supportBreathing()
{
    //Given the variables breathesPerMin, pressure, target, and ratio

    // Loop breath out
    for (;;)
    {
    }

    // Loop breath out
    for (;;)
    {
    }
}

// Function for performing controlled breathing
void controlBreathing()
{
    //Given the variables breathesPerMin, pressure, target, and ratio

    // Loop breath out
    for (;;)
    {
    }

    // Loop breath out
    for (;;)
    {
    }
}

// Task to Run the display
void taskProcessSensors(void *pvParameters)
{
    // Loop to repeat processing data endlessly.
    while (1)
    {
    }
}
