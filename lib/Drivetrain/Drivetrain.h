#ifndef Drivetrain_h
#define Drivetrain_h

#include <Arduino.h>

// Global variables
extern int last_left;
extern int last_right;

// Pins
extern const int GREEN_PIN;
extern const int RED_PIN;

// Timer
void startDriveTimer(long duration = 200);

// Buzzer
void beep(int count);

// Bumpers
bool getBumberRear();
bool getBumberFront();
bool getBumberLeft();
bool getBumberRight();
bool getBumberAny();

// Motors
bool stop();
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
int getIRFront();
int getIRRight();
bool detectWallFront(int distance = 275);
bool detectWallRight();




bool getLineMid_L();
bool getLineMid_M();
bool getLineMid_R();
bool getLineLeft_L();
bool getLineLeft_M();
bool getLineLeft_R();
bool getLineRight_L();
bool getLineRight_M();
bool getLineRight_R();
bool getLineMid();
bool getLineLeft();
bool getLineRight();

void testLineSensor();

void drivetrainInit();

#endif
