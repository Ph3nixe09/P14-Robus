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


void arret(){
  MOTOR_SetSpeed(RIGHT, 0);
  MOTOR_SetSpeed(LEFT, 0);
};

void avance(float distance){
  int val_expected_distance = 13400 * distance;
  int val_expected_speed = 3333;
  float KPD = 0.0001;
  float KPL = 0.0001;
  float KID = 0.00002;
  float KIL = 0.00002;
  int distance_right = 0;
  int distance_left = 0;
  int cycle = 0;
  float speed_correct_right = 1.0;
  float speed_correct_left = 1.0;

  while (true) {
    int32_t read_pulse_left = ENCODER_ReadReset(0);
    int32_t read_pulse_right =  ENCODER_ReadReset(1);
    int left_error = (val_expected_speed - read_pulse_left)*KPL;
    int right_error = (val_expected_speed - read_pulse_right)*KPD;

    distance_right += read_pulse_right;
    distance_left += read_pulse_left;
    
    if (distance_right >= (val_expected_distance - 1000) && distance_left >= (val_expected_distance - 1000)){
      val_expected_speed = 1500; 
    } 
    else if (distance_right >= (val_expected_distance - 50)  && distance_left >= (val_expected_distance - 50)){
      MOTOR_SetSpeed(RIGHT, 0);
      MOTOR_SetSpeed(LEFT, 0);
      break;
    };

    MOTOR_SetSpeed(RIGHT,speed_correct_right);
    MOTOR_SetSpeed(LEFT, speed_correct_left);
    
    delay(300);

  };
};

void recule(){
  MOTOR_SetSpeed(RIGHT, -vitesse);
  MOTOR_SetSpeed(LEFT, -vitesse);
};

void tourner(int angle){
  int expected_val = 1600;
  int KP = 0.00001;
  int compteur = 0;

  switch (angle)
  {
    case 1: // Pivot à droite (90)
      while(compteur != expected_val){
        int32_t read_pulse_right =  ENCODER_ReadReset(1);
        int32_t read_pulse_left =  ENCODER_ReadReset(0);

        int difference_right = expected_val - read_pulse_right;
        int difference_left = expected_val - read_pulse_left;
      
        int correction = KP * difference;

        
        int speed = 0.5 + correction;
        
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
  avance();
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


  
};
