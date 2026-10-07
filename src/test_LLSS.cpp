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
}

void PID(int32_t pulse_gauche, int32_t pulse_droit){
  int32_t read_pulse_gauche = ENCODER_ReadReset(0);
  int32_t read_pulse_droit =  ENCODER_ReadReset(1);
  int val_attendu_gauche = 10000;
  int val_attendu_droit = 10000;
  int KP = 0.00001;

  int erreur_gauche = val_attendu_gauche - read_pulse_gauche;
  int erreur_droit = val_attendu_droit - read_pulse_droit;

  int correctif_gauche = KP * erreur_gauche;
  int correctif_droit = KP * erreur_droit;
  



}

//Consigne de 0 pour la vitesse pour effectuer un arret
void arret(){
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
};

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

void avance_1PID(float distance){
  float val_expected_distance = (13998.25 * distance); 
  float KP = 0.001;
  float distance_right = 0;  //Pour atteindre la distance voulu
  float speed_right = 0;
  float speed_correct_left = 0;
  float left_error = 0;
  int accel = 0;

  while (true) {
    int32_t read_pulse_left = ENCODER_ReadReset(0);
    int32_t read_pulse_right =  ENCODER_ReadReset(1);
    Serial.print("Pulse right / left: ");
    Serial.println(read_pulse_right);
    Serial.println(read_pulse_left);

    left_error = (read_pulse_right - read_pulse_left) * KP;

      //Distance reel parcouru
    distance_right += read_pulse_right;

    /**
    // Nouveau code d'acceleration à tester
    switch (accel)
    {
      case 0:
        speed_right += 0.1;
        speed_correct_left += 0.1;
        if (speed_right == 0.5){
          accel = 1;
        };
        break;

      case 1:
        if (distance_right <= (val_expected_distance - 3000)){
          speed_right -= 0.1;
          speed_correct_left -= 0.1;
          if (speed_right == 0.1){
            accel = 2;
        };
        break;
      
      case 2:
        break;
    };

    if (distance_right >= val_expected_distance){
      arret()
      break;
    };
    */

    if (distance_right <= (12000)){
      if (speed_right < 0.9){
      speed_right += 0.07;
      speed_correct_left += 0.07;
      }

    }

    else if (distance_right >= (val_expected_distance)){
      arret();
      break;
    }

    else if (distance_right >=(val_expected_distance-12000)){
      if (speed_right > 0.4){
      speed_right -= 0.1;
      speed_correct_left -= 0.1;
      }
    };

    //Ajustement de la vitesse
    speed_correct_left += left_error;
    Serial.print("Error left / speed correct left: ");
    Serial.println(left_error);
    Serial.println(speed_correct_left);
    
    //Ajustement de la vitesse
    MOTOR_SetSpeed(RIGHT,speed_right);
    MOTOR_SetSpeed(LEFT, speed_correct_left);

    delay(200);

  };
};

void recule(){
  MOTOR_SetSpeed(RIGHT, -vitesse);
  MOTOR_SetSpeed(LEFT, -vitesse);
};

void tourner(int angle){
  int expected_val_distance_90 = 2000; // Distance en pulse pour parcourir 90°
  int expected_val_distance_180 = 4000; // Distance en pulse pour parcourir 180°
  int KP = 0.0001;
  int speed = 0;
  int difference_right = 0;
  int difference_left = 0;
  
  switch (angle)
  {
    case 1: // Pivot à droite (90)
      while(true){
        // Lecture des encodeurs
        int32_t read_pulse_right = ENCODER_ReadReset(1);
        int32_t read_pulse_left = ENCODER_ReadReset(0);
      }
      break;
    case -1: // Pivot à gauche (-90)
      break;
    case 2: // Demi-tour (180)
      break;
    default:
        break;
  }
  MOTOR_SetSpeed(RIGHT, speed);
  MOTOR_SetSpeed(LEFT, -speed);
};

bool detection_sifflet(bool &son){
  //Détection du sifflet pour démarrer le programme de labyrinthe
  delay(500);   //Attends une demi seconde
  return (son = true);
};

bool detection_infrarouge(bool obstacle){
  //Détection d'obstacle avec l'infrarouge mais seulement si les deux s'allument
  vert = digitalRead(vertpin);
  rouge = digitalRead(rougepin);
  if (!vert && !rouge){
    obstacle = true;
    return obstacle;
  }
  else {
    obstacle = false;
    return obstacle;
  };
}
/**
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
  if (bumperArr){
    if (etat == 0){
      beep(1);
      avance_1PID(2);
      etat = 1;
    } 
    else{
      beep(1);
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
