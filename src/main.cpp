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

void moveIndependant(float speed_right, int speed_left) {
  MOTOR_SetSpeed(RIGHT, speed_right);
  MOTOR_SetSpeed(LEFT, speed_left);
}

// Encoders

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

// Fonction actionner quand le robot s'allume
void setup() {
  Serial.begin(9600);
  BoardInit();

  delay(100);
  beep(3);
  delay(200);
  dance();
}

// Boucle du robot
void loop() {

}