#ifndef SIFFLET_H
#define SIFFLET_H

#include <Arduino.h>
#include <LibRobus.h>

// ---------------------------------------------------------------------------
// Broches
// ---------------------------------------------------------------------------
#define PIN_INTENSITE_5KHZ A0
#define PIN_BRUIT_AMBIANT  A1

// ---------------------------------------------------------------------------
// Constantes
// ---------------------------------------------------------------------------
#define DELAI_BOUCLE_MS 5 // 5 ms par tour de boucle

// ---------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------
typedef enum {
  TYPE_AUCUN,
  TYPE_1KHZ,
  TYPE_5KHZ
} TypeSignal;

typedef enum {
  SIGNAL_AUCUN,
  SIGNAL_AVERTISSEMENT,
  SIGNAL_DEPART,
  SIGNAL_10_SECONDES
} SignalDetecte;

int step;

// ---------------------------------------------------------------------------
// Fonctions
// ---------------------------------------------------------------------------

// Lit les entrées analogiques et retourne le type de signal sonore détecté
// (TYPE_AUCUN, TYPE_1KHZ ou TYPE_5KHZ).
TypeSignal analyserFrequence(void);

// Bloque jusqu'à la détection de la séquence de départ :
// 3 bips à 1 kHz suivis d'un signal à 5 kHz (~2 s).
void attendreDepart(void);

#endif // SIFFLET_H