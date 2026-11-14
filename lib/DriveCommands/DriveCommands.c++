#include <Arduino.h>
#include <LibRobus.h>
#include <Drivetrain.h>
#include <DriveCommands.h>   

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
bool nouveauMouvement = true;
int nbChambre = 0;
const int NB_CHAMBRE_TOTAL = 5;
int coteChambre = 0;   // -1 = gauche, +1 = droite

void changerEtat(Etat e) {
  etat = e;
  nouveauMouvement = true;
}

// Le bout d'une chambre = la ligne disparaît (stable pendant ~150 ms)
bool boutDeChambre() {
  static unsigned long debutPerte = 0;
  if (!getLineLeft() && !getLineMid() && !getLineRight()) {
    if (debutPerte == 0) debutPerte = millis();
    return (millis() - debutPerte) > 150;
  }
  debutPerte = 0;
  return false;
}

void programmeChambre();   // le programme de ton collègue (fait le 180°)

void setup() {
  BoardInit();
  drivetrainInit();
}

void loop() {
  switch (etat) {

    case LIGNE_MILIEU:
      if (nbChambre >= NB_CHAMBRE_TOTAL) {
        changerEtat(DEMI_TOUR_FINAL);
      } else if (getLineLeft() || getLineRight()) {   // voir « à tester »
        coteChambre = getLineLeft() ? -1 : +1;
        changerEtat(AVANCER_INTERSECTION);
      } else {
        followLine(0.15);
      }
      break;

    case AVANCER_INTERSECTION:
      if (nouveauMouvement) { driveDistance(8, 0.2); nouveauMouvement = false; }
      else                  { driveDistance(0, 0.2); }
      if (endMoving()) changerEtat(TOURNER_VERS_CHAMBRE);
      break;

    case TOURNER_VERS_CHAMBRE:
      if (nouveauMouvement) { tourner(coteChambre * 90, 0.25); nouveauMouvement = false; }
      else                  { tourner(0, 0); }
      if (endMoving()) changerEtat(AVANCER_CHAMBRE);
      break;

    case AVANCER_CHAMBRE:
      if (boutDeChambre()) {
        stop();
        changerEtat(PROGRAMME_INTERACTION);
      } else {
        followLine(0.15);
      }
      break;

    case PROGRAMME_INTERACTION:
      programmeChambre();   // bloquant; fait déjà le demi-tour
      changerEtat(RETOUR_LIGNE);
      break;

    case RETOUR_LIGNE:
      // la ligne du milieu est perpendiculaire : les 3 capteurs la voient d'un coup
      if (getLineLeft() && getLineMid() && getLineRight()) {
        changerEtat(AVANCER_RETOUR);
      } else {
        followLine(0.15);
      }
      break;

    case AVANCER_RETOUR:
      if (nouveauMouvement) { driveDistance(8, 0.2); nouveauMouvement = false; }
      else                  { driveDistance(0, 0.2); }
      if (endMoving()) changerEtat(TOURNER_VERS_MILIEU);
      break;

    case TOURNER_VERS_MILIEU:
      if (nouveauMouvement) { tourner(coteChambre * 90, 0.25); nouveauMouvement = false; }
      else                  { tourner(0, 0); }
      if (endMoving()) {
        nbChambre++;
        changerEtat(LIGNE_MILIEU);
      }
      break;

    case DEMI_TOUR_FINAL:
      if (nouveauMouvement) { tourner(180, 0.25); nouveauMouvement = false; }
      else                  { tourner(0, 0); }
      if (endMoving()) changerEtat(RETOUR_DEPART);
      break;

    case RETOUR_DEPART:
      if (nouveauMouvement) { ResetEncoderAll(); nouveauMouvement = false; }
      driveStraight(0.3);   // ignore les branches
      // TODO : condition d'arrêt -> FIN
      break;

    case FIN:
      stop();
      break;
  }
}