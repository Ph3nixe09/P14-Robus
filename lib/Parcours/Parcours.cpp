#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>

enum Position {
    Left,
    Middle,
    Right
};

// Commandes pour le défi
Position left() {
  Position current; 
  turnAngleRight(90);
  driveDistance(50);
  turnAngleLeft(90);
  delay(200);
  if (detectWall()) {
    current = Middle;
    return current;
  } else {
    turnAngleRight(90);
    driveDistance(50);
    turnAngleLeft(90);
    current = Right;
    return current;
  }
}

Position middle() {
  Position current; 
  turnAngleLeft(90);
  driveDistance(50);
  turnAngleRight(90);
  delay(200);
  if (detectWall()) {
    current = Left;
    return current;
  } else {
    turnAngleRight(90);
    driveDistance(100);
    turnAngleLeft(90);
    current = Right;
    return current;
  }
}

Position right() {
  Position current; 
  turnAngleLeft(90);
  driveDistance(50);
  turnAngleRight(90);
  delay(200);
  if (detectWall()) {
    current = Middle;
    return current;
  } else {
    turnAngleLeft(90);
    driveDistance(50);
    turnAngleRight(90);
    current = Left;
    return current;
  }
}

void logic() {
  Position position = Middle;
  int level = 1;
  while (level <= 4) {
    switch (position) {
      case Left:
        position = left();
        level = level + driveToWall();
        break;
      case Middle:
        position = middle();
        level = level + driveToWall();
        break;
      case Right:
        position = right();
        level = level + driveToWall();
        break;
    }
  }
  driveDistance(25);
}

//fonction qui enregistre la position des colonne dans une matrice
//à utiliser quand on avance de rangée

void positionMemory(){
  // int index_position = verticalPosition/2;
  // goBackListe[index_position] = horizontalPosition;
}

//inverse la matrice de position
int* reverse_goBackList (int* list){
  //TEST : FONCTIONNE
  static int reverse_list[5] = {0,0,0,0,0};
  for (int i = 0; i < 5; i++) {
    reverse_list[4-i] = list[i];
  }
  return reverse_list;
}

//sous etapes de la fonction qui permet de faire le retour
void goBackStep (int pastX, int futurX, int dist){
  //TEST : FONCTIONNE
  if ((pastX == 1 && futurX == 1) || (pastX == 2 && futurX == 2) || (pastX == 3 && futurX == 3)) {
    driveDistance(dist);}
  if ((pastX == 1 && futurX == 2) || (pastX == 2 && futurX == 3)){
    turnAngleRight(90);
    driveDistance(50);
    turnAngleLeft(90);
    driveDistance(dist);
  }
  if ((pastX == 2 && futurX == 1) || (pastX == 3 && futurX == 2)){
    turnAngleLeft(90);
    driveDistance(50);
    turnAngleRight(90);
    driveDistance(dist);
  }
  if (pastX == 1 && futurX == 3){
    turnAngleRight(90);
    driveDistance(100);
    turnAngleLeft(90);
    driveDistance(dist);
  }
  if (pastX == 3 && futurX == 1){
    turnAngleLeft(90);
    driveDistance(100);
    turnAngleRight(90);
    driveDistance(dist);
  }
}

//fonction qui permet de faire le retour
void goBack(int* liste){
  //TEST : FONCTIONNE
  int* goBackPath = reverse_goBackList(liste);
  int pastPositionX = 0;
  int futurPositionX = 0;
  turnAngleLeft(180);
  driveDistance(50);

  for (int i = 0; i < 4; i++) {
    pastPositionX = goBackPath[i];
    futurPositionX = goBackPath[i+1];
      goBackStep(pastPositionX, futurPositionX, 100);
    }
  }