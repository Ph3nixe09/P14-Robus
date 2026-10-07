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

// Initial vertical positioning
int InitialVerticalPosition () {
    // Variable declarations
    int initial_vertical_position = 0;
    int max_vertical_position = 5;
    int vertical_position = 0;
    if (vertical_position == max_vertical_position){
        state = 0; // arret
    }
    else {
        green = digitalRead(greenpin);
        red = digitalRead(redpin);
        if (state > 0) {
            if (green && red) { // aucun obstacle, avance
                state = 1;
            }
            if (!green && !red) { // obstacle, arrête
                state = 2;
            }
        }
    }
    return {vertical_position};
}

// Initial horizontal positioning
int InitialHorizontalPosition (){
    // Variable declarations
    int initial_horizontal_position = 2;
    int horizontal_position = 0;
}

// Matrices de position à jour pour le retour

// mettre des limites de position ex: peut pas aller en bas de 1 ou plus haut que 3
// éviter de changer la position trop vite lorsqu'il est au milieu pour éviter qu'il sorte des limites

// Matrice de position horizontale

 // Si la horizontal_position est égale à 2:
        // Est-ce qu'il y a un objet devant moi?
            // Oui
                // Je me tourne vers la gauche et vérifie s'il y a un objet.
                // horizontal_position -= 1
                // Est-ce qu'il y a un objet devant moi?
                    // Oui
                        // Je me tourne vers la droite, j'avance de 1 m et j'avance
                        // horizontal_position += 2
                    // Non
                        // J'avance de 1 m.
                        // vertical_position += 1
            // Non
                // J'avance de 1,0 m.
                // vertical_position += 1


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
