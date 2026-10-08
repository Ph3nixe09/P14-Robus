#ifndef PIDB_H
#define PIDB_H

void stop();
void beep(int count);
void stop();
void move(float speed, bool backwards = false);
void driveStraight(float speed = 0.5);
void spinRight(float speed);
void spinLeft(float speed);

#endif