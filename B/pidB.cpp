#include <LibRobus.h>
#include <Arduino.h>
#include <math.h>

// Global variables
int last_left = 0;
int last_right = 0;

// Pins
const int GREEN_PIN = 48;
const int RED_PIN = 49;

// Constants
const float WHEEL_SPACE = 191.5; // mm
const float WHEEL_SPACE_CIRCUMFERENCE = 3.141592 * WHEEL_SPACE; // mm
const float WHEEL_DIAMETER = 3;
const float WHEEL_CIRCUMFERENCE_INCHES = WHEEL_DIAMETER * 3.141592; // inches
const float WHEEL_CIRCUMFERENCE = WHEEL_CIRCUMFERENCE_INCHES * 25.4; // mm
const float PULSES_PER_ROTATION = 3200;

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

// Bumbers
bool getBumberRear() {
  return ROBUS_IsBumper(3);
}

bool getBumberFront() {
  return ROBUS_IsBumper(2);
}

bool getBumberLeft() {
  return ROBUS_IsBumper(1);
}

bool getBumberRight() {
  return ROBUS_IsBumper(0);
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
int getEncoderLeft() {
  return ENCODER_Read(0);
}

int 
getEncoderRight() {
  return ENCODER_Read(1);
}

int getResetEncoderLeft() {
  return ENCODER_ReadReset(0);
}

int getResetEncoderRight() {
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
  last_right = 0;
  last_left = 0;
}

// objectDetection
bool detectWall() {
  // La fonctions retourne true si le capteur ne détecte pas de murs
  return (digitalRead(GREEN_PIN) == 1 and digitalRead(RED_PIN) == 1);
}

// Commands
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

void driveToWall() {
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
}

// Angle en degrées
void turnAngleRight(float angle) {
  ResetEncoderAll();
  float arc = (angle / 360) * WHEEL_SPACE_CIRCUMFERENCE;
  float arc_pulses = (arc * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
  float encoder = 0;
  int error = 10;
  bool turning = true;
  while (turning) {
    encoder = getEncoderRight();
    if (arc_pulses - error < encoder && encoder < arc_pulses + error) {
      stop();
      turning = false;
    } else if (encoder <= arc_pulses - 500) {
      spinRight(0.3);
    } else if (arc_pulses - 500 <= encoder && encoder <= arc_pulses + error) {
      spinRight(0.13);
    } else {
      spinLeft(0.13);
    }
    delay(20);
  } 
}

// Angle en degrées
void turnAngleLeft(float angle) {
  ResetEncoderAll();
  float arc = (angle / 360) * WHEEL_SPACE_CIRCUMFERENCE;
  float arc_pulses = (arc * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
  float encoder = 0;
  int error = 10;
  bool turning = true;
  while (turning) {
    encoder = getEncoderLeft();
    if (arc_pulses - error < encoder && encoder < arc_pulses + error) {
      stop();
      turning = false;
    } else if (encoder <= arc_pulses - 500) {
      spinLeft(0.3);
    } else if (arc_pulses - 500 <= encoder && encoder <= arc_pulses + error) {
      spinLeft(0.13);
    } else {
      spinRight(0.13);
    }
    delay(20);
  } 
}

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
      driveStraight(0.8);
    } else {
      driveStraight(0.3);
    }
    delay(20);
  }
}

void first() {
  
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
}

// Boucle du robot
void loop() {
  if (getBumberRear()) {
    driveDistance(50);
  } else if (getBumberLeft()) {
    turnAngleLeft(90);
  } else if (getBumberRight()) {
    turnAngleRight(90);
  } else if (getBumberFront()) {
    driveToWall();
  }
}