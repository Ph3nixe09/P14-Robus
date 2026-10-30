#ifndef COLORDETECTION_H
#define COLORDETECTION_H
#include <GroveColorSensor.h>
// #include <Adafruit_TCS34725.h>

extern GroveColorSensor tcs;
// extern Adafruit_TCS34725 tcs;

void detectColorInit();
void printRGB();

#endif