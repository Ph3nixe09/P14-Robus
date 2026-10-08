/*
Projet: Déplacement du robot dans le labyrinthe - Robus
Équipe: 14B
Auteurs: Félix Albert, Éléna Barabé, Edouard Farley et Rose Villeneuve
Description: Le script suivant sert à faire avancer le robot dans un labyrinthe inconnu avec des dimensions connues
Date: 01/10/2026
*/

// Librairies
#include <LibRobus.h>
#include <stdio.h>
#include <Arduino.h>

// Global variables and definitions
bool bumperArr;
int greenpin = 48;
int redpin = 49;
bool green = false;
bool red = false;
int state = 0; // = 0 arrêt 1 = avance 2 = recule 3 = TourneDroit 4 = TourneGauche
int PastState = 0;
float speed = 0.40;

bool detectObject(){
    
}

void goBack()
{
  // int reversedPath = 
}

bool detectSound()
{
  if (3>2) {
    return true;
  }
  else {
    return false;
  }
}
