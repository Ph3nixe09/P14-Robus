#include <DriveCommands.h>

int step;
bool runningYellow;

void yellow(bool init = false) {
    if (init) { // parametres initiales
        runningYellow = true;
        tourner(90); // lancer la premiere commande
        step = 1; // changer d'étape
    }

    switch (step) { // sélectionne la bonne étape
    case 1: // étape 1
        tourner();
        if (endMoving()) { // si la condition de fin est atteinte
            driveDistance(50); // lancer la deuxieme commande
            step++; // changer d'étape
        }
        break;
    case 2:
        driveDistance();
        if (endMoving()) {
            tourner(-90);;
            step++;
        }
        break;
    case 3:
        tourner();
        if (endMoving()) {
            driveDistance(25);
            step++;
        }
        break;
    case 4:
        driveDistance();
        if (endMoving()) {
            tourner(-90);;
            step++;
        }
        break;
    case 5:
        tourner();
        if (endMoving()) {
            driveDistance(50);
            step++;
        }
        break;
    case 6:
        driveDistance();
        if (endMoving()) {
            tourner(90);;
            step++;
        }
        break;
    case 7: // dernière étape
        tourner();
        if (endMoving()) { // si la condition de fin est atteinte
            runningYellow = false; // Changer la valeur de fin de la  commande couleur
        }
        break;
    }
}

bool endYellow() {
    return !runningYellow;
}