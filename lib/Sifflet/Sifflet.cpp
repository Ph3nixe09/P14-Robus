#include <LibRobus.h>
#include <Arduino.h>

#define PIN_INTENSITE_5KHZ A0
#define PIN_BRUIT_AMBIANT A1
#define DELAI_BOUCLE_MS 5 // 5 ms par tour de boucle

typedef enum { TYPE_AUCUN, TYPE_1KHZ, TYPE_5KHZ } TypeSignal;
 
typedef enum {
  SIGNAL_AUCUN,
  SIGNAL_AVERTISSEMENT,
  SIGNAL_DEPART,
  SIGNAL_10_SECONDES
} SignalDetecte;

/*
--------------------------------------------------------------------------------------------------------
DÉTECTEUR SONORE
--------------------------------------------------------------------------------------------------------
*/
 
TypeSignal analyserFrequence(void) {
  const int SEUIL_DIFFERENCE = 10;  // Écart minimal pour 5 kHz
  const int SEUIL_BRUIT_5KHZ = 500; // Seuil bip à 5khz dans le bruit ambiant
  const int SEUIL_BRUIT_1KHZ = 400; // Seuil bip à 1 kHz dans le bruit ambiant
 
  int val5kHz = analogRead(PIN_INTENSITE_5KHZ);
  int valBruit = analogRead(PIN_BRUIT_AMBIANT);
 
  if (val5kHz > SEUIL_BRUIT_5KHZ && (val5kHz - valBruit) > SEUIL_DIFFERENCE) {
    return TYPE_5KHZ;
  }
  if (valBruit > SEUIL_BRUIT_1KHZ) {
    return TYPE_1KHZ;
  }
  return TYPE_AUCUN;
}
 
// Fonction pour détecter assigner le son entendu avec son type de signa
void attendreDepart() {
  const long PERIODE_MS = 15;
  const long DUREE_BIP_MS = 300;
  const long DUREE_5KHZ_MS = 1900;
  const long TOLERANCE_MS = 200;
  const long TIMEOUT_MS = 3000;
 
  int step = 0; // 0 = attente des 3 bips, 1 = attente du 5 kHz
  int bip_amount = 0;
  bool signal_en_cours = false; // un signal du bon type est en cours
  bool deja_valide = false;     // ce signal a déjà été compté
  long debut = 0;
  long dernier_match = 0;
  long time_last_event = millis();
 
  Serial.println("[DEPART] Attente: 3 bips 1kHz...");
 
  while (true) {
    long now = millis();
    TypeSignal signalActuel = analyserFrequence();
 
    TypeSignal attendu = (step == 0) ? TYPE_1KHZ : TYPE_5KHZ;
    long dureeRequise = (step == 0) ? DUREE_BIP_MS : DUREE_5KHZ_MS;
 
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
 
        if (step == 0) {
          bip_amount++;
          Serial.print("[BIP] Valide #");
          Serial.println(bip_amount);
 
          if (bip_amount == 3) {
            Serial.println("[ETAPE] 3 bips OK -> attente du 5kHz (2s)");
            step = 1;
            signal_en_cours = false;
            deja_valide = false;
            time_last_event = now;
          }
        } else {
          Serial.println("[DEPART] 5kHz valide -> GO !");
          return;
        }
      }
    } else if (signal_en_cours && (now - dernier_match) > TOLERANCE_MS) {
      Serial.print("[SIGNAL] Perdu apres ");
      Serial.print(dernier_match - debut);
      Serial.println(" ms");
      signal_en_cours = false;
      deja_valide = false;
    }
 
    if (!signal_en_cours && (step == 1 || bip_amount > 0) &&
        (now - time_last_event) > TIMEOUT_MS) {
      Serial.println("[TIMEOUT] Sequence annulee, retour au debut");
      step = 0;
      bip_amount = 0;
      time_last_event = now;
    }
 
    delay(PERIODE_MS);
  }
}