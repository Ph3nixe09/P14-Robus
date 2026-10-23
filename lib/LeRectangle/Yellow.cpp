#include <DriveCommands.h>

void yellow() {
    turnAngleRight(90);
    driveDistance(50);
    turnAngleLeft(90);
    driveDistance(25);
    turnAngleLeft(90);
    driveDistance(50);
    turnAngleRight(90);
}