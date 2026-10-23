#include <LibRobus.h>
#include <Arduino.h>

// Global variables
int last_left = 0;
int last_right = 0; 

// Pins
extern const int GREEN_DETECT_PIN = 48;
extern const int RED_DETECT_PIN = 49;
extern const int U1_PIN = 37; // fil Jaune
extern const int U2_PIN = 39; // fil Vert
extern const int U3_PIN = 41; // fil Brun

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
  return ROBUS_IsBumper(0);
}

bool getBumberRight() {
  return ROBUS_IsBumper(1);
}

bool getBumberAny() {
  return getBumberRear() || getBumberFront() || getBumberLeft() || getBumberRight();
}


// Motors
void stop(){
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  delay(200);
};

// Meme vitesse pour les deux moteurs
void move(float speed, bool backwards = false) { // Ajouter True si en arrière
  if (backwards) {
    speed*=-1;
  };
  MOTOR_SetSpeed(RIGHT, speed);
  MOTOR_SetSpeed(LEFT, speed);
};

void spinRight(float speed) {
  MOTOR_SetSpeed(RIGHT, -speed);
  MOTOR_SetSpeed(LEFT, speed);
};

void spinLeft(float speed) {
  MOTOR_SetSpeed(RIGHT, speed);
  MOTOR_SetSpeed(LEFT, -speed);
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
  return (digitalRead(GREEN_DETECT_PIN) == 1 and digitalRead(RED_DETECT_PIN) == 1);
}

// line follower
bool getLineLeft() {
  return !digitalRead(U1_PIN);
}

bool getLineMid() {
  return !digitalRead(U2_PIN);
}

bool getLineRight() {
  return !digitalRead(U3_PIN);
}

// Init
void drivetrainInit() {
  pinMode(GREEN_DETECT_PIN, INPUT);
  pinMode(RED_DETECT_PIN, INPUT);
  pinMode(U1_PIN, INPUT);
  pinMode(U2_PIN, INPUT);
  pinMode(U3_PIN, INPUT);
}