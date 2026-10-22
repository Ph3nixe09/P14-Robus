#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <LED.h>

// Fonction actionner quand le robot s'allume
void setup() { 
  Serial.begin(9600);
  BoardInit();
  ledInit();

  pinMode(GREEN_PIN, INPUT);
  pinMode(RED_PIN, INPUT);

  beep(3);
  delay(200);
}

// Boucle du robot
void loop() {
  sapinNoel();
}