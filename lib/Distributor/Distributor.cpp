#include <LibRobus.h>
#include <Arduino.h>


const uint8_t CENTER = 0;
const uint8_t CYLINDER_1 = 110;
const uint8_t CYLINDER_2 = -120;

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
