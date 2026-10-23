#include <DriveCommands.h>

int step;
bool runningYellow;

void yellow(bool init = false) {
    if (init) {
        runningYellow = true;
        turnAngleRight(90);
        step = 1;
    }

    switch (step) {
    case 1:
        turnAngleRight();
        if (endMoving()) {
            driveDistance(50);
            step++;
        }
        break;
    case 2:
        driveDistance();
        if (endMoving()) {
            turnAngleLeft(90);;
            step++;
        }
        break;
    case 3:
        turnAngleLeft();
        if (endMoving()) {
            driveDistance(25);
            step++;
        }
        break;
    case 4:
        driveDistance();
        if (endMoving()) {
            turnAngleLeft(90);;
            step++;
        }
        break;
    case 5:
        turnAngleLeft();
        if (endMoving()) {
            driveDistance(50);
            step++;
        }
        break;
    case 6:
        driveDistance();
        if (endMoving()) {
            turnAngleRight(90);;
            step++;
        }
        break;
    case 7:
        turnAngleRight();
        if (endMoving()) {
            runningYellow = false;
        }
        break;
    }
}

bool endYellow() {
    return !runningYellow;
}