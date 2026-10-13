/*
Projet: Déplacement du robot dans le labyrinthe - Robus
Équipe: 14B
Auteurs: Félix Albert, Éléna Barabé, Edouard Farley et Rose Villeneuve
Description: Le script sert à faire détecteur le siflet et faire avancer le robot
Date: 08/10/2026
*/

#include <Arduino.h>
#include <LibRobus.h>
#include <FreqMeasureMulti.h>
FreqMeasureMulti freq1;

// Variables
double sommeFreq = 0;
int nbFreq = 0;
double frequence = 0;


// Broches
int pinBruit_Ambiant = A0;   // tension DC du bruit ambiant (D1, R2, R9, C5)

// Seuils a ajuster en labo
int freqMin = 4800;    // limite basse du sifflet
int freqMax = 5200;    // limite haute du sifflet
float seuilBruitMin = 1.0;   // bruit ambiant minimal (en volts)
float seuilBruitMax = 2.5;   // bruit ambiant maximal (en volts)

// -------------------------------
// Fonction pour lire une tension
// -------------------------------
float lireTension(int pin) {
    int valeur = analogRead(pin);
    float tension = (valeur * 5.0) / 1023.0;
    return tension;
}

// -------------------------------
// Initialisation du detecteur
// -------------------------------
void initDetecteurSifflet() {
    freq1.begin(8);   // lecture sur D8
}

// -------------------------------
// Detection du sifflet
// -------------------------------
bool detecterSifflet() {

    // 1. Lire la frequence 5 kHz
    if (freq1.available()) {

        double periode = freq1.read();
        frequence = freq1.countToFrequency(periode);

        sommeFreq = sommeFreq + frequence;
        nbFreq = nbFreq + 1;

        Serial.print("Frequence = ");
        Serial.println(frequence);
    }

    // 2. Lire le bruit ambiant
    float bruit = lireTension(pinBruitDC);
    Serial.print("Bruit Ambiant = ");
    Serial.println(bruit);

    // 3. Quand on a assez de mesures de frequence
    if (nbFreq >= 5) {

        double moyenneFreq = sommeFreq / nbFreq;
        sommeFreq = 0;
        nbFreq = 0;

        Serial.print("Frequence moyenne = ");
        Serial.println(moyenneFreq);

        // 4. Conditions
        bool bonne_frequence = (moyenneFreq > freqMin && moyenneFreq < freqMax);
        bool bon_bruit = (bruit > seuilBruitMin && bruit < seuilBruitMax);

        if (bonne_frequence && bon_bruit) {
            Serial.println("SIFFLET TROUVE !");
            return true;
        }
    }

    return false;
}

// -------------------------------
// Programme principal
// -------------------------------
void setup() {
    BoardInit();
    Serial.begin(9600);

    initDetecteurSifflet();

    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
}

void loop() {

    if (detecterSifflet()) {
        Serial.println("DEPART DU ROBOT !");
        MOTOR_SetSpeed(0, 0.5);
        MOTOR_SetSpeed(1, 0.5);
    } else {
        MOTOR_SetSpeed(0, 0);
        MOTOR_SetSpeed(1, 0);
    }

    delay(10);
}