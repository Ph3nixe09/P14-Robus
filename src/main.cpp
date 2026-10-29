#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <LED.h>
#include <Yellow.h>
#include <Green.h>
#include <Rose.h>
#include <ColorDetection.h>

enum RobotState {
  WAITING_FOR_IR,
  TEST_COMMAND,
  FOLLOWING_LINE,
  COLOR_COMMAND,
};

enum Color {
  GREEN,
  RED,
  BLUE,
  ORANGE,
  YELLOW
};

enum RobotState state;
enum Color color;

bool bumper_safety;

// Fonction actionner quand le robot s'allume
void setup() { 
  Serial.begin(9600);
  BoardInit();
  ledInit();
//   detectColorInit();
  drivetrainInit();

  state = WAITING_FOR_IR;
  bumper_safety = true;

  beep(3);
  delay(200);
}

// Boucle du robot
void loop() {
  switch (state) {
    case WAITING_FOR_IR:
      /* code */
      // if (recois signal manette) {
      if (getBumberRear()) { // placeholder de la manette
        greenOn10();
        state = FOLLOWING_LINE;
      } 
      break;

    case TEST_COMMAND:
      yellow();

      if (endYellow()) {
        state = WAITING_FOR_IR;
      } else if (bumper_safety && getBumberAny()) {
        if (stop()) {
          state = WAITING_FOR_IR;
        }
      }

    case FOLLOWING_LINE:
      followLine();
      
    // if (vois un carton de couleur) {changer l'état à COLOR_COMMAND et à la couleur vue}, 
    /* code */
    break;

    case COLOR_COMMAND:
      switch (color) {
        case GREEN:
          green();
          break;
        case RED:
          rose();
          break;
        case BLUE:
          /* code */
          break;
        case ORANGE:
          /* code */
          break;
        case YELLOW:
          yellow();

          if (endYellow()) {
            state = WAITING_FOR_IR;
          }
          break;
    }
  }
  endLed();
  delay(20); // clock de la boucle
}