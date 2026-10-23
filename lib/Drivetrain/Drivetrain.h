#ifndef Drivetrain_h
#define Drivetrain_h

#include <Arduino.h>

// Global variables
extern int last_left;
extern int last_right;

// Pins
extern const int GREEN_PIN;
extern const int RED_PIN;

// Buzzer
void beep(int count);

// Bumpers
bool getBumberRear();
bool getBumberFront();
bool getBumberLeft();
bool getBumberRight();
bool getBumberAny();

// Motors
void stop();
void move(float speed, bool backwards = false);
void spinRight(float speed);
void spinLeft(float speed);
void moveIndependant(float speed_left, float speed_right);

// Encoders
int getEncoderLeft();
int getEncoderRight();
int getResetEncoderLeft();
int getResetEncoderRight();

void resetEncoderLeft();
void resetEncoderRight();
void ResetEncoderAll();

// Object detection
bool detectWall();

bool getLineLeft();
bool getLineMid();
bool getLineRight();

void drivetrainInit();

#endif
