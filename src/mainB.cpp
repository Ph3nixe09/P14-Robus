#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <math.h>
#include <Parcours.h>

// Fonction actionner quand le robot s'allume
void setup() { 
  Serial.begin(9600);
  BoardInit();

  pinMode(GREEN_PIN, INPUT);
  pinMode(RED_PIN, INPUT);

  delay(100);
  delay(200);
}

// Boucle du robot
void loop() {
  if (getBumberRear()) {
    logic();
  } 
  else if (getBumberLeft()) {
    // spinLeft(0.3);
    turnAngleLeft(89.6);
    // left();
  } else if (getBumberRight()) {
    // spinRight(0.3);
    turnAngleRight(91.4);
    // right();
  } else if (getBumberFront()) {
    middle();
  }
}