#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <LED.h>
#include <InfraReceiv.h>

enum RobotState {
  WAITING_FOR_IR,
  TEST_COMMAND,
  FOLLOWING_LINE,
};

int count;
enum RobotState state;

bool ir_safety = true;
int buffer = 0;
bool bumper_safety = false;  //True sur le final
bool testing = true;

// Fonction actionner quand le robot s'allume
void setup() { 
  Serial.begin(9600);
  BoardInit();
  ledInit();
  drivetrainInit();
  count = 0;

  state = WAITING_FOR_IR;
  beep(3);
  delay(200);  
}

// Boucle du robot
void loop() {
  //Safety bumper et infrarouge pour la détection d'obstacle
  if (ir_safety){
    if (getIRFront() > 300){
      buffer ++;
      if (buffer >= 3){
        stop();
        state = WAITING_FOR_IR;
        bumper_safety = false;  //A enlever sur le final
      }
    }
    else{
      buffer = 0;
    }
  }
  if (bumper_safety){
    if (getBumberAny()){
      stop();
      state = WAITING_FOR_IR;
      bumper_safety = false;  //Pour pas arreter direct (a enlever sur le final)
      delay(200);  //Pour pas activer le test direct (a enlever sur le final)
    }
  }

  switch (state) {
    case WAITING_FOR_IR:
      if (getBumberRear() && testing) { // POUR TESTER
        state = TEST_COMMAND;
        delay(500);  //Pour pas activer le bumper_safety (a enlever sur le final)
        bumper_safety = true;  //Pour pas activer le bumper_safety (a enlever sur le final)
        driveDistance(100);
      }
      break;
    
    case TEST_COMMAND:
      driveDistance();
      if (endMoving()){
        state = WAITING_FOR_IR;
      }
      break;
  }
  delay(20); // clock de la boucle
}