#include <DriveCommands.h>
#include <LibRobus.h>

int steps;
bool runningGreen;

void green(bool init = false) {
    if (init) { // parametres initiales
        runningGreen = true;
        driveDistance(75); // lancer la premiere commande
        steps ++;
        Serial.println("init sequence");
    }
    switch (steps) { // sélectionne la bonne étape
    case 1: // étape 1
        driveDistance();
        if (endMoving()) { // si la condition de fin est atteinte
            runningGreen = false;
            steps++; // changer d'étape
            Serial.println("end moving");
        }
        break;
}
}

bool endGreen() {
    return !runningGreen;
}
