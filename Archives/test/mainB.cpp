#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <math.h>
#include <nouvellelogiqueB.h> 
#include <Sifflet.h>

// Fonction actionner quand le robot s'allume
void setup() { 
  Serial.begin(9600);
  BoardInit();

  pinMode(GREEN_PIN, INPUT);
  pinMode(RED_PIN, INPUT);

  beep(3);
  delay(200);
  attendreDepart();
  path();
}

void loop() {
  delay(1000);
}