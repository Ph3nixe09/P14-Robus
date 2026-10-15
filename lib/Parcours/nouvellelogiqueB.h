#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>

int positionX = 1;
int positionY = 0;
//                0,1,2   3,4,5   6,7,8   9,10,11 12,13,14 15,16,17 18,19,20 21,22,23 24,25,26 27,28,29
int portes[30] = {1,1,1,  1,0,1,  1,1,1,  1,0,1,   1,1,1,   1,1,1,   1,1,1,   1,0,1,   1,1,1,   1,0,1};
//Utilisé pour optimiser les mouvements selon l'ancien
int last_state = 0;

void retourne(){
  last_state = 0;
  //Sortir de la ligne d'arrivé
  driveDistance(50);
  positionY -= 3;
  while (positionY != 0){
    Serial.println(positionX);
    Serial.println(positionY);
    //0
    if (portes[(positionY - 3) + positionX] == 2){
      //Avance
      portes[positionY + positionX] = 3;
      positionY -= 6;
      /**if (last_state == 1){
        tourner(1);
        avance(1);
      }
      else if (last_state == 2){
        tourner(-1);
        avance(1);
      }
      else{
        avance(1);
      }*/
      driveDistance(100);
      last_state = 0;
      Serial.println("Passage en avant");
    }
    //1
    else if (portes[(positionY + (positionX + 1))] == 2){
      //Gauche
      portes[positionY + positionX] = 3;
      positionX += 1;
      if (last_state == 0){
        turnAngleLeft(90);
        driveDistance(50);
      }
      else if (last_state == 3){
        turnAngleRight(90);
        driveDistance(50);
        turnAngleRight(90);
      }
      else{
        driveDistance(50);
        turnAngleRight(90);
      };
      last_state = 1;
      Serial.println("Passage a gauche");
    }
    //2
    else if (portes[(positionY + (positionX - 1))] == 2){
      //Droite
      portes[positionY + positionX] = 3;
      positionX -= 1;
      if (last_state == 0){
        turnAngleRight(90);
        driveStraight(50);
      }
      else if (last_state == 3){
        turnAngleLeft(90);
        driveDistance(50);
        turnAngleLeft(90);
      }
      else{
        driveDistance(50);
        turnAngleLeft(90);
      };
      last_state = 2;
      Serial.println("Passage a droite");
    }
    //3
    //Pour éviter recule dans le finish
    else if (positionY < 24){
      if (portes[(positionY + 3) + positionX] == 2){
        //Recule
        portes[(positionY + 3) + positionX] = 3;
        positionY += 6;
        if (last_state == 1){
          turnAngleLeft(90);
          driveDistance(100);
        }
        else{
          turnAngleRight(90);
          driveDistance(100);
        };
      };
      last_state = 3;
      Serial.println("Passage en arriere");
    };
  };
};

void verif_y(){
  /*DESC: Verifie la presence d'un mur avant de faire un deplacement vers l'avant */

  while (positionY < 27){
  
  if (not detectWall()){ //---
    //change next matrice at same x position to 0
    portes[((positionY + 3)+positionX)] = 0;
    break;
  
  } else if(detectWall()) { //avance et enregistre le path valide en ajoutant 1 au porte valide     //---
    
    portes[((positionY + 3 )+ positionX)] = 2; //pos porte valide
    if (positionY == 24){
      positionY += 3;
      driveDistance(50); //---
    }
    else{
      positionY += 6;
      driveDistance(100); //---
    };

  };

  };
};

void verif_xMoins(){
  /*DESC: Verifie la presence d'un mur a gauche de sa position en X avant de faire un deplacement vers celle-ci, si elle est libre */
 
 if (detectWall()){
    portes[(positionY + positionX)] =2;
    positionX -= 1;
    driveDistance(50);
    if (detectWall() && positionX == 1){
      turnAngleRight(90);
    }
    else if(positionX == 0){//
      turnAngleLeft(90);
    }
    else{
      portes[(positionY + positionX)] =2;
      positionX -= 1;
      driveDistance(50);
      turnAngleRight(90);
    }

  } else {
    portes[((positionY)+positionX-1)] = 0;
    turnAngleRight(90);
    printf("probleme, cul de sac");// manque le s shape
  }
};

void verif_xPlus(){
 /*DESC: Verifie la presence d'un mur a droite de sa position en X avant de faire un deplacement vers celle-ci, si elle est libre */
  if (detectWall()){
    portes[(positionY + positionX)] =2;
    positionX ++;
    driveDistance(50);

    if (not detectWall() && positionX == 1){
      turnAngleLeft(90);
    }
    else if(positionX == 2){//
      turnAngleLeft(90);
    }
    
    else{ // no detection
      portes[(positionY + positionX)] =2;
      positionX ++;

      driveDistance(50);
      turnAngleLeft(90);
    }

  } else {
    portes[((positionY)+positionX+1)] = 0;
    turnAngleLeft(90);
    printf("probleme, cul de sac");
  }

};

void descend(int previous){
  switch (previous){
    case 0:
      turnAngleRight(90);
      while (detectWall() == true){
        portes[(positionY-3)+positionX] = 2;
        driveDistance(1);
        positionY -= 6;
      }
      portes[positionY + positionX] = 2;
      turnAngleLeft(90);
      driveDistance(50);
      turnAngleLeft(90);
      positionX += 1;
      break;

    case 2:
      turnAngleLeft(90);
      while (detectWall() == true){
        portes[(positionY-3)+positionX] = 2;
        driveDistance(1);
        positionY -= 6;
      }
      portes[positionY + positionX] = 2;
      turnAngleRight(90);
      driveDistance(50);
      turnAngleRight(90);
      positionX -= 1;
      break;
  }
};

void dirChoice(){
  //Vérification si l'on a terminé le parcours
  if (positionY < 27){
  //Choisi la direction a tourner selon l'etat 0 ou 1 autour de cette positions X Y. Selon le choix, il tourne et appel verif x
  switch (positionX)
    {
    case 0:
      if (portes[(positionY + 3)+(0)]== 0 && portes[((positionY+3)+(1))] == 0 ){
        positionX = 2;
        turnAngleRight(90);
        driveDistance(100);
        turnAngleLeft(90);
        break;
      }
      else{
        turnAngleRight(90);
        verif_xPlus();
        break;
      }

    case 1: // au milieu et doit choisir entre -1 et 1 #
      if (portes[((positionY)+(positionX - 1))] == 1 && portes[((positionY+3)+(positionX - 1))]==1 ){
        turnAngleLeft(90);//tourne a gauche
        verif_xMoins();
        break;
      }
      else if (portes[((positionY)+(positionX + 1))] == 1 && portes[((positionY+3)+(positionX + 1))]==1 ){
        turnAngleLeft(90);
        verif_xPlus();
        break;
      }

      //situation en s en arrivant par 0
      else if(portes[((positionY+3)+(0))] == 0 && portes[((positionY)+(2))] == 0 ){
        descend(0);
        break;

      }
      
      //situation en s en arrivant par 2
      else if(portes[((positionY)+(0))] == 0 && portes[((positionY+3)+(2))] == 0 ){
        descend(2);
        break;

      }

    case 2:
      turnAngleLeft(90);
      verif_xMoins();
      break;

    default:
      break;
    }
   
  
};
};

void path(){
  //alterne entre avancer ou tourner jusqu'a la position 9 en Y et effectue le trajet inverse
  while (positionY < 27) 
  {
    verif_y();
    dirChoice();
    Serial.print("Position actuel X:");
    Serial.println(positionX);
    Serial.print("Position actuel Y:");
    Serial.println(positionY);
    for (int val : portes){
    Serial.println(val);
    }
  }
  //reverse path
  retourne();
};