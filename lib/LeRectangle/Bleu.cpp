#include <Arduino.h>
#include <LibRobus.h>
#include <InfraReceiv.h>

int step;
bool runningBleu;

void bleu(bool init =false){
    if (init){// parametres initiales
        runningBleu = true;
        step = 1; // changer d'étape
    }
    switch (step)
    {
    case 1:
            MOTOR_SetSpeed(1,0);
            MOTOR_SetSpeed(0,0);
        if(wakeUp()){
            step++;
        }
        break;
    case 2:
        runningBleu = false;
        break;
    }
}
bool endBleu(){
    return !runningBleu;
}