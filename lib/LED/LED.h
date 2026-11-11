#ifndef LED_H
#define LED_H

extern const int GREEN_PIN;
extern const int RED_PIN;
extern const int BLUE_PIN;
extern const int ORANGE_PIN;
extern const int YELLOW_PIN;

enum Color {
  GREEN,
  RED,
  BLUE,
  ORANGE,
  YELLOW,
  NONE
};

void greenOn();
void redOn();
void blueOn();
void orangeOn();
void yellowOn();

void allLedOff();
void ledInit();
void sapinNoel();

void ledOn10(Color color);
void endLed();

#endif