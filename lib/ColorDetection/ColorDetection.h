#ifndef COLORDETECTION_H
#define COLORDETECTION_H
#include <Adafruit_TCS34725.h>
#include <LED.h>

extern Adafruit_TCS34725 tcs;
void detectColorInit();
Color colorValues();
//void printRGB();

#endif