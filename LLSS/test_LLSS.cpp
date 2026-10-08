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
float vitesse = 1;
int accel_timer = 0;
bool fini = false;
bool sifflet = false;
int test_stop = 0;
bool one_time = true;

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
};
/** PID 2
void avance_2PID(float distance){
  float val_expected_distance = (13400 * distance); 
  int val_expected_speed = 1000;
  float KPD = 0.00002;
  float KPL = 0.00002;
  //float KID = 0.00003;
  //float KIL = 0.00003;
  float distance_right = 0;
  float distance_left = 0;
  int cycle = 0;
  float speed_correct_right = 0.6;
  float speed_correct_left = 0.6;
  float left_error = 0;
  float right_error = 0;

  while (true) {
    int32_t read_pulse_left = ENCODER_ReadReset(0);
    int32_t read_pulse_right =  ENCODER_ReadReset(1);
    Serial.print("Pulse right / left: ");
    Serial.println(read_pulse_right);
    Serial.println(read_pulse_left);


    //Difference avec la realite pour la vitesse, calcul directement de l'ajustement
    left_error = 0;
    right_error = 0;    
    left_error = (val_expected_speed - read_pulse_left)*KPL;    
    right_error = (val_expected_speed - read_pulse_right)*KPD;  
    Serial.print("1er right / left error: ");
    Serial.println(right_error);
    Serial.println(left_error);


    //Distance reel parcouru
    distance_right += read_pulse_right;
    distance_left += read_pulse_left;
    
    //Diminution de la vitesse vers la fin de la distance
    if (distance_right >= (val_expected_distance)  && distance_left >= (val_expected_distance)){
      //Vitesse a zero pour arreter le robot
      arret();
      break;
    };

    //Calcul de l'ajustement de la distance totale
    //left_error += (((cycle * val_expected_speed) - distance_left) * KIL);
    //right_error += (((cycle * val_expected_speed) - distance_right) * KID);
    //Serial.print("2nd right / left error: ");
    //Serial.println(right_error);
    //Serial.println(left_error);

    //Ajustement des variables de vitesse
    speed_correct_right += right_error;
    speed_correct_left += left_error;
    Serial.print("Vitesse appliqué: ");
    Serial.println(speed_correct_right);
    Serial.println(speed_correct_left);

    //Ajustement de la vitesse
    MOTOR_SetSpeed(RIGHT,speed_correct_right);
    MOTOR_SetSpeed(LEFT, speed_correct_left);

    //Un cycle complete
    cycle++;
    Serial.print("Valeur du cycle: ");
    Serial.println(cycle);

    delay(300);

  };
};
*/
void avance(float distance){
  float val_expected_distance = (13228.25 * distance); 
  float KP = 0.0005;
  float distance_right = 0;  //Pour atteindre la distance voulu
  float speed_right = 0;
  float speed_correct_left = 0;
  float left_error = 0;
  int accel = 0;
  int slow_distance = 0;

  //Choix de distance pour ralentir le robot
  if (distance == 0.5){
      slow_distance = 2;    //Moitié à 50cm
  }
  else if (distance == 1){
    slow_distance = 3;      //Tier à 1m
  }
  else{
    slow_distance = 7;      //Le reste du temps (3m environ) au septième
  };

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
        speed_right += 0.1;
        speed_correct_left += 0.1;
        if ((speed_right >= 0.7) || distance_right >= (val_expected_distance / 2)){
          accel = 1;
        };
        break;

      case 1:
        if (distance_right >= (val_expected_distance - (val_expected_distance / slow_distance))){
          speed_right -= 0.2;
          speed_correct_left -= 0.2;
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

    delay(200);

  };
  delay(1000);
};

void recule(){
  MOTOR_SetSpeed(RIGHT, -vitesse);
  MOTOR_SetSpeed(LEFT, -vitesse);
};

void tourner(int angle){ // 1 -> rotation à 90° -1 -> rotation à -90° 2 -> rotation à 180°
  float expected_val_distance_90 = 1300; // Distance en pulse

  switch (angle)
  {
    case 1: {// Pivot à droite (90)
      float right_rotation_distance = 0;
      float right_motor_speed_right_rotation = 0;
      float left_motor_speed_right_rotation = 0;
      float left_motor_error_right_rotation = 0;
      float right_rotation_KP = 0.00003;

      while(true){
        // Lecture des encodeurs
        int32_t read_pulse_right = ENCODER_ReadReset(1);
        int32_t read_pulse_left = ENCODER_ReadReset(0);
                
        left_motor_error_right_rotation = (read_pulse_right - read_pulse_left) * right_rotation_KP;

        right_rotation_distance += read_pulse_right;

        if (right_rotation_distance < 1000){
          right_motor_speed_right_rotation += 0.07;
          left_motor_speed_right_rotation += 0.07;
        }
        else if (right_rotation_distance >= expected_val_distance_90){
          arret();
          //Serial.print("arret atteint");
          break;
        }

        left_motor_speed_right_rotation += left_motor_error_right_rotation;

        MOTOR_SetSpeed(RIGHT, -right_motor_speed_right_rotation);
        MOTOR_SetSpeed(LEFT,   left_motor_speed_right_rotation);

        delay(300);
      }
      break;
    }
    case -1: { // Pivot à gauche (-90)
      float left_rotation_distance = 0;
      float right_motor_speed_left_rotation = 0;
      float left_motor_speed_left_rotation = 0;
      float left_motor_error_left_rotation = 0;
      float left_rotation_KP = 0.00003;

      while (true){
        int32_t read_pulse_right = ENCODER_ReadReset(1);
        int32_t read_pulse_left = ENCODER_ReadReset(0);
        Serial.println("Encodeur lu " + read_pulse_right);
        Serial.println("Encodeur lu " + read_pulse_left);

        left_motor_error_left_rotation = (read_pulse_right - read_pulse_left) * left_rotation_KP;

        left_rotation_distance += read_pulse_right;

        if (left_rotation_distance < 1500){
          right_motor_speed_left_rotation += 0.07;
          left_motor_speed_left_rotation += 0.07;
          Serial.println("Acceleration atteint");
        }
        else if (left_rotation_distance >= expected_val_distance_90){
          arret();
          Serial.println("arret atteint");
          break;
        }

        left_motor_error_left_rotation += left_motor_error_left_rotation;

        MOTOR_SetSpeed(RIGHT, right_motor_speed_left_rotation);
        MOTOR_SetSpeed(LEFT, -left_motor_speed_left_rotation);

        delay(300);
      }
      break;
    }
    case 2: {// Demi-tour (180)
      float expected_val_distance_turn_over = 2600; // Distance en pulse pour parcourir 180°
      float turn_over_KP = 0.00003;
      float left_motor_error_turn_over = 0;
      float distance_turn_over = 0;
      float right_motor_speed_turn_over = 0;
      float left_motor_speed_turn_over = 0;

      while (true){
        int32_t read_pulse_right = ENCODER_ReadReset(1);
        int32_t read_pulse_left = ENCODER_ReadReset(0);

        left_motor_error_turn_over = (read_pulse_right - read_pulse_left) * turn_over_KP;

        distance_turn_over += read_pulse_right;

        if (distance_turn_over < 1500){
          right_motor_speed_turn_over += 0.07;
          left_motor_speed_turn_over += 0.07;
        }
        else if (distance_turn_over >= expected_val_distance_turn_over){
          arret();
          break;
        }

        left_motor_speed_turn_over += left_motor_error_turn_over;

        MOTOR_SetSpeed(RIGHT, -right_motor_speed_turn_over);
        MOTOR_SetSpeed(LEFT,   left_motor_speed_turn_over);

        delay(300);
      }
      break;
    }
    default: {
      Serial.print("fct tourner échoué");
      break;
    }
  }
  //MOTOR_SetSpeed(RIGHT, speed);
  //MOTOR_SetSpeed(LEFT, -speed);
/**
 switch (angle){
  case 1:
    MOTOR_SetSpeed(RIGHT,-0.4);
    MOTOR_SetSpeed(LEFT, 0.4);
    delay(500);
    arret();
    break;
  case -1:
    MOTOR_SetSpeed(RIGHT,0.4);
    MOTOR_SetSpeed(LEFT, -0.4);
    delay(500);
    arret();
    break;
  case 2:
    MOTOR_SetSpeed(RIGHT, -0.4);
    MOTOR_SetSpeed(LEFT, 0.4);
    delay(1000);
    arret();
    break;
 };*/
};

bool detection_sifflet(bool &son){
  //Détection du sifflet pour démarrer le programme de labyrinthe
  delay(500);   //Attends une demi seconde
  return (son = true);
};

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
int portes[30] = {1,1,1,  1,1,1,  1,1,1,  1,1,1,   1,1,1,   1,1,1,   1,1,1,   1,1,1,   1,1,1,   1,1,1};



void retourne(){
  //Sortir de la ligne d'arrivé
  avance(0.5);
  while (positionY != 0){
    if (portes[(positionY - 3) + positionX] == 2){
      //Avance
      portes[positionY + positionX] = 3;
      positionY -= 6;
      avance(1);
    }
    else if (portes[(positionY + (positionX + 1))] == 2){
      //Gauche
      portes[positionY + positionX] = 3;
      positionX += 1;
      tourner(-1);
      avance(0.5);
      tourner(1);
    }
    else if (portes[(positionY + (positionX - 1))] == 2){
      //Droite
      portes[positionY + positionX] = 3;
      positionX -= 1;
      tourner(1);
      avance(0.5);
      tourner(-1);
    }
    //Pour éviter recule dans le finish
    else if (positionY < 24){
      if (portes[(positionY + 3) + positionX] == 2){
        //Recule
        portes[positionY + positionX] = 3;
        positionY += 6;
        tourner(2);
        avance(1);
        tourner(2);
      };
    };
  };
};

void verif_y(){

  while (true){
  
  if (detection_infrarouge()){
    //change next matrice at same x position to 0
    portes[((positionY + 3)+positionX)] = 0;
    break;
  
  } else if(not detection_infrarouge()) { //avance et enregistre le path valide en ajoutant 1 au porte valide
    
    portes[((positionY + 3 )+ positionX)] = 2; //pos porte valide
    positionY += 6;
    avance(1);
  }

  };
};



void verif_xMoins(){
 // detecte devant lui, si rien -> posX--
 //                     si oui -> S shape / tourne gauche et avance
 if (not detection_infrarouge()){
    portes[(positionY + positionX)] =2;
    positionX -= 1;
    avance(1);
    tourner(1);

  } else {
    portes[((positionY)+positionX+1)] = 0;
    tourner(1);
    printf("probleme, cul de sac");// manque le s shape
  }
};

void verif_xPlus(){
  // detecte devant lui, si rien -> posX ++
 //                     si oui -> S shape / tourne droite et avance
  if (not detection_infrarouge()){
    portes[(positionY + positionX)] =2;
    positionX ++;
    avance(0.5);
    tourner(-1);

  } else {
    portes[((positionY)+positionX+1)] = 0;
    tourner(-1);
    printf("probleme, cul de sac");// manque le s shape
  }

};

void dirChoice(){
  //Choisi de la direction a tourner selon l'etat 0 ou 1 dans cette positions Y, tourne et appel verif x
switch (positionX)
  {
  case 0:
    tourner(1);
    verif_xPlus();
    break;

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

    //situation en s a faire plus tard
    else if(portes[((positionY+3)+(0))] == 0 && portes[((positionY+3)+(1))] == 0 ){
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
  tourner(2);
  //reverse path
  retourne();
};

void setup(){
  BoardInit();
  
  //initialisation
  pinMode(vertpin, INPUT);
  pinMode(rougepin, INPUT);
  delay(100);
};

/*
Fonctions de boucle infini
 -> Se fait appeler perpetuellement suite au "setup"
*/
void loop() {
  
  /**
  detection_sifflet(sifflet);

  if (sifflet && not fini){
    while (!fini){

    }
  }
  */ 
   
  etatPast = etat;
  bumperArr = ROBUS_IsBumper(3);
  if (false){
    if (etat == 0){
      beep(1);
      tourner(-1);
      avance(0.5);
      tourner(1);
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
    else{
      beep(3);
      arret();
      etat = 0;
    }
  }
  else if (ROBUS_IsBumper(1)){
    if (etat == 0){
      beep(1);
      tourner(-1);
      etat = 1;
    }
    else{
      beep(3);
      arret();
      etat = 0;
    }
  }
  else if (ROBUS_IsBumper(2)){
    if (etat == 0){
      beep(1);
      tourner(2);
      etat = 0;
    }
    else{
      beep(3);
      arret();
      etat = 0;
    }
  }

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

