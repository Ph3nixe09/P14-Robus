#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <LED.h>
#include <Yellow.h>
#include <BLeu.h>
#include <InfraReceiv.h>
#include <Green.h>
#include <Rose.h>
#include <ColorDetection.h>

int step;
bool runningBlue;

void blue(){
    bool init = false;
    if (!init){// parametres initiales
        stop();
        Serial.print(runningBlue);
        runningBlue = true;
        step = 1; // changer d'étape
        init=true;
    }
    if (wakeUp()){
        runningBlue = false;
    }
}
bool endBlue(){
    return !runningBlue;
}