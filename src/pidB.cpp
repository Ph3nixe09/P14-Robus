#include <LibRobus.h>
#include <Arduino.h>


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

void driveStraight() {
  float kp = 0.0005;
  float speed = 0.5;
  float p = (getResetEncoderLeft() - getResetEncoderRight()) * kp;
  Serial.print(p);
  Serial.print("\n");
  float speedLeft = speed - p;
  float speedRight = speed + p;
  moveIndependant(speedLeft, speedRight);
}

void testDriveStraight() {
  for (int i = 0; i < 25; i++) {
    delay(200);
    driveStraight();
  }
  stop();
}

// Fonction actionner quand le robot s'allume
void setup() {
  Serial.begin(9600);
  BoardInit();

  delay(100);
  beep(3);
  delay(200);
  testDriveStraight();
}

// Boucle du robot
void loop() {

}