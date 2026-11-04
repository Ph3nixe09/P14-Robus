#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <LED.h>
#include <Yellow.h>
#include <Green.h>
#include <Red.h>
#include <ColorDetection.h>
#include <InfraReceiv.h>

enum RobotState {
  WAITING_FOR_IR,
  TEST_COMMAND,
  FOLLOWING_LINE,
  COLOR_COMMAND,
};

enum RobotState state;
enum Color color;

bool bumper_safety = false;
bool testing = true;

// Fonction actionner quand le robot s'allume
void setup() { 
  Serial.begin(9600);
  BoardInit();
  ledInit();
  detectColorInit();
  drivetrainInit();

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
      }

      // if (demarrage()) { // placeholder de la manette
      //   state = FOLLOWING_LINE;
      // } 
      break;

    case TEST_COMMAND:
      state = COLOR_COMMAND;
      color = RED;
      ledOn10(RED);
      red(true);
      break;

    case FOLLOWING_LINE: {
      Color current = colorValues();
      if (current == NONE) {
        followLine();
      } else {
        state = COLOR_COMMAND;
        color = current;
        ledOn10(color);
        switch (color) // INIT des fonctions de couleurs
        {
        case GREEN:
          green(true);
          break;
        case RED:
          green(true);
          break;
        case BLUE:
          stop();
          break;
        case YELLOW:
          yellow(true);
          break;
        
        default:
          break;
        }
      }
      break;
    }

    case COLOR_COMMAND:
      switch (color) {
        case GREEN:
          green();
          if (endGreen()){
            state = FOLLOWING_LINE;
          }
          break;

        case RED:
          red();
          if (endRed()) {
            state = FOLLOWING_LINE;
          } 
          break;

        case BLUE:
          if (wakeUp()){
            state = FOLLOWING_LINE;   
          }       
          break;

        case ORANGE:
          state = FOLLOWING_LINE;          
          break;
        case YELLOW:
          yellow();
          if (endYellow()) {
            state = FOLLOWING_LINE;
          }
          break;
          
    }
  };
  endLed();
  delay(20); // clock de la boucle
}