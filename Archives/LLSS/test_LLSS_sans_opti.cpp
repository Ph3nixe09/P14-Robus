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
#include <Sifflet.h>

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
float vitesse = 1;
int accel_timer = 0;
bool fini = false;
bool sifflet = false;
int test_stop = 0;
bool one_time = true;
bool son = true;

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
};

//Consigne de 0 pour la vitesse pour effectuer un arret
void arret(){
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
  delay(500);
};

void avance(float distance){
  float val_expected_distance = (13228.25 * distance); 
  float KP = 0.0005;
  float distance_right = 0;  //Pour atteindre la distance voulu
  float speed_right = 0.05;
  float speed_correct_left = 0.05;
  float left_error = 0;
  int accel = 0;
  int slow_distance = 0;
  float max_speed = 0.7;

  if (distance >= 5){
    max_speed = 0.8;
  }

  if (distance == 0.5){
    slow_distance = 3000;
  }
  else{
    slow_distance = 3000;
  }

  while (true) {
    int32_t read_pulse_left = ENCODER_ReadReset(0);
    int32_t read_pulse_right =  ENCODER_ReadReset(1);
    //Serial.print("Pulse right / left: ");
    //Serial.println(read_pulse_right);
    //Serial.println(read_pulse_left);

    left_error = (read_pulse_right - read_pulse_left) * KP;

      //Distance reel parcouru
    distance_right += read_pulse_right;
    //Serial.print("Distance total: ");
    //Serial.println(distance_right);

    // Nouveau code d'acceleration à tester
    switch (accel){
      case 0:
        speed_right += 0.01;
        speed_correct_left += 0.01;
        if ((speed_right >= max_speed) || distance_right >= (val_expected_distance / 2)){
          accel = 1;
        };
        break;

      case 1:
        if (distance_right >= (val_expected_distance - slow_distance)){
          speed_right -= 0.02;
          speed_correct_left -= 0.02;
          if (speed_right <= 0.3){
            speed_right = 0.1;
            speed_correct_left = 0.1;
            accel = 2;
          };
        break;
        };
      case 2:
        break;
    };

    if (distance_right >= val_expected_distance - 200){
      arret();
      break;
    };
    
    //Ajustement de la vitesse
    speed_correct_left += left_error;
    //Serial.print("Error left / speed correct left: ");
    //Serial.println(left_error);
    //Serial.println(speed_correct_left);
    
    //Ajustement de la vitesse
    MOTOR_SetSpeed(RIGHT,speed_right);
    MOTOR_SetSpeed(LEFT, speed_correct_left);

    delay(10);

  };
};


void tourner(float angle) {
  float val_expected_turn = -1980.0 * angle;
  float KPT = 0.0005;   // Correction proportionnelle sur l'écart de distance total
  float KIT = 0.00005;  // Correction intégrale pour compenser l'asymétrie des moteurs
  
  float turn_right = 0;
  float turn_left = 0;
  
  // Astuce : si 0.05 est trop proche de la zone morte, passe à 0.10 ou 0.15
  float base_speed = 0.2; 
  int inverter = (val_expected_turn >= 0) ? 1 : -1;

  float speed_right = base_speed * inverter;
  float base_speed_left = -base_speed * inverter;
  float integral_error = 0;

  ENCODER_Reset(0);
  ENCODER_Reset(1);

  while (true) {
    int32_t read_pulse_left = ENCODER_ReadReset(0);
    int32_t read_pulse_right = ENCODER_ReadReset(1);

    // Distances totales parcourues par CHAQUE roue
    turn_left += read_pulse_left;
    turn_right += read_pulse_right;

    // Distance moyenne parcourue par les deux roues (détermine le vrai angle du robot)
    float distance_moyenne = (abs(turn_right) + abs(turn_left)) / 2.0;

    if (distance_moyenne >= (abs(val_expected_turn) - 50)) {
      arret();
      break;
    }

    // Écart de position total entre les deux roues (doit tendre vers 0)
    float total_pos_error = (-1.0 * turn_right) - turn_left;
    integral_error += total_pos_error;

    // Calcul de la vitesse gauche pour maintenir exactement la même distance que la droite
    float speed_correct_left = base_speed_left + (total_pos_error * KPT) + (integral_error * KIT);

    MOTOR_SetSpeed(RIGHT, speed_right);
    MOTOR_SetSpeed(LEFT, speed_correct_left);

    delay(10); // Indispensable pour échantillonner les encodeurs régulièrement (100 Hz)
  }

  ENCODER_Reset(0);
  ENCODER_Reset(1);
}

bool detection_infrarouge(){
  //Détection d'obstacle avec l'infrarouge mais seulement si les deux s'allument
  vert = digitalRead(vertpin);
  rouge = digitalRead(rougepin);
  if (!vert && !rouge){
    return true;
  }
  else {
    return false;
  };
};
/**
Fonctions d'initialisation (setup)
 -> Se fait appeler au debut du programme
 -> Se fait appeler seulement un fois
 -> Generalement on y initilise les varibbles globales
*/


//Logic
int positionX = 1;
int positionY = 0;
//                0,1,2   3,4,5   6,7,8   9,10,11 12,13,14 15,16,17 18,19,20 21,22,23 24,25,26 27,28,29
int portes[30] = {1,1,1,  1,0,1,  1,1,1,  1,0,1,   1,1,1,   1,1,1,   1,1,1,   1,0,1,   1,1,1,   1,0,1};
//Utilisé pour optimiser les mouvements selon l'ancien
int last_state = 0;

void retourne(){
  //Sortir de la ligne d'arrivé
  //0
  if (positionX == 0){
    tourner(0.98);
    avance(0.5);
    tourner(0.98);
    avance(5.5);
  }
  //2
  else{
    //Gauche
    tourner(-1);
    avance(0.5);
    tourner(-0.98);
    avance(5.5);
  };
};

void verif_y(){
  /*DESC: Verifie la presence d'un mur avant de faire un deplacement vers l'avant */

  while (positionY < 27){
    if (detection_infrarouge()){
      //change next matrice at same x position to 0
      portes[((positionY + 3)+positionX)] = 0;
      break;
      
    } else if(not detection_infrarouge()) { //avance et enregistre le path valide en ajoutant 1 au porte valide
      
      
      if (positionX == 1 && portes[((positionY + 3 )+ positionX)] == 0){  // si le robot est en positionX 1 et que la porte devant est fermé, retourne a dirchoice
          break;
      }
      
      portes[((positionY + 3 )+ positionX)] = 2; //pos porte valide
      if (positionY == 24){
        positionY += 3;
        avance(1);
      }
      else{
      
        positionY += 6;
        avance(1);
      };
    };

  };
};

void verif_xMoins(){
  /*DESC: Verifie la presence d'un mur a gauche de sa position en X avant de faire un deplacement vers celle-ci, si elle est libre */
 
 if (not detection_infrarouge()){
    portes[(positionY + positionX)] =2;
    positionX -= 1;
    avance(0.5);
    if (detection_infrarouge() && positionX == 1){
      tourner(1);
    }
    else if(positionX == 0){//
      tourner(1);
    }
    else{
      portes[(positionY + positionX)] =2;
      positionX -= 1;
      avance(0.5);
      tourner(1);
    }

  } else {
    portes[((positionY)+positionX-1)] = 0;
    tourner(1);
    printf("probleme, cul de sac");// manque le s shape
  }
};

void verif_xPlus(){
 /*DESC: Verifie la presence d'un mur a droite de sa position en X avant de faire un deplacement vers celle-ci, si elle est libre */
  if (not detection_infrarouge()){
    portes[(positionY + positionX)] =2;
    positionX ++;
    avance(0.5);

    if (detection_infrarouge() && positionX == 1){//
      tourner(-1);
    }
    else if(positionX == 2){//
      tourner(-1);
    }
    
    else{ // no detection
      portes[(positionY + positionX)] =2;
      positionX ++;

      avance(0.5);
      tourner(-1);
    }

  } else {
    portes[((positionY)+positionX+1)] = 0;
    tourner(-1);
    printf("probleme, cul de sac");
  }

};

void descend(int previous){
  switch (previous){
    case 0:
      tourner(1);
      while (detection_infrarouge() == false){
        portes[(positionY-3)+positionX] = 2;
        avance(1);
        positionY -= 6;
      }
      portes[positionY + positionX] = 2;
      tourner(-1);
      avance(0.5);
      tourner(-1);
      positionX += 1;
      break;

    case 2:
      tourner(-1);
      while (detection_infrarouge() == false){
        portes[(positionY-3)+positionX] = 2;
        avance(1);
        positionY -= 6;
      }
      portes[positionY + positionX] = 2;
      tourner(1);
      avance(0.5);
      tourner(1);
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
        tourner(1);
        avance(1);
        tourner(-1);
        break;
      }
      else{
        tourner(1);
        verif_xPlus();
        break;
      }

    case 1: // au milieu et doit choisir entre -1 et 1 #
      if (portes[((positionY)+(positionX - 1))] == 1 && portes[((positionY+3)+(positionX - 1))]==1 ){
        tourner(-1);//tourne a gauche
        verif_xMoins();
        break;
      }
      else if (portes[((positionY)+(positionX + 1))] == 1 && portes[((positionY+3)+(positionX + 1))]==1 ){
        tourner(1);
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
      tourner(-1);
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

//============================================================================================================================

void setup(){
  BoardInit();
  
  //initialisation
  pinMode(vertpin, INPUT);
  pinMode(rougepin, INPUT);
  delay(100);
  attendreDepart();
  beep(3);
  path();
};


void loop() {
  /**
  detection_sifflet(sifflet);

  if (sifflet && not fini){
    while (!fini){

    }
  }
  
  
  if (detection_sifflet(son)==1){
    Serial.print("Avertissement");
  }else if (detection_sifflet(son)==2){
    Serial.print("Depart");
  }else if (detection_sifflet(son)==3){
    Serial.print("Signal 10s");
  }*/
  

 
  etatPast = etat;
  bumperArr = ROBUS_IsBumper(3);
  if (bumperArr){
    if (etat == 0){
      beep(1);
      path();
      etat = 1;
    } 
    else{
      beep(3);
      arret();
      etat = 0;
    };
  }
  else if (ROBUS_IsBumper(0)){
    if (etat == 0){
      beep(1);
      tourner(1);
      etat = 1;
    }
  }
  else if (ROBUS_IsBumper(1)){
    if (etat == 0){
      beep(1);
      tourner(-1);
      etat = 1;
    }
  }
  else if (ROBUS_IsBumper(2)){
    if (etat == 0){
      beep(1);
      tourner(2);
      etat = 1;
    }
  }
  etat = 0;




  /**
  int32_t gauche = ENCODER_ReadReset(0);
  int32_t droite = ENCODER_ReadReset(1);
  
  Serial.print("Voici la valeur de droite: ");
  Serial.println(droite); // Imprime la valeur et saute une ligne

  Serial.print("Voici la valeur de gauche:::::: ");
  Serial.println(gauche);

  test_stop++;

  if (test_stop == 3){
    MOTOR_SetSpeed(RIGHT,(0.5*vitesse));
    MOTOR_SetSpeed(LEFT, (0.5*vitesse));
    delay(200);
    arret();
    test_stop = 0;
  };

  delay (1000);
  */
};

