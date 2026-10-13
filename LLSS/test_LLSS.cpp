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
  int32_t read_pulse_gauche =  int32_t_ENCODER_ReadReset(0)
  int32_t read_pulse_droit =  int32_t_ENCODER_ReadReset(1)
  val_attendu_gauche = 10000;
  val_attendu_droit = 10000;
  KP = 0.00001;

  erreur_gauche = val_attendu_gauche - read_pulse_gauche;
  erreur_droit = val_attendu_droit - read_pulse_droit;

  correctif_gauche = KP * erreur_gauche;
  correctif_droit = KP * erreur_droit;
  



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
