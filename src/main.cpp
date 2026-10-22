#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <LED.h>
#include <Yellow.h>

enum Color {
  GREEN,
  RED,
  BLUE,
  ORANGE,
  YELLOW
};

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
  enum Color color = BLUE;

  // if color...

  switch (color)
  {
  case GREEN:
    /* code */
    break;
  case RED:
    /* code */
    break;
  case BLUE:
    /* code */
    break;
  case ORANGE:
    /* code */
    break;
  case YELLOW:
    /* code */
    break;
  }


  // if (getBumberRear()) {
  //   yellow();
  // } 
  // else if (getBumberLeft()) {
  //   // spinLeft(0.3);
  //   turnAngleLeft(90);
  //   // left();
  // } else if (getBumberRight()) {
  //   // spinRight(0.3);
  //   turnAngleRight(90);
  //   // right();
  // } else if (getBumberFront()) {
  //   driveDistance(100);
  // }
}