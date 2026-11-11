#include <DriveCommands.h>

int step;
bool runningYellow;

void yellow(bool init = false) {
    if (init) { // parametres initiales
        runningYellow = true;
        driveToWall(true); // lancer la premiere commande
        step = 0; // changer d'étape
    }

    switch (step) { // sélectionne la bonne étape
    case 0:
        driveToWall();
        if (endMoving()) {
            tourner(90, 0.2);
            step++;
        }
        break;
    
    case 1: // étape 1
        tourner();
        if (endMoving()) { // si la condition de fin est atteinte
            followWall(true); // lancer la deuxieme commande
            step++; // changer d'étape
        }
        break;
    case 2:
        followWall();
        if (endMoving()) {
            driveDistance(10);
            step++;
        }
        break;

    case 3:
        driveDistance();
        if (endMoving()) {
            tourner(-90);
            step++;
        }
        break;

    case 4:
        tourner();
        if (endMoving()) {
            driveDistance(55);//55
            step++;
        }
        break;
    case 5:
        driveDistance();
        if (endMoving()) {
            tourner(-90);;
            step++;
        }
        break;
    case 6:
        tourner();
        if (endMoving()) {
            driveToLine(true);
            step++;
        }
        break;
    case 7:
        driveToLine();
        if (endMoving()) {
            tourner(90);
            step++;
        }//ajouter un trouver la ligne avec le capteur de ligne case 8
        break;
    case 8:
        tourner();
        if (endMoving()) {
            turnToLine(true);
            step++;
        }//ajouter un trouver la ligne avec le capteur de ligne case 8
        break;
    case 9: // dernière étape
        turnToLine();
        if (endMoving()) { // si la condition de fin est atteinte
            runningYellow = false; // Changer la valeur de fin de la  commande couleur
        }
        break;
    }
}

bool endYellow() {
    return !runningYellow;
}