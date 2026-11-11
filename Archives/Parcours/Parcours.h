
#ifndef PARCOURS_H
#define PARCOURS_H

#include <Arduino.h>
#include <DriveCommands.h>

enum Position {
    Left,
    Middle,
    Right
};

Position left();
Position middle();
Position right();

void logic();

void positionMemory();
int* reverse_goBackList(int* list);
void goBackStep(int pastX, int futurX, int dist);
void goBack(int* liste);

#endif
