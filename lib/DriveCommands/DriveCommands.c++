#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>

int goBackListe[5] = {0,0,0,0,0};

// Constants
const float WHEEL_SPACE = 190.5; // mm
const float WHEEL_SPACE_CIRCUMFERENCE = 3.141592 * WHEEL_SPACE; // mm
const float WHEEL_DIAMETER = 3;
const float WHEEL_CIRCUMFERENCE_INCHES = WHEEL_DIAMETER * 3.141592; // inches
const float WHEEL_CIRCUMFERENCE = WHEEL_CIRCUMFERENCE_INCHES * 25.4; // mm
const float PULSES_PER_ROTATION = 3200;

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

  Serial.print("P: ");
  Serial.println(p);
  Serial.print("I: ");
  Serial.println(i);

  moveIndependant(speed_left, speed_right);
  
  last_left = left;
  last_right = right;
}

// Touner précisement à droite (horaire) selon un angle
// Angle en degrées
void turnAngleRight(float angle) {
  ResetEncoderAll();
  float arc = ((angle - 1.4) / 360) * WHEEL_SPACE_CIRCUMFERENCE;
  float arc_pulses = (arc * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
  float encoder = 0;
  int error = 10;
  bool turning = true;
  while (turning) {
    encoder = -getEncoderRight();
    Serial.println(encoder);
    if (arc_pulses - error < encoder && encoder < arc_pulses + error) {
      stop();
      turning = false;
    } else if (encoder <= arc_pulses - 1000) {
      spinRight(0.3);
    } else if (arc_pulses - 1000 <= encoder && encoder <= arc_pulses + error) {
      spinRight(0.13);
    } else {
      spinLeft(0.13);
    }
    delay(20);
  } 
  delay(200);
}

// Touner précisement à gauche (anti-horaire) selon un angle
// Angle en degrées
void turnAngleLeft(float angle) {
  ResetEncoderAll();
  float arc = ((angle - 0.4) / 360) * WHEEL_SPACE_CIRCUMFERENCE;
  float arc_pulses = (arc * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
  float encoder = 0;
  int error = 10;
  bool turning = true;
  while (turning) {
    encoder = getEncoderRight();
    Serial.println(encoder);
    if (arc_pulses - error < encoder && encoder < arc_pulses + error) {
      stop();
      turning = false;
    } else if (encoder <= arc_pulses - 1000) {
      spinLeft(0.3);
    } else if (arc_pulses - 1000 <= encoder && encoder <= arc_pulses + error) {
      spinLeft(0.13);
    } else {
      spinRight(0.13);
    }
    delay(20);
  } 
  delay(200);
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

// Avancer avec PID d'une distance fixe
// Distance en cm
void driveDistance(float distance) {
  ResetEncoderAll();
  
  float distance_pulses = (distance * 10 * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
  bool moving = true;
  float encoder = 0;
  driveStraight(0.2);
  delay(200);
  driveStraight(0.3);
  delay(200);
  driveStraight(0.4);
  delay(200);
  while (moving) {
    encoder = (getEncoderLeft() + getEncoderRight()) / 2;
    if (distance_pulses - 100 < encoder) {
        stop();
        moving = false;
    } else if (encoder < distance_pulses - 1500) {
      driveStraight(0.6);
    } else {
      driveStraight(0.3);
    }
    delay(20);
  }
}