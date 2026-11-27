#include <Arduino.h>
#include <numpad.h>

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char key = getNumpad();
  
  if (key) { // Une touche a été peser 
    Serial.println(key);
    // La touche est stockée dans key
    // Changer l'état de la commande
  }
}