#include <LibRobus.h>
#include <Arduino.h>


const uint8_t CENTER = 0;
const uint8_t CYLINDER_1 = 110;
const uint8_t CYLINDER_2 = -110;

bool distributor_running = false;

long dist_start_time;
long dist_timer_duration;
int step = 0;

// Fonction du bras qui vont être appeler
void distributorPill1() {
    SERVO_SetAngle(0, CYLINDER_1);
}

void distributorPill2() {
    SERVO_SetAngle(0, CYLINDER_2);
}

void distributorCenter() {
    SERVO_SetAngle(0, CENTER);
}

void servoInit() {
    SERVO_Enable(0); 
    distributorCenter();

}
    
void servoOff() {
    distributorCenter();
    SERVO_Disable(0); // Distributeur
}

void distribute(int cylinder = 0) { // 1 gauche, 2 droit, seulement pour initialiser
    if (cylinder != 0) {
        distributor_running = true;
        dist_start_time = millis();
        if (cylinder == 1) {
            distributorPill1();
        } else {
            distributorPill2();
        } 
    }

    else {
        if ((millis() - dist_start_time) >= 1000) {
            distributor_running = false;
            distributorCenter();
        }
    }
}

bool endDistributor() {
    return !distributor_running;
}