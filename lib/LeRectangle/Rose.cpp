#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>

//Code pour le défi Rose

bool terminer_defi = false;
uint16_t infrarouge = 0;

enum RoseState {
    CHERCHE,
    TROUVER,
    REVIENT
};

enum Deplacement {
  START,
  FIN
};

enum RoseState etape;
enum Deplacement deplacement;

void rose(){
  infrarouge = ROBUS_ReadIR(0);
  switch (etape){
      case CHERCHE:
        switch (deplacement){
          case START:
            turnAngleRight(360);
            deplacement = FIN;
            Serial.println("tourne");

          case FIN:
          turnAngleRight();
            if (endMoving()){
              stop();
              deplacement = START;
              etape = TROUVER;
              Serial.println("tourne_fin");
            }
        }
        
        if (infrarouge > 150){
          stop();
          deplacement = START;
          etape = TROUVER;
        }
      break;
    
    case TROUVER:
      Serial.println("Fini");
      switch (deplacement){
        case START:
          break;

        case FIN:
          break;
      }
      break;

    case REVIENT:
      switch (deplacement){
        case START:
          break;

        case FIN:
          break;
      }
      break;
  }
}

bool endRose(){
  return terminer_defi;
}

//Code pour faire les test

enum RobotState {
    ATTEND,
    AFFICHE
};

enum RobotState state;

void setup() { 
  Serial.begin(9600);
  BoardInit();
//   detectColorInit();
  drivetrainInit();

  etape = CHERCHE;
  deplacement = START;

  beep(3);
  delay(200);
};

// Boucle du robot
void loop() {
    if (getBumberRear()) { // placeholder de la manette
      state = AFFICHE;
    }
    if (getBumberFront()) {
        state = ATTEND;
    }

    if (state == AFFICHE){
        Serial.println(infrarouge);
        rose();
    }
    delay(20);
}