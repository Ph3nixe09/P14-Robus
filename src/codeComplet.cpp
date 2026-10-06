/*
Projet: Déplacement du robot dans le labyrinthe - Robus
Équipe: 14B
Auteurs: Félix Albert, Éléna Barabé, Edouard Farley et Rose Villeneuve
Description: Le script suivant sert à faire avancer le robot dans un labyrinthe inconnu avec des dimensions connues
Date: 04/10/2026
*/

// Librairies
#include <LibRobus.h>
#include <stdio.h>
#include <Arduino.h>

// Global variables
int last_left = 0;
int last_right = 0;
int tot_left = 0;
int tot_right = 0;
int greenpin = 48;
int redpin = 49;
bool green = false;
bool red = false;
int state = 0; // = 0 arrêt 1 = avance 2 = recule 3 = TourneDroit 4 = TourneGauche
int PastState = 0;
float speed = 0.40;
int horizontalPosition = 1;
int verticalPosition = 0;

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

void move(float speed, bool backwards = false) {
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

void driveStraight() {
  float kp = 0.01;
  float ki = 0.00001;
  float kd = 0.003;
  float speed = 0.5;
  
  int left = getEncoderLeft();
  int right = getEncoderRight();
  tot_left += left;
  tot_right += right;
  
  float p = ((right - last_right) / (left - last_left)) * kp;
  float i = (tot_right - tot_left) * ki;
  float d = ((right / left) - (last_right / last_left)) * kd;

  float speedLeft = speed + p + i + d;
  float speedRight = speed - p - i - d;

  moveIndependant(speedLeft, speedRight);
  
  last_left = left;
  last_right = right;
}

void goBack() {

}

void detectSound() {

}

// Setup
void setup() {
    Serial.begin(9600);

    BoardInit();

    pinMode(greenpin, INPUT);
    pinMode(redpin, INPUT);

    delay(100);

    beep(3);

    verticalPosition = 0;
    horizontalPosition = 1;
}

// Loop
void loop()
{
    if (verticalPosition > 5) {
        goBack();
    }
    else {
        goBack();
    }
}