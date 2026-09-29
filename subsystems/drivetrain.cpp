#include <LibRobus.h>

void stop(){
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
};

void move(int speed, bool backwards = false) {
  if (backwards) {
    speed*=-1;
  };
  MOTOR_SetSpeed(RIGHT,speed);
  MOTOR_SetSpeed(LEFT, speed);
};

void spinRight(int speed) {
  MOTOR_SetSpeed(RIGHT, speed);
  MOTOR_SetSpeed(LEFT, -speed);
};

void spinLeft(int speed) {
  MOTOR_SetSpeed(RIGHT, -speed);
  MOTOR_SetSpeed(LEFT, speed);
};

void moveIndependant(int speed_right, int speed_left) {
  MOTOR_SetSpeed(RIGHT, speed_right);
  MOTOR_SetSpeed(LEFT, speed_left);
}