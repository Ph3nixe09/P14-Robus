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
int count = 0;
enum RobotState state;
enum Color color;
enum Color current;
enum Color past;


bool bumper_safety = false;
bool testing = false;

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

  else {
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
        if (current == NONE or current == past) { // continue si il revoit la meme couleur que l'etape precedante
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
        }
        break;
      }
  
      case COLOR_COMMAND:
        switch (color) {
          case GREEN:
            green();
            if (endGreen()){
              state = FOLLOWING_LINE;
              count ++;
              past = GREEN;
            }
            break;
  
          case RED:
            red();
            if (endRed()) {
              state = FOLLOWING_LINE;
              count ++;
              past = RED;
            } 
            break;
  
          case BLUE:
            if (wakeUp()){
              state = FOLLOWING_LINE;
              count ++;  
              past = BLUE; 
            }       
            break;
  
          case ORANGE:
            orange(); 
            if (endOrange()) {
              state = FOLLOWING_LINE;
              count ++;
              past = ORANGE;
            }         
            break;
          case YELLOW:
            yellow();
            if (endYellow()) {
              state = FOLLOWING_LINE;
              count ++;
              past = YELLOW;
            }
            break;
            
      }
    };
    endLed();
    delay(20); // clock de la boucle
  }
}