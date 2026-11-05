#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <bras.h>

//CRÉER FONCTION DÉTECTEUR IR / UPDATE LES FCT DÉPLACEMENT

//Code pour le défi Orange
float angle_atteint;
float distance_atteint;
bool terminer_defi = false;
uint16_t infrarouge = 0;
int buffer = 0;   //Pour s'assurer que ce n'est pas une donnée abérante
unsigned long tempsDebut = 0;

enum StepOrange{
    CHERCHE,   //Tourner sur lui même pour trouver la quille 
    CORRECTION,
    TROUVER,   //Avancer jusqu'à la quille
    ACTION,
    PRISE,    //Rentre dedans dans ce cas-ci, pour le orange => ramasse
    AVANCER,   //Revient sur ces pas
    LIGNE      //Tourne pour reprendre la ligne
};

enum StepOrange stepOrange;
float angle_orange;
bool runningOrange;

void orange(bool init = false) {
    if (init) {
        runningOrange = true;
        stepOrange = CHERCHE;
        tourner(360, 0.1);
        ResetEncoderAll();
    }

    if (runningOrange) {
        switch (stepOrange) {
            case CHERCHE:
                tourner();
                if (getIRFront() > 300) {
                    angle_orange = distanceAtteint(0) - 7;
                    tourner(-9);
                    stepOrange = CORRECTION;
                }
                break;

            case CORRECTION:
                tourner();
                if (endMoving()) {
                    stepOrange = TROUVER;
                }
                break;
            
            case TROUVER:
                driveStraight(0.1);
                if (getIRFront() > 290) {
                    stop();
                    stepOrange = ACTION;
                    tourner(160);
                }
                break;

            case ACTION:                //Pourrait ajouter un if pour le orange sans faire nouveau code complet
              tourner();                     // update du mouvement
              if (endMoving()) {
                tempsDebut = millis();     // le chrono démarre APRÈS le tour
                stepOrange = PRISE;
              }
              break;
      
          case PRISE:
            if (millis() - tempsDebut < 600) {
              ouvrirPince();
              baisserBras();
            }
            else if (millis() - tempsDebut < 1000) {
              fermerPince();
            }
            else if (millis() - tempsDebut < 1400) {
              leverBras();
            }
            else {
              driveToLine(true);         // lance le retour
              stepOrange = AVANCER;
            }
            break;
           
          
    
          case AVANCER:
            driveToLine();
            if (endMoving()) {
              tourner(180 - angle_orange);
                stepOrange = LIGNE;
          }
          break;
    
            case LIGNE:
                tourner();
                if(endMoving()) {
                    runningOrange = false;
                }
                break;
        }
    }
}


bool endOrange() {
    return !runningOrange;
};
