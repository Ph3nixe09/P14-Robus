#include <DriveCommands.h>
#include <Drivetrain.h>

enum StepRed {
    SEARCH,   //Tourner sur lui même pour trouver la quille 
    CORRECTION, // Tourne pour revenir un peu
    FOUND,   //Avancer jusqu'à la quille
    ACTION,    //Rentre dedans dans ce cas-ci, pour le orange => ramasse
    TURN,   //U-turn après avoir rentrer dedans
    RETURN,   //Revient sur ces pas
    RETURNTWO,
    LINE      //Tourne pour reprendre la ligne
};

enum StepRed stepRed;
float angle;
bool runningRed;
float angle_atteint_red;
float distance_atteint_red;
//int buffer_red;

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
                if (getIRFront() > 400) {
                    //buffer_red ++;
                    angle = distanceAtteint(0) - 7;
                    tourner(-7);
                    stepRed = CORRECTION;

                /*if (buffer_red == 15){
                    angle = distanceAtteint(0) - 7;
                    tourner(-7);
                    stepRed = CORRECTION;
                }*/
                }
                //if (endMoving){
                    
                //}
                break;

            case CORRECTION:
                tourner();
                if (endMoving()) {
                    stepRed = FOUND;
                    driveDistance(70,0.2);
                }
                break;
            
            case FOUND:
                driveDistance(0,0.2);
                if (getIRFront() > 600) {
                    stop();
                    distance_atteint_red = distanceAtteint(1) + 15;
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
                driveDistance(distance_atteint_red, 0.2);   //Revient driveDistance(distance_atteint_red + 10, 0.2);
                stepRed = RETURNTWO;
                break;
                
            case RETURNTWO:
                driveDistance(0,0.2);     //Update mouvement
                if (endMoving()){
                    stepRed = LINE;
                    if (angle > 180){
                        tourner(-angle + 180);
                    }
                    else{
                        tourner(175 - angle);   //Replace sur ligne
                    }
                    break;
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
