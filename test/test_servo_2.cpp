#include <Arduino.h>
#include <LibRobus.h>

void setup()
{

    Serial.begin(9600);

    BoardInit();

    SERVO_Enable(0);
    
//changer l'angle
    SERVO_SetAngle(0, 90);
}

void loop()
{

    }

    /* il faut connaitre bras_haut et bras_bas*/