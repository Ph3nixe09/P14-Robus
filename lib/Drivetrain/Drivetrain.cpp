#include <LibRobus.h>
#include <Arduino.h>

// Global variables
int last_left = 0;
int last_right = 0; 

// Pins
extern const int GREEN_DETECT_PIN = 48;
extern const int RED_DETECT_PIN = 49;
extern const int U1_PIN = 41;  
extern const int U2_PIN = 43; 
extern const int U3_PIN = 45; 
const int U1_Left_PIN = 40;
const int U2_Left_PIN = 42;
const int U3_Left_PIN = 44;
const int U1_Right_PIN = 46;
const int U2_Right_PIN = 47;
const int U3_Right_PIN = 48; 
//U1 = fil vert
//U2 = fil blanc
//U3 = fil gris 
//left captor = tape vert
//mid captor = tape bleu
//right captor = tape jaune

// Timer
long drive_start_time;
long drive_timer_duration;

void startDriveTimer(long duration = 200) { // temps en millis
    drive_start_time = millis();
    drive_timer_duration = duration;
}

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
bool stop() { // MUST TEST
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);

  long delta = millis() - drive_start_time;

  if (delta > drive_timer_duration) {
      return true; // retourne True si le temps est atteint
  } else {
      return false;
  }
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
//Pin 0
int getIRFront() {
  return ROBUS_ReadIR(0);
}
//Pin 1
int getIRRight() {
  return ROBUS_ReadIR(1);
}

bool detectWallFront(int distance = 275) {
    Serial.println(getIRFront());
  return getIRFront() > distance;
}

bool detectWallRight() {
    Serial.println(getIRFront());
  return getIRRight() > 300;
}

// line follower
//les fonctions retournent false si une ligne noir est détecté TRUEEE

//CENTRAL LINE FOLLOWER
bool getLineLeft() {
  return !digitalRead(U1_PIN);
}

bool getLineMid() {
  return !digitalRead(U2_PIN);
}

bool getLineRight() {
  return !digitalRead(U3_PIN);
}
//LEFT LINE FOLLOWER
 bool getLineLeft_L() {
  return !digitalRead(U1_Left_PIN);
 }

bool getLineMid_L() {
  return !digitalRead(U2_Left_PIN);
}

bool getLineRight_L() {
  return !digitalRead(U3_Left_PIN);
}

//RIGHT LINE FOLLOWER
 bool getLineLeft_R() {
  return !digitalRead(U1_Right_PIN);
 }

bool getLineMid_R() {
  return !digitalRead(U2_Right_PIN);
}

bool getLineRight_R() {
  return !digitalRead(U3_Right_PIN);
}

void testLineSensor() {
  Serial.print("Left :");
  Serial.println(getLineLeft());
  Serial.print("Middle :");
  Serial.println(getLineMid());
  Serial.print("Right :");
  Serial.println(getLineRight());

  Serial.print("L Left :");
  Serial.println(getLineLeft_L());
  Serial.print("L Middle :");
  Serial.println(getLineMid_L());
  Serial.print("L Right :");
  Serial.println(getLineRight_L()); 

  Serial.print("R Left :");
  Serial.println(getLineLeft_R());
  Serial.print("R Middle :");
  Serial.println(getLineMid_R());
  Serial.print("R Right :");
  Serial.println(getLineRight_R());
  delay(1000);
}


// Init
void drivetrainInit() {
  pinMode(GREEN_DETECT_PIN, INPUT);
  pinMode(RED_DETECT_PIN, INPUT);
  pinMode(U1_PIN, INPUT);
  pinMode(U2_PIN, INPUT);
  pinMode(U3_PIN, INPUT);
  pinMode(U1_Left_PIN, INPUT);
  pinMode(U2_Left_PIN, INPUT);
  pinMode(U3_Left_PIN, INPUT);
  pinMode(U1_Right_PIN, INPUT);
  pinMode(U2_Right_PIN, INPUT);
  pinMode(U3_Right_PIN, INPUT);
}
