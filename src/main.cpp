#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <LED.h>
#include <InfraReceiv.h>
#include <rfid.h>



enum RobotState {
  WAITING_FOR_IR,
  TEST_COMMAND,
  FOLLOWING_LINE,
};

int count;
enum RobotState state;

bool bumper_safety = false;
bool testing = false;

// Fonction actionner quand le robot s'allume
void setup() { 
  Serial.begin(9600);
  BoardInit();
  ledInit();
  drivetrainInit();
  rfidInit();
  count = 0;

  state = WAITING_FOR_IR;
  beep(3);
  delay(200);  
}

// Boucle du robot
void loop() {
  ReadRFID();

  //ReadRFID();



  /*
  switch (state) {
    case WAITING_FOR_IR:
      if (getBumberRear() && testing) { // POUR TESTER
        state = TEST_COMMAND;
        // command();
      }
      break;

    // case TEST_COMMAND:
    //   if (endCommand()) {
    //     state = WAITING_FOR_IR;
    //   }     
    //   break;
  }
  delay(20); // clock de la boucle

  */
}