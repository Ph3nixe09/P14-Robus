#include <LibRobus.h>
#include <Arduino.h>

const int GREEN = 30;
const int RED = 31;
const int BLUE = 32;
const int ORANGE = 33;
const int YELLOW = 34;

void greenOn() {
    digitalWrite(GREEN, LOW);
}

void redOn() {
    digitalWrite(RED, LOW);
}

void blueOn() {
    digitalWrite(BLUE, LOW);
}

void orangeOn() {
    digitalWrite(ORANGE, LOW);
}

void yellowOn() {
    digitalWrite(YELLOW, LOW);
}


void allLedOff() {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, HIGH);
    digitalWrite(ORANGE, HIGH);
    digitalWrite(YELLOW, HIGH);
}

void ledInit() {
    pinMode(GREEN, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(ORANGE, OUTPUT);
    pinMode(YELLOW, OUTPUT);
    allLedOff();
}

void sapinNoel() {
    int delta = 100;

    greenOn();
    delay(delta);
    redOn();
    delay(delta);
    blueOn();
    delay(delta);
    orangeOn();
    delay(delta);
    yellowOn();
    delay(delta);
    allLedOff();
    delay(delta);
}