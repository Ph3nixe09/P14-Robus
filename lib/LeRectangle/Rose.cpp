#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>

//CRÉER FONCTION DÉTECTEUR IR / UPDATE LES FCT DÉPLACEMENT

//Code pour le défi Rose
float angle_atteint;
float distance_atteint;
bool terminer_defi = false;
uint16_t infrarouge = 0;
int buffer = 0;   //Pour s'assurer que ce n'est pas une donnée abérante

enum RoseState {
    CHERCHE,   //Tourner sur lui même pour trouver la quille 
    TROUVER,   //Avancer jusqu'à la quille
    ACTION,    //Rentre dedans dans ce cas-ci, pour le orange => ramasse
    TOURNER,   //U-turn après avoir rentrer dedans
    AVANCER,   //Revient sur ces pas
    LIGNE      //Tourne pour reprendre la ligne
};

enum Deplacement {
  START,
  FIN
};

enum RoseState etape;
enum Deplacement deplacement;

void rose(){
  infrarouge = ROBUS_ReadIR(0);   //Lecture infrarouge à chaque fois
  switch (etape){
      //Recherche de la quille en faisant un 360
      case CHERCHE:
        switch (deplacement){
          case START:
            tourner(360, 0.1);
            deplacement = FIN;
            Serial.println("tourne");
            break;

          case FIN:
            tourner(0,0.1);     //Update mouvement
            Serial.println("Update mouvement");
            if (endMoving()){   //Si on a rien trouver, ce n'est pas possible, on recommence
              stop();
              deplacement = START;
              etape = CHERCHE;
              Serial.println("tourne_fin");
            }
        }
        
        if (infrarouge > 150){    //Quand on détecte quelque chose d'assez proche
          //Valeur d'econdeur pour savoir combien tourner à ajouter *************************************************************
          stop();
          deplacement = START;
          etape = TROUVER;
          Serial.println("Angle atteint: ");
          angle_atteint = distanceAtteint(0);
          Serial.print(angle_atteint);
        }
      break;
    
    case TROUVER:
      Serial.println("Fini");
      switch (deplacement){
        case START:
          driveDistance(70, 0.2);
          deplacement = FIN;
          break;

        case FIN:
          driveDistance(0,0.2);
          if (infrarouge > 600){    //Quand on détecte quelque chose d'assez proche
     //On vérifie si ce n'est pas abérant
              //Valeur d'econdeur pour savoir combien tourner à ajouter *************************************************************
              stop();
              deplacement = START;
              etape = ACTION;
              buffer = 0;
              Serial.println("Action mtn");
              distance_atteint = distanceAtteint(1);
              Serial.print(distance_atteint);
            buffer ++;
            
          }
          else if (endMoving()){
            stop();
            distance_atteint = distanceAtteint(1);
            etape = ACTION;
            deplacement = START;
          }
          break;
      }
      break;

    case ACTION:                //Pourrait ajouter un if pour le orange sans faire nouveau code complet
      switch (deplacement){
        case START:
          driveDistance(10);   //Rentre dans la quille
          deplacement = FIN;
          break;

        case FIN:
          driveDistance();     //Update mouvement
          if (endMoving()){
            stop();
            deplacement = START;
            etape = TOURNER;
          }
          break;
      }
      break;
    
    case TOURNER:
      switch (deplacement){
        case START:
          tourner(180);   //U-turn
          deplacement = FIN;
          break;

        case FIN:
          tourner();     //Update mouvement
          if (endMoving()){
            stop();
            deplacement = START;
            etape = AVANCER;
          }
          break;
      }
      break;
    
    case AVANCER:
      switch (deplacement){
        case START:
          //CALCUL DE LA DISTANCE AVEC LA FONCTION DE RETOUR DU NBR DE PULSE
          driveDistance(distance_atteint + 10, 0.2);   //Revient
          deplacement = FIN;
          break;

        case FIN:
          driveDistance(0,0.2);     //Update mouvement
          if (endMoving()){
            stop();
            deplacement = START;
            etape = LIGNE;
          }
          break;
      }
      break;

    case LIGNE:
      switch (deplacement){
        case START:
          //CALCUL DE L'ANGLE AVEC LA FONCTION DE RETOUR DU NBR DE PULSE
          if (angle_atteint > 180){
            tourner(-angle_atteint + 180);
          }
          else{
            tourner(175 - angle_atteint);   //Replace sur ligne
          }
          deplacement = FIN;
          break;

        case FIN:
          tourner();     //Update mouvement
          if (endMoving()){
            stop();
            deplacement = START;
            terminer_defi = true;
          }
          break;
      }
      break;
  }
}

bool endRose(){
  return terminer_defi;
}

//=========================================================
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
  //infrarouge = ROBUS_ReadIR(0);
    if (getBumberRear()){ // placeholder de la manette
      state = AFFICHE;
      etape = CHERCHE;
      deplacement = START;
    }
    if (getBumberFront()) {
        state = ATTEND;
        stop();
    }

    if (state == AFFICHE){
        Serial.println(infrarouge);
        rose();
    }
    if (endRose()){
      stop();
      state = ATTEND;
    }
    delay(20);
}