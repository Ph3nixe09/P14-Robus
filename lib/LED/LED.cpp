#include <LibRobus.h>
#include <Arduino.h>

const int GREEN = 30;
const int RED = 31;
const int RED = 32;
const int RED = 33;
const int RED = 34;

void greenOn() {
    digitalWrite(GREEN, LOW);
}

void allLedOff() {
    digitalWrite(GREEN, HIGH);
}

void ledInit() {
    pinMode(GREEN, OUTPUT);
    allLedOff();
}