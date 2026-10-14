
#ifndef SIFFLET_H
#define SIFFLET_H

#include <Arduino.h>

// Broches et configuration
#define PIN_INTENSITE_5KHZ A0
#define PIN_BRUIT_AMBIANT A1
#define DELAI_BOUCLE_MS 5  // 5 ms par tour de boucle

// Types de signaux
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

// --------------------------------------------------
// Détecteur sonore
// --------------------------------------------------

inline TypeSignal analyserFrequence(void) {
    const int SEUIL_DIFFERENCE = 10;
    const int SEUIL_BRUIT_5KHZ = 500;
    const int SEUIL_BRUIT_1KHZ = 400;

    int val5kHz = analogRead(PIN_INTENSITE_5KHZ);
    int valBruit = analogRead(PIN_BRUIT_AMBIANT);

    if (val5kHz > SEUIL_BRUIT_5KHZ &&
        (val5kHz - valBruit) > SEUIL_DIFFERENCE) {
        return TYPE_5KHZ;
    }

    if (valBruit > SEUIL_BRUIT_1KHZ) {
        return TYPE_1KHZ;
    }

    return TYPE_AUCUN;
}

// --------------------------------------------------
// Détection de la séquence de départ
// 3 bips de 1 kHz suivis d'un signal de 5 kHz
// --------------------------------------------------

inline void attendreDepart(void) {
    const long PERIODE_MS = 15;
    const long DUREE_BIP_MS = 300;
    const long DUREE_5KHZ_MS = 1900;
    const long TOLERANCE_MS = 200;
    const long TIMEOUT_MS = 3000;

    int step = 0;
    int bip_amount = 0;

    bool signal_en_cours = false;
    bool deja_valide = false;

    long debut = 0;
    long dernier_match = 0;
    long time_last_event = millis();

    Serial.println("[DEPART] Attente: 3 bips 1kHz...");

    while (true) {
        long now = millis();
        TypeSignal signalActuel = analyserFrequence();

        TypeSignal attendu =
            (step == 0) ? TYPE_1KHZ : TYPE_5KHZ;

        long dureeRequise =
            (step == 0) ? DUREE_BIP_MS : DUREE_5KHZ_MS;

        if (signalActuel == attendu) {
            if (!signal_en_cours) {
                signal_en_cours = true;
                debut = now;

                Serial.print("[SIGNAL] Debut ");
                Serial.println(step == 0 ? "1kHz" : "5kHz");
            }

            dernier_match = now;
            time_last_event = now;

            if (!deja_valide && (now - debut) >= dureeRequise) {
                deja_valide = true;
                Serial.println("[DEPART] 5kHz valide -> GO !");
                return;
            }
        } else if (signal_en_cours &&
                   (now - dernier_match) > TOLERANCE_MS) {

            Serial.print("[SIGNAL] Perdu apres ");
            Serial.print(dernier_match - debut);
            Serial.println(" ms");

            signal_en_cours = false;
            deja_valide = false;
        }

        if (!signal_en_cours &&
            (step == 1 || bip_amount > 0) &&
            (now - time_last_event) > TIMEOUT_MS) {

            Serial.println(
                "[TIMEOUT] Sequence annulee, retour au debut"
            );

            step = 0;
            bip_amount = 0;
            time_last_event = now;
        }

        delay(PERIODE_MS);
    }
}

#endif
