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
  COLOR_COMMAND,
};
int count;
enum RobotState state;
enum Color color;
enum Color current;

bool bumper_safety = false;
bool testing = false;

enum Color colorList[5] = {NONE, NONE, NONE, NONE, NONE};

bool checkList(enum Color color) {
  for (int i = 0; i < 5; i++) {
    if (colorList[i] == color) {
      Serial.println("tu troll");
      return false;
    }
  }
  return true;
}

void endColor(enum Color color) {
  state = FOLLOWING_LINE;
  colorList[count] = color;
  count ++;
}

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
  delay(20); // clock de la boucle
  }
}