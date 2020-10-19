
#include "Arduino.h"

#define touchThreshold 40

extern SemaphoreHandle_t xTouchSemaphore;

extern bool leftTouchDetected;
extern bool rightTouchDetected;
extern bool upTouchDetected;
extern bool downTouchDetected;
extern bool centerTouchDetected;

void interruptsInit();

void gotTouchLeft();
void gotTouchRight();
void gotTouchUp();
void gotTouchDown();
void gotTouchCenter();
