#include <DriveCommands.h>
#include <Drivetrain.h>

enum StepRed {
    SEARCH,   //Tourner sur lui même pour trouver la quille 
    CORRECTION, // Tourne pour revenir un peu
    FOUND,   //Avancer jusqu'à la quille
    ACTION,    //Rentre dedans dans ce cas-ci, pour le orange => ramasse
    TURN,   //U-turn après avoir rentrer dedans
    RETURN,   //Revient sur ces pas
    LINE      //Tourne pour reprendre la ligne
};

enum StepRed stepRed;
float angle;
bool runningRed;

void red(bool init = false) {
    if (init) {
        runningRed = true;
        stepRed = SEARCH;
        tourner(360, 0.1);
        ResetEncoderAll();
    }

    if (runningRed) {
        switch (stepRed) {
            case SEARCH:
                tourner();
                if (getIRFront() > 420) {
                    angle = distanceAtteint(0) - 7;
                    tourner(-7);
                    stepRed = CORRECTION;
                }
                break;

            case CORRECTION:
                tourner();
                if (endMoving()) {
                    stepRed = FOUND;
                }
                break;
            
            case FOUND:
                driveStraight(0.3);
                if (getIRFront() > 600) {
                    stop();
                    driveDistance(15, 0.8);
                    stepRed = ACTION;
                }
                break;
            
            case ACTION:
                driveDistance();
                if (endMoving()) {
                    tourner(180);
                    stepRed = TURN;
                }
                break;
    
            case TURN:
                tourner();
                if (endMoving()) {
                    driveToLine(true);
                    stepRed = RETURN;
                }
                break;
            
            case RETURN:
                driveToLine();
                if (endMoving()) {
                    tourner(180 - angle);
                    stepRed = LINE;
                }
                break;
    
            case LINE:
                tourner();
                if(endMoving()) {
                    runningRed = false;
                }
                break;
        }
    }
}


bool endRed() {
    return !runningRed;
}
