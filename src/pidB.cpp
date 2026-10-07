#include <LibRobus.h>
#include <Arduino.h>
#include "pidB.h"

// Global variables
int last_left = 0;
int last_right = 0;
int tot_left = 0;
int tot_right = 0;

// Constants
const int GREEN_PIN = 48;
const int RED_PIN = 49;

// Buzzer
void beep(int count){
  for(int i=0;i<count;i++){
    AX_BuzzerON();
    delay(100);
    AX_BuzzerOFF();
    delay(100);  
  }
  delay(400);
}

// Motors
void stop(){
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
};

void move(float speed, bool backwards = false) { // Ajouter True si en arrière
  if (backwards) {
    speed*=-1;
  };
  MOTOR_SetSpeed(RIGHT, speed);
  MOTOR_SetSpeed(LEFT, speed);
};

void spinRight(float speed) {
  MOTOR_SetSpeed(RIGHT, speed);
  MOTOR_SetSpeed(LEFT, -speed);
};

void spinLeft(float speed) {
  MOTOR_SetSpeed(RIGHT, -speed);
  MOTOR_SetSpeed(LEFT, speed);
};

void moveIndependant(float speed_left, float speed_right) {
  MOTOR_SetSpeed(LEFT, speed_left);
  MOTOR_SetSpeed(RIGHT, speed_right);
}

// Encoders
// Les encodeurs retourne 64 pulses par tours
// Reducteur de 50:1, donc 3200 pulses par tours
// Les roues ont une circonference de 3 pouces
int32_t getEncoderLeft() {
  return ENCODER_Read(0);
}

int32_t getEncoderRight() {
  return ENCODER_Read(1);
}

int32_t getResetEncoderLeft() {
  return ENCODER_ReadReset(0);
}

int32_t getResetEncoderRight() {
  return ENCODER_ReadReset(1);
}

void resetEncoderLeft() {
  ENCODER_Reset(0);
}

void resetEncoderRight() {
  ENCODER_Reset(1);
}

void ResetEncoderAll() {
  ENCODER_Reset(0);
  ENCODER_Reset(1);
}

// Commandes
void dance(float speed = 0.4) {
  move(speed);
  delay(500);
  move(speed, true);
  delay(500);
  spinRight(speed);
  delay(500);
  spinLeft(speed);
  delay(500);
  stop();
}

void testEncoders(){
  for (int i = 0; i <= 5; i++) {
    move(0.4);
    Serial.print("Left: ");
    Serial.print(getResetEncoderLeft());
    Serial.print("\n");
    Serial.print("Right: ");
    Serial.print(getResetEncoderRight());
    Serial.print("\n");
    delay(1000);
    stop();
    delay(1000);
  }
}

void driveStraight(float speed = 0.5) {
  float kp = 0.0005;
  
  int left = getEncoderLeft();
  int right = getEncoderRight();
  tot_left += left;
  tot_right += right;
  
  float p = ((right - last_right) - (left - last_left)) * kp;

  float speedLeft = speed + p;
  float speedRight = speed - p;

  moveIndependant(speedLeft, speedRight);
  
  last_left = left;
  last_right = right;
}

void testDriveStraight() {
  move(0.3);
  delay(200);
  for (int i = 0; i < 25; i++) {
    delay(200);
    driveStraight();
  }
  move(0.3);
  delay(200);
  stop();
}

void driveToWall() {
  move(0.3);
  delay(200);
  bool green = 1;
  bool red = 1;
  while (green == 1 and red == 1) {
    driveStraight();
    green = digitalRead(GREEN_PIN);
    red = digitalRead(RED_PIN);
    delay(200);
  }
  move(0.3);
  delay(200);
  stop();
}

// Fonction actionner quand le robot s'allume
void setup() {
  Serial.begin(9600);
  BoardInit();

  pinMode(GREEN_PIN, INPUT);
  pinMode(RED_PIN, INPUT);

  delay(100);
  beep(3);
  delay(200);
  driveToWall();
}

// Boucle du robot
void loop() {
  // Serial.print("Vert");
  // Serial.print(digitalRead(GREEN_PIN));
  // Serial.print("\n");
  // Serial.print("ROUGE");
  // Serial.print(digitalRead(RED_PIN));
  // Serial.print("\n");
  // delay(500);
}