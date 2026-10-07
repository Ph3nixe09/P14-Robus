/*
Projet: Déplacement du robot dans le labyrinthe - Robus
Équipe: 14B
Auteurs: Félix Albert, Éléna Barabé, Edouard Farley et Rose Villeneuve
Description: Le script suivant sert à faire avancer le robot dans un labyrinthe inconnu avec des dimensions connues
Date: 04/10/2026
*/

// Librairies
#include <LibRobus.h>
#include <stdio.h>
#include <Arduino.h>
#include "pidB.h"
#include "positionB.h"

// Global variables
int last_left = 0;
int last_right = 0;
int tot_left = 0;
int tot_right = 0;
int greenpin = 48;
int redpin = 49;
bool green = false;
bool red = false;
int state = 0; // = 0 arrêt 1 = avance 2 = recule 3 = TourneDroit 4 = TourneGauche
int pastState = 0;
float speed = 0.40;
int horizontalPosition = 1;
int verticalPosition = 0;

// Setup
void setup()
{
  Serial.begin(9600);

  BoardInit();

  pinMode(greenpin, INPUT);
  pinMode(redpin, INPUT);

  delay(100);

  beep(3);

  verticalPosition = 0;
  horizontalPosition = 1;
}

// Loop
void loop()
{
  if (detectSound()) {
    if (verticalPosition > 5)
  {
    goBack();
  }
  else
  {
  pastState = state;
  green = digitalRead(greenpin);
  red = digitalRead(redpin);
  if (state > 0) {
    if (green && red)
    { // pas d'obstacles: avance
      state = 1;
    }
  }
  if (pastState != state)
  {
    stop();
    delay(50);
  }
  else
  {
    switch (state){
    case 0:
      stop();
      break;
    case 1:
      driveStraight();
      break;
    case 2:
      spinRight(0.35);
      break;
    case 3:
      spinLeft(0.35);
      break; // faudrait mettre un 90 degrés?
    default:
      driveStraight();
      state= 1;
      break;
    }
  }
  delay(200);
  }
  }
  else {
    stop();
  }
}