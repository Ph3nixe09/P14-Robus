#include <Arduino.h>
#include <LibRobus.h>
#include <Drivetrain.h>
#include <DriveCommands.h>   // mets le vrai nom de ton fichier

enum Etat {
  LIGNE_MILIEU,
  AVANCER_INTERSECTION,
  TOURNER_VERS_CHAMBRE,
  AVANCER_CHAMBRE,
  PROGRAMME_INTERACTION,
  RETOUR_LIGNE,
  AVANCER_RETOUR,
  TOURNER_VERS_MILIEU,
  DEMI_TOUR_FINAL,
  RETOUR_DEPART,
  FIN
};

Etat etat = LIGNE_MILIEU;

int nbChambre = 0;
int coteChambre = 0;   // -1 = gauche, +1 = droite

void programmeChambre();   

void setup() {
  BoardInit();
  drivetrainInit();
}

void loop() {
  switch (etat) {

    case LIGNE_MILIEU:
      if (nbChambre >= 5) {
        tourner(180, 0.1);
        etat = DEMI_TOUR_FINAL;
      }
      else if (getLineLeft() || getLineRight()) {
        if (getLineLeft()) {
          coteChambre = -1;
        } else {
          coteChambre = 1;
        }
        driveDistance(8, 0.2);   // on lance l'avance jusqu'au centre de l'intersection
        etat = AVANCER_INTERSECTION;
      }
      else {
        followLine(0.15);
      }
      break;

    case AVANCER_INTERSECTION:
      driveDistance(0, 0.2);   // on continue l'avance
      if (endMoving()) {
        tourner(coteChambre * 90, 0.25);   // on lance le virage
        etat = TOURNER_VERS_CHAMBRE;
      }
      break;

    case TOURNER_VERS_CHAMBRE:
      tourner(0, 0);   // on continue le virage
      if (endMoving()) {
        etat = AVANCER_CHAMBRE;
      }
      break;

    case AVANCER_CHAMBRE:
      // Le bout de la chambre : les 3 capteurs ne voient plus la ligne
      if (!getLineLeft() && !getLineMid() && !getLineRight()) {
        stop();
        tourner(180, 0.25);
        etat = PROGRAMME_INTERACTION;
      } else {
        followLine(0.15);
      }
      break;
//Il faut changer du code quand le fichier interaction serait fonctionnel, enlever tourner(180, 25), car il tournera dans l'autre code
    case PROGRAMME_INTERACTION:
     // programmeChambre(); //
      if (endMoving())  {
      etat = RETOUR_LIGNE;
      } 
      break;

    case RETOUR_LIGNE:
      // Les 3 capteurs voient la ligne du milieu en même temps
      if (getLineLeft() && getLineMid() && getLineRight()) {
        driveDistance(8, 0.2);
        etat = AVANCER_RETOUR;
      } else {
        followLine(0.15);
      }
      break;

    case AVANCER_RETOUR:
      driveDistance(0, 0.2);
      if (endMoving()) {
        tourner(coteChambre * 90, 0.25);
        etat = TOURNER_VERS_MILIEU;
      }
      break;

    case TOURNER_VERS_MILIEU:
      tourner(0, 0);
      if (endMoving()) {
        nbChambre = nbChambre + 1;
        etat = LIGNE_MILIEU;
      }
      break;

    case DEMI_TOUR_FINAL:
      tourner(0, 0);
      if (endMoving()) {
        etat = RETOUR_DEPART;
      }
      break;

    case RETOUR_DEPART:
       if (!getLineLeft() && !getLineMid() && !getLineRight()) {
        stop();
        etat = FIN;
      } else {
        followLine(0.15);
      }
      break;
  

    case FIN:
      stop();
      break;
  }
}