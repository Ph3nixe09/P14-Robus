#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <LED.h>
#include <Yellow.h>
//#include <Bleu.h>
#include <InfraReceiv.h>
#include <Green.h>
#include <orange.h>
#include <Red.h>
#include <ColorDetection.h>
#include <InfraReceiv.h>
#include <bras.h>

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
  detectColorInit();
  drivetrainInit();
  count = 0;

  state = WAITING_FOR_IR;
  beep(3);
  delay(200);  
}

// Boucle du robot
void loop() {
  // demarrage();
  // wakeUp();

  if (count == 5){
    state = WAITING_FOR_IR;
    stop();
    baisserBras();
    delay(600);
    ouvrirPince();
    sapinNoel();
  } 

  switch (state) {
    case WAITING_FOR_IR:
      if (getBumberRear() && testing) { // POUR TESTER
        state = TEST_COMMAND;
        orange(true);
      }

      if (demarrage() && !testing) {
        state = FOLLOWING_LINE; //réajustement des valeurs de couleur
        //state = FOLLOWING_LINE;
      } 
      break;

    case TEST_COMMAND:
      orange();
      if (endOrange()) {
        state = WAITING_FOR_IR;
      }     
      break;

    case FOLLOWING_LINE: {
      current = colorValues();
      if (current != NONE && checkList(current)) { // continue si il revoit la meme couleur que l'etape precedante
        state = COLOR_COMMAND;
        color = current;
        ledOn10(color);
        switch (color) // INIT des fonctions de couleurs
        {
        case GREEN:
          green(true);
          break;
        case RED:
          red(true);
          break;
        case BLUE:
          stop();
          break;
        case YELLOW:
          yellow(true);
          break;
        case ORANGE:
          orange(true);
          break;
        default:
          break;
        }
      } else {
        followLine();
      }
      
      break;
    }

    case COLOR_COMMAND:
      switch (color) {
        case GREEN:
          green();
          if (endGreen()){
            endColor(color);
          }
          break;

        case RED:
          red();
          if (endRed()) {
            endColor(color);
          } 
          break;

        case BLUE:
          if (wakeUp()){
            endColor(color);
          }       
          break;

        case ORANGE:
          orange(); 
          if (endOrange()) {
            endColor(color);
          }         
          break;
        case YELLOW:
          yellow();
          if (endYellow()) {
            endColor(color);
          }
          break;

        case NONE:
          break;
    }
  };
  endLed();
  delay(20); // clock de la boucle
}