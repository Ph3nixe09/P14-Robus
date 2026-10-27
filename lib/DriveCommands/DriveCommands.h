#ifndef DRIVE_COMMANDS_H
#define DRIVE_COMMANDS_H

extern int goBackListe[5];

// Constants
extern const float WHEEL_SPACE;
extern const float WHEEL_SPACE_CIRCUMFERENCE;
extern const float WHEEL_DIAMETER;
extern const float WHEEL_CIRCUMFERENCE_INCHES;
extern const float WHEEL_CIRCUMFERENCE;
extern const float PULSES_PER_ROTATION;

// Commands
void driveStraight(float speed = 0.5);
void turnStraight(float speed = 0.3);

void tourner(float angle = 0, float speed = 0);

int driveToWall();
void driveDistance(float distance = 0, float vitesse = 0);
bool endMoving();
float distanceAtteint(int type = 0);

#endif