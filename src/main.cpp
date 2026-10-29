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

enum RobotState state;
enum Color color;

bool bumper_safety;

// Fonction actionner quand le robot s'allume
void setup() { 
  Serial.begin(9600);
  BoardInit();
  ledInit();
  detectColorInit();
  drivetrainInit();

  state = WAITING_FOR_IR;
  bumper_safety = false;

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
        state = FOLLOWING_LINE;
      } 

      if (getBumberFront()) { // placeholder de la manette
        ledOn10(GREEN);
      } 
      if (getBumberLeft()) { // placeholder de la manette
        ledOn10(RED);
      } 

      if (getBumberRight()) { // placeholder de la manette
        ledOn10(BLUE);
      } 
      break;

    case TEST_COMMAND:
      colorValues();
      break;
      //if (bumper_safety && getBumberAny()) {
        
      //}

    case FOLLOWING_LINE: {
      Color current = colorValues();
      if (current == NONE) {
        Serial.println("PLANCHER");
        followLine();
      } else {
        state = COLOR_COMMAND;
        color = current;
      }
      break;
    }

    case COLOR_COMMAND:
      switch (color) {
        case GREEN:
          ledOn10(color);
          state = FOLLOWING_LINE;          
          break;
        case RED:
          ledOn10(color);
          state = FOLLOWING_LINE;          
          break;
        case BLUE:
          ledOn10(color);
          state = FOLLOWING_LINE;          
          break;
        case ORANGE:
          ledOn10(color);
          state = FOLLOWING_LINE;          
          break;
        case YELLOW:
          ledOn10(color);
          state = FOLLOWING_LINE;          

          // yellow();

          // if (endYellow()) {
          //   state = WAITING_FOR_IR;
          // }
          break;
          
    }
  };
  endLed();
  delay(20); // clock de la boucle
}