#ifndef LED_H
#define LED_H

extern const int GREEN_PIN;
extern const int RED_PIN;
extern const int BLUE_PIN;
extern const int ORANGE_PIN;
extern const int YELLOW_PIN;

void greenOn();
void redOn();
void blueOn();
void orangeOn();
void yellowOn();

void allLedOff();
void ledInit();
void sapinNoel();

void greenOn10();
bool endLed();

#endif