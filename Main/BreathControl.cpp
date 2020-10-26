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
#include Arduino.h
#include Wire.h
#include BasicStepperDriver.h

//Initiate motor
#define MOTOR_STEPS 200
#define MICROSTEPS 1
#define DIR 23
#define STEP 24

BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);
stepper.begin(RPM, MICROSTEPS);

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
