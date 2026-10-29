#ifndef COLORDETECTION_H
#define COLORDETECTION_H
#include <Adafruit_TCS34725.h>

extern Adafruit_TCS34725 tcs;
void detectColorInit();
void colorValues();
//void printRGB();

#endif