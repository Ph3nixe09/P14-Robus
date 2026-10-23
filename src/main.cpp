#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <LED.h>
#include <Yellow.h>
#include <ColorDetection.h>

enum RobotState {
  WAITING_FOR_IR,
  FOLLOWING_LINE,
  COLOR_COMMAND,
  END
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

// Fonction actionner quand le robot s'allume
void setup() { 
  Serial.begin(9600);
  BoardInit();
  ledInit();
//   detectColorInit();
  drivetrainInit();

  state = WAITING_FOR_IR;

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
      turnAngleLeft(90);
      state = FOLLOWING_LINE;
    } 
    break;

    case FOLLOWING_LINE:
    turnAngleLeft();
    if (endMoving()) {
      state = WAITING_FOR_IR;
    }
    // if (vois un carton de couleur) {changer l'état à COLOR_COMMAND et à la couleur vue}, 
    /* code */
    break;

    case COLOR_COMMAND:
      switch (color) {
        case GREEN:
          /* code */
          break;
        case RED:
          /* code */
          break;
        case BLUE:
          /* code */
          break;
        case ORANGE:
          /* code */
          break;
        case YELLOW:
          /* code */
          break;
    }
  }
}