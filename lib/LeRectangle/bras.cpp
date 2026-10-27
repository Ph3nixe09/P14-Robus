#include <LibRobus.h>
#include <Arduino.h>


const uint8_t BRAS_HAUT = 30;
const uint8_t BRAS_BAS = 120;
const uint8_t PINCE_OUVERTE = 130;
const uint8_t PINCE_FERME = 160;

// Fonction du bras qui vont être appeler

void allumerBras()
{
    SERVO_Enable(0); // Bras
    SERVO_Enable(1); // Pince
}
void leverBras()
{
    SERVO_SetAngle(0, BRAS_HAUT);
}

void baisserBras()
{
    SERVO_SetAngle(0, BRAS_BAS);
}

void ouvrirPince()
{
    SERVO_SetAngle(1, PINCE_OUVERTE);
}

void fermerPince()
{
    SERVO_SetAngle(1, PINCE_FERME);
} 
    
void eteindreBras()
{
    SERVO_Disable(0); // Bras
    SERVO_Disable(1); // Pince
}
