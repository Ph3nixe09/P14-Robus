/*
Projet: Le nom du script
Equipe: Votre numero d'equipe
Auteurs: Les membres auteurs du script
Description: Breve description du script
Date: Derniere date de modification
*/

/*
Inclure les librairies de functions que vous voulez utiliser
*/
#include <LibRobus.h>

/*
Variables globales et defines
 -> defines...
 -> L'ensemble des fonctions y ont acces
*/

bool bumperArr;
int vertpin = 48;
int rougepin = 49;
bool vert = false;
bool rouge = false;
int etat = 0; // = 0 arrêt 1 = avance 2 = recule 3 = TourneDroit 4 = TourneGauche
int etatPast = 0;
float vitesse = 0.40;
int accel_timer = 0;

/*
Vos propres fonctions sont creees ici
*/

void beep(int count){
  for(int i=0;i<count;i++){
    AX_BuzzerON();
    delay(100);
    AX_BuzzerOFF();
    delay(100);  
  }
  delay(400);
}

void arret(){
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
};

void avance(){
  MOTOR_SetSpeed(RIGHT,vitesse);
  MOTOR_SetSpeed(LEFT, vitesse);
};

void recule(){
  MOTOR_SetSpeed(RIGHT, -vitesse);
  MOTOR_SetSpeed(LEFT, -vitesse);
};

void tourneDroit(){
  MOTOR_SetSpeed(RIGHT, 0.5*vitesse);
  MOTOR_SetSpeed(LEFT, -0.5*vitesse);
};

void tourneGauche(){
  MOTOR_SetSpeed(RIGHT, -0.5*vitesse);
  MOTOR_SetSpeed(LEFT, 0.5*vitesse);
};

/*
Fonctions d'initialisation (setup)
 -> Se fait appeler au debut du programme
 -> Se fait appeler seulement un fois
 -> Generalement on y initilise les varibbles globales
*/
void setup(){
  BoardInit();
  
  //initialisation
  pinMode(vertpin, INPUT);
  pinMode(rougepin, INPUT);
  delay(100);
  beep(3);
}

/*
Fonctions de boucle infini
 -> Se fait appeler perpetuellement suite au "setup"
*/
void loop() {
  etatPast = etat;
  bumperArr = ROBUS_IsBumper(3);
  if (bumperArr){
    if (etat == 0){
      beep(8);
      etat = 1;
    } 
    else{
      beep(1);
      etat = 0;
    }
  }
  
  vert = digitalRead(vertpin);
  rouge = digitalRead(rougepin);
  if (etat > 0){
    if (vert && rouge){ // aucun obstacle => avance
      etat = 1;
    }
    if (!vert && !rouge){  // obstacle devant => recule
      etat = 2;
    }
    if (!vert && rouge){ // obstacle à gauche => tourne droit
        etat = 3;
      }
    if (vert && !rouge){ // obstacle à droite => tourne gauche
        etat = 4;
    }
  }

  if (etatPast != etat){
    if (etatPast == 2){ // si on reculait, on continue de reculer
      recule();
      delay(500);
      tourneDroit();
      delay(900);
      avance();
      delay(1100);
      tourneGauche();
      delay(900);
      arret();
    }
    else{
      arret();
      delay(50);
      }
  }
  else{
    if (accel_timer > 10 && etat == 1){
      vitesse += 0.05;
      if (vitesse > 1.0){
        vitesse = 1.0;
      }
      avance();
    }
    else if (accel_timer <= 10){
      vitesse -= 0.5;
      avance();
    }
    else{
      switch (etat)
      {
      case 0:
        arret();
        accel_timer = 0;
        break;
      case 1:
        avance();
        accel_timer += 1;
        break;
      case 2:
        recule();
        accel_timer = 0;
        break;
      case 3:
        tourneDroit();
        accel_timer = 0;
        break;
      case 4:
        tourneGauche();
        accel_timer = 0;
        break;            
      default:
        avance();
        etat = 1;
      break;
      }
    }
    
  }
  delay(200);
}
