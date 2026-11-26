#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal.h>

// Initialise l'écran LCD (à appeler dans setup() de main.cpp)
void startLCD();

// Affiche jusqu'à 4 lignes de texte sur l'écran 16x4
void LCD(const char* ligne1,
         const char* ligne2 = "",
         const char* ligne3 = "",
         const char* ligne4 = "");

// Efface tout le contenu de l'écran
void clearLCD();

#endif