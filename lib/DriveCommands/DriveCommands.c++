#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>

int goBackListe[5] = {0,0,0,0,0};

// Constants
const float WHEEL_SPACE = 191.5; // mm
const float WHEEL_SPACE_CIRCUMFERENCE = 3.141592 * WHEEL_SPACE; // mm
const float WHEEL_DIAMETER = 3;
const float WHEEL_CIRCUMFERENCE_INCHES = WHEEL_DIAMETER * 3.141592; // inches
const float WHEEL_CIRCUMFERENCE = WHEEL_CIRCUMFERENCE_INCHES * 25.4; // mm
const float PULSES_PER_ROTATION = 3200;

// Global variables
float distance_pulses;
bool moving = false;
float encoder = 0;
const int turning_error = 10;
float arc;
float arc_pulses;


// Commands
// La commande change les vitesses de chaques roues selon les valeurs d'encodeurs pour aller droit
void driveStraight(float speed = 0.5) {
  float kp = 0.00075 * speed;
  float ki = 0.00015;
  
  int left = getEncoderLeft();
  int right = getEncoderRight();
  
  float p = ((right - last_right) - (left - last_left)) * kp;
  float i = (right - left) * ki;

  float speed_left = speed + p + i;
  float speed_right = speed - p - i;

  moveIndependant(speed_left, speed_right);
  
  last_left = left;
  last_right = right;
}

// Touner précisement à droite (horaire) selon un angle
// Angle en degrées
void turnAngleRight(float angle = 0) {
  if (angle != 0) {
    ResetEncoderAll();
    arc = ((angle - 1.4) / 360) * WHEEL_SPACE_CIRCUMFERENCE;
    arc_pulses = (arc * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
    moving = true;
    encoder = 0;
  }

  encoder = -getEncoderRight();
  if (arc_pulses - turning_error < encoder && encoder < arc_pulses + turning_error) {
    stop();
    moving = false;
  } else if (encoder <= arc_pulses - 1000) {
    spinRight(0.3);
  } else if (arc_pulses - 1000 <= encoder && encoder <= arc_pulses + turning_error) {
    spinRight(0.13);
  } else {
    spinLeft(0.13);
  }
}

// Touner précisement à gauche (anti-horaire) selon un angle
// Angle en degrées
void turnAngleLeft(float angle) {
  if (angle != 0) {
    ResetEncoderAll();
    arc = ((angle - 1.4) / 360) * WHEEL_SPACE_CIRCUMFERENCE;
    arc_pulses = (arc * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
    moving = true;
    encoder = 0;
  }

  encoder = getEncoderRight();
  if (arc_pulses - turning_error < encoder && encoder < arc_pulses + turning_error) {
    stop();
    moving = false;
  } else if (encoder <= arc_pulses - 1000) {
    spinLeft(0.3);
  } else if (arc_pulses - 1000 <= encoder && encoder <= arc_pulses + turning_error) {
    spinLeft(0.13);
  } else {
    spinRight(0.13);
  }
}

// Avancer avec PID jusqu'à ce que un des capteurs voit un obstacle
// Retourne le nombre de metres parcourus
int driveToWall() {
  ResetEncoderAll();

  move(0.3);
  delay(200);
  while (detectWall()) {
    driveStraight();
    delay(200);
  }
  move(0.3);
  delay(200);
  stop();

  float encoder = (getEncoderLeft() + getEncoderRight()) / 2;
  float meter_pulses = (1100 * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE; // 1.10m en pulses
  int count = 0;
  while (meter_pulses < encoder) {
    count++;
    encoder = encoder - meter_pulses;
  }
  return count;
}

// Distance en cm seulement en initialisant
void driveDistance(float distance = 0) {
  if (distance != 0) {
    ResetEncoderAll();
    distance_pulses = (distance * 10 * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
    moving = true;
    encoder = 0;
  }
  
  encoder = (getEncoderLeft() + getEncoderRight()) / 2;
    if (distance_pulses - 100 < encoder) {
        stop();
        moving = false;
    } else if (encoder < 200) {
      driveStraight(0.2);
    } else if (encoder < 400) {
      driveStraight(0.4);
    } else if (encoder < distance_pulses - 1500) {
      driveStraight(0.6);
    } else {
      driveStraight(0.3);
    }
}

// 
bool endMoving() {
  // if (!moving) {
  //   delay(100); // délais pour ne pas saccadé le mouvements
  // }
  return !moving;
}

// Suivre une ligne au sol
// void followLine(float speed = 0.5) {
//   if ()
//   moveIndependant(speed, speed);
// }