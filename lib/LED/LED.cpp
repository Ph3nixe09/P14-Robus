#include <LibRobus.h>
#include <Arduino.h>

const int GREEN_PIN = 38;
const int RED_PIN = 39;
const int BLUE_PIN = 40;
const int ORANGE_PIN = 41;
const int YELLOW_PIN = 42;

long green_start_time;
bool green_timer_on = false;
long green_timer_duration;

long red_start_time;
bool red_timer_on = false;
long red_timer_duration;

long blue_start_time;
bool blue_timer_on = false;
long blue_timer_duration;

long orange_start_time;
bool orange_timer_on = false;
long orange_timer_duration;

long yellow_start_time;
bool yellow_timer_on = false;
long yellow_timer_duration;

typedef enum {
  GREEN,
  RED,
  BLUE,
  ORANGE,
  YELLOW,
  NONE
} Color;

// On
void greenOn() {
    digitalWrite(GREEN_PIN, LOW);
}

void redOn() {
    digitalWrite(RED_PIN, LOW);
}

void blueOn() {
    digitalWrite(BLUE_PIN, LOW);
}

void orangeOn() {
    digitalWrite(ORANGE_PIN, LOW);
}

void yellowOn() {
    digitalWrite(YELLOW_PIN, LOW);
}

// Off
void greenOff() {
    digitalWrite(GREEN_PIN, HIGH);
}

void redOff() {
    digitalWrite(RED_PIN, HIGH);
}

void blueOff() {
    digitalWrite(BLUE_PIN, HIGH);
}

void orangeOff() {
    digitalWrite(ORANGE_PIN, HIGH);
}

void yellowOff() {
    digitalWrite(YELLOW_PIN, HIGH);
}

void allLedOff() {
    greenOff();
    redOff();
    blueOff();
    orangeOff();
    yellowOff();
}

void ledInit() {
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(ORANGE_PIN, OUTPUT);
    pinMode(YELLOW_PIN, OUTPUT);
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

// Timers
void startGreenTimer(long duration = 10000) { // temps en millis
    green_start_time = millis();
    green_timer_on = true;
    green_timer_duration = duration;
}
void startRedTimer(long duration = 10000) { // temps en millis
    red_start_time = millis();
    red_timer_on = true;
    red_timer_duration = duration;
}

void startBlueTimer(long duration = 10000) { // temps en millis
    blue_start_time = millis();
    blue_timer_on = true;
    blue_timer_duration = duration;
}

void startOrangeTimer(long duration = 10000) { // temps en millis
    orange_start_time = millis();
    orange_timer_on = true;
    orange_timer_duration = duration;
}

void startYellowTimer(long duration = 10000) { // temps en millis
    yellow_start_time = millis();
    yellow_timer_on = true;
    yellow_timer_duration = duration;
}

void endLed() {
    if (green_timer_on) {
        long delta = millis() - green_start_time;
    
        if (delta > green_timer_duration) {
            greenOff();
            green_timer_on = false;
        }
    }

    if (red_timer_on) {
        long delta = millis() - red_start_time;
    
        if (delta > red_timer_duration) {
            redOff();
            red_timer_on = false;
        }
    }

    if (blue_timer_on) {
        long delta = millis() - blue_start_time;
    
        if (delta > blue_timer_duration) {
            blueOff();
            blue_timer_on = false;
        }
    }

    if (orange_timer_on) {
        long delta = millis() - orange_start_time;
    
        if (delta > orange_timer_duration) {
            orangeOff();
            orange_timer_on = false;
        }
    }

    if (yellow_timer_on) {
        long delta = millis() - yellow_start_time;
    
        if (delta > yellow_timer_duration) {
            yellowOff();
            yellow_timer_on = false;
        }
    }
}

void ledOn10(Color color) {
    switch (color) {
        case GREEN:
            startGreenTimer();
            greenOn();
            break;
        case RED:
            startRedTimer();
            redOn();
            break;
        case BLUE:
            startBlueTimer();
            blueOn();
            break;
        case ORANGE:
            startOrangeTimer();
            orangeOn();
            break;
        case YELLOW:
            startYellowTimer();
            yellowOn();
            break;
    }
}