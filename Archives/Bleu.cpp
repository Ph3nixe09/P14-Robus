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
    if (wakeUp()){
        runningBlue = false;
    }
}
bool endBlue(){
    return !runningBlue;
}