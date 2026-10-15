#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <math.h>
#include <Parcours.h> 
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
  Serial.println("GOOOO");
  
  //Le robot commence son parcour
    logic();
}


// Boucle du robot
void loop() {
  if (getBumberRear()) {
    logic();
  } 
  else if (getBumberLeft()) {
    // spinLeft(0.3);
    turnAngleLeft(90);
    // left();
  } else if (getBumberRight()) {
    // spinRight(0.3);
    turnAngleRight(90);
    // right();
  } else if (getBumberFront()) {
    middle();
  }
}