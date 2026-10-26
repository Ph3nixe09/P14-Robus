#include <LibRobus.h>
#include <Arduino.h>

const int GREEN_PIN = 30;
const int RED_PIN = 31;
const int BLUE_PIN = 32;
const int ORANGE_PIN = 33;
const int YELLOW_PIN = 34;

long start_time;
bool timer_on = false;
long led_timer_duration;

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

void allLedOff() {
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(ORANGE_PIN, HIGH);
    digitalWrite(YELLOW_PIN, HIGH);
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

// MUST TEST
// Timers 
// CES FONCTIONS PEUVENT ÊTRE RECRÉÉS POUR TOUS LES AUTRES SYSTÈMES
void startLedTimer(long duration = 10000) { // temps en millis
    start_time = millis();
    timer_on = true;
    led_timer_duration = duration;
}

bool endLed() {
    long delta = millis() - start_time;

    if (delta > led_timer_duration) {
        allLedOff();
        return true; // retourne True si le temps est atteint
    } else {
        return false;
    }
}

void greenOn10() {
    allLedOff();
    orangeOn();
    startLedTimer(5000);
}