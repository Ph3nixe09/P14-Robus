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
float speed = 0.40;
int horizontalPosition = 1;
int verticalPosition = 0;
int state = 0;

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
  state = 0;
}

void loop()
{
  if (detectSound()) 
  {
    if (verticalPosition > 5)
  {
    goBack();
  }
  else
  {
    if (detectObstacle()){
      stop();
      spinLeft(speed);
      driveStraight(speed);
      spinRight(speed);
        if (detectObstacle()){
          stop();
          spinRight(speed);
          driveStraight(speed);
          driveStraight(speed);
          spinLeft(speed);
          driveStraight(speed);
          driveStraight(speed);
          verticalPosition ++;
          horizontalPosition = 2;
        }
        else {
          driveStraight(speed);
          driveStraight(speed);
          verticalPosition ++;
          horizontalPosition = 0;
        }
      }
    else {
      driveStraight(speed);
      verticalPosition ++;
    }
    }
  }
  }