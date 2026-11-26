#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>

int goBackListe[5] = {0,0,0,0,0};

// Constants
const float WHEEL_SPACE = 191.5; // mm
const float WHEEL_SPACE_CIRCUMFERENCE = 3.141592 * WHEEL_SPACE; // mm
const float WHEEL_DIAMETER = 3;
const float WHEEL_CIRCUMFERENCE_INCHES = WHEEL_DIAMETER * 3.141592; // inches
const float WHEEL_CIRCUMFERENCE = WHEEL_CIRCUMFERENCE_INCHES * 25.4; // mm
const float PULSES_PER_ROTATION = 3200;

// Global variables
float distance_pulses;
bool moving = false;
float encoder = 0;
const int turning_error = 10;
float arc;
float arc_pulses;
//Tourner
float val_expected_turn;
float KPT = 0.0005;   // Pour tourner
float KIT = 0.00005;
float turn_right;
float turn_left;
float base_speed;
int inverter;
float speed_right;
float base_speed_left;
float integral_error;

//-----------
// uint32_t previous_pulse_R;
// uint32_t previous_pulse_L;
//--------

// enum Correction {
//   STRAIGHT,
//   SOFTLEFT,
//   SOFTRIGHT,
//   HARDLEFT,
//   HARDRIGHT,
// };

// enum Intersection {
//   NONE,
//   LEFT,
//   RIGHT
// };

// enum Correction last_correction = STRAIGHT;

// Commands
// La commande change les vitesses de chaques roues selon les valeurs d'encodeurs pour aller droit
void driveStraight(float speed = 0.3) {
  float kp = 0.00075 * speed;
  float ki = 0.00015;
  
  int left = getEncoderLeft();
  int right = getEncoderRight();
  
  float p = ((right - last_right) - (left - last_left)) * kp;
  float i = (right - left) * ki;

  float speed_left = speed + p + i;
  float speed_right = speed - p - i;

  moveIndependant(speed_left, speed_right);
  
  last_left = left;
  last_right = right;
}

float distanceAtteint(int type){    //0 est pour tourner et 1 pour avancer
  if (type == 0){
    return (encoder / 22.05);
  }
  else{
    return (encoder * WHEEL_CIRCUMFERENCE / 10 / PULSES_PER_ROTATION);
  }
}

//================================================================================================
void tourner(float angle, float speed) {
  if (angle != 0){
    val_expected_turn = -22.05 * angle; //En nombre de degré dans les deux sens (+ droite et - gauche)
    
    turn_right = 0;
    turn_left = 0;
    
    base_speed = 0.25; //0.3
    inverter = (val_expected_turn >= 0) ? 1 : -1;

    if (speed != 0 ){
      base_speed = speed;
    }

    speed_right = base_speed * inverter;
    base_speed_left = -base_speed * inverter;
    integral_error = 0;
    encoder = 0;
    moving = true;
    ResetEncoderAll();
  }

  if (moving) {
    int32_t read_pulse_left = ENCODER_ReadReset(0);
    int32_t read_pulse_right = ENCODER_ReadReset(1);
  
    // Distances totales parcourues par roue
    turn_left += read_pulse_left;
    turn_right += read_pulse_right;
  
    // Distance moyenne parcourue par les deux roues (angle du robot plus précis)
    encoder  = (abs(turn_right) + abs(turn_left)) / 2.0;
  
    //terminer le mouvement
    if (encoder >= (abs(val_expected_turn) - 50)) {
      startDriveTimer();
      moving = false;
      return;
    }
  
    // Écart de position total entre les roues
    float total_pos_error = (-1.0 * turn_right) - turn_left;
    integral_error += total_pos_error;
  
    // Calcul de la vitesse gauche pour le PID
    float speed_correct_left = base_speed_left + (total_pos_error * KPT) + (integral_error * KIT);
  
    MOTOR_SetSpeed(RIGHT, speed_right);
    MOTOR_SetSpeed(LEFT, speed_correct_left);
  }
}

void turnToLine(bool init = false, float angle = 40, float speed = 0.2) {
  if (init) {
    ResetEncoderAll();
    moving = true;
  }

  if (moving) {
    tourner(angle, speed);
    if (!getLineMid()) {// détection de la ligne
      stop();
      moving = false;
    } else if (distanceAtteint(0) > angle) { // si le robot a tourné de 40 degrés sans voir la ligne
      stop();
      tourner(-(angle * 2), speed); // tourner dans l'autre sens
    }
  }
}

// Avancer avec PID jusqu'à ce que un des capteurs voit un obstacle
void driveToWall(bool init = false) {
  if (init) {
    ResetEncoderAll();
    moving = true;
  }

  if (moving) {
    driveStraight(0.2);
    if (detectWallFront()) {
      startDriveTimer();
      moving = false;
    }
  }
}

void followWall(bool init = false) {
  if (init) {
    ResetEncoderAll();
    moving = true;
  }

  if (moving) {
    driveStraight(0.3);
    if (!detectWallRight()) {
      startDriveTimer();
      moving = false;
    }
  }
}

void driveToLine(bool init = false) {
  if (init) {
    ResetEncoderAll();
    moving = true;
  }

  if (moving) {
    driveStraight(0.3);
    if (!getLineLeft() || !getLineMid() || !getLineRight()) {
      startDriveTimer();
      moving = false;
    }
  }
}

// Distance en cm seulement en initialisant
void driveDistance(float distance = 0, float vitesse = 0) {
  if (distance != 0) {
    ResetEncoderAll();
    distance_pulses = (distance * 10 * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
    moving = true;
    encoder = 0;
  }

  if (moving) {
    encoder = (getEncoderLeft() + getEncoderRight()) / 2;
    if (distance_pulses - 100 < encoder) {
        startDriveTimer();
        moving = false;
    }
    if (vitesse != 0){
      driveStraight(vitesse);
    }
    else {
      if (encoder < 200) {
        driveStraight(0.1);
      } else if (encoder < 400) {
        driveStraight(0.2);
      } else if (encoder < distance_pulses - 1500) {
        driveStraight(0.4);
      } else {
        driveStraight(0.3);
      }
    }
  }
}

void driveDistanceBackward(float distance = 0, float vitesse = 0) {
  if (distance != 0) {
    ResetEncoderAll();
    distance_pulses = (distance * 10 * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE;
    moving = true;
    encoder = 0;
  }

  if (moving) {
    encoder = -((getEncoderLeft() + getEncoderRight()) / 2);
    if (distance_pulses - 100 < encoder) {
        startDriveTimer();
        moving = false;
    }
    if (vitesse != 0){
      driveStraight(-vitesse);
    }
    else {
      if (encoder < 200) {
        driveStraight(-0.2);
      } else if (encoder < 400) {
        driveStraight(-0.4);
      } else if (encoder < distance_pulses - 1500) {
        driveStraight(-0.6);
      } else {
        driveStraight(-0.3);
      }
    }
  }
}
// 
bool endMoving() {
  if (moving) {
    return false;
  } else {
    if (stop()) {
      return true;
    } else {
      return false;
    }
  }
}

//----------------------------
// Suivre une ligne au sol
// La condition de fin sera dans le système de détection de couleurs

float last_time;
float I = 0;
float last_error = 0;
float kp = 0.02;
float ki = 0.0005;
float kd = 0;
bool first_iteration = true;
float past_speed_right;
float past_speed_left;
float erreur;

void followLine(float speed = -0.15) {
  delay(0);
  // if (first_iteration){
  //   past_speed_right = speed;
  //   past_speed_left = speed;
  //   moveIndependant(past_speed_right, past_speed_left);
  //   first_iteration = false;
  // }
  
  int numberOfCaptor = 7;
  int captor_read [numberOfCaptor] = {getLineMid_L(), getLineRight_L(),
                                        getLineLeft(), getLineMid(), getLineRight(),
                                        getLineLeft_R(), getLineMid_R()};
  int ponderation [numberOfCaptor] = {-3,-2,-1,0,1,2,3};

  erreur = 0;
  float now = millis();
  float dt = now - last_time;
  //Serial.println(captor_read[0]);
  //Serial.println(captor_read[1]);
  //Serial.println(captor_read[2]);
  
  //l'erreur est la moyenne ponderer de l'etat de chaque capteur avec la ponderation definit plus tot
   for (int i = 0; i < numberOfCaptor; i++){
     erreur += captor_read[i]*ponderation[i];
   }
   
   Serial.println("erreur");
   Serial.println(erreur);

  //Valeurs des corrections PID
   float P = erreur;
         I += erreur*dt;
   float D = (erreur - last_error)/dt;

   float correction = P*kp + I*ki + D*kd;
  
   float new_speed_right = speed + correction;
   float new_speed_left = speed - correction;
   Serial.println("new speed L");
   Serial.println(new_speed_left);
   Serial.println("new speed R");
   Serial.println(new_speed_right);
  
   moveIndependant(new_speed_right, new_speed_left);

   last_time = now;
  //  past_speed_right = new_speed_right;
  //  past_speed_left = new_speed_left;
}

  // bool left = getLineLeft();
  // bool mid = getLineMid();
  // bool right = getLineRight();

  // bool left_L = getLineLeft_L();
  // bool mid_L = getLineMid_L();
  // bool right_L = getLineRight_L();

  // bool left_R = getLineLeft_R();
  // bool mid_R = getLineMid_R();
  // bool right_R = getLineRight_R();

  // enum Correction current;
  // float speed_left;
  // float speed_right;
  // float k = 0.15; // facteur de correction initiale 0.15

  // enum Intersection now;
  
  // // Presence ou non d une intersection
  // if (left_L) {
  //   now = LEFT;
  // } else if (right_R) {
  //   now = RIGHT;
  // } else {
  //   now = NONE;
  // }

  // // Choix du facteur de correction de vitesse
  // // 8 combinaisons possibles
  // if (now = NONE){
  //   if (left && mid && right) { // tous
  //     current = STRAIGHT;
  //   } else if (left && mid) {
  //     current = SOFTRIGHT;
  //   } else if (right && mid) {
  //     current = SOFTLEFT;
  //   } else if (left && right) { // ne devrait pas arriver
  //     current = STRAIGHT;//straight
  //   } else if (left) {
  //     current = SOFTRIGHT;
  //   } else if (right) {
  //     current = SOFTLEFT;
  //   } else if (mid) {
  //     current = STRAIGHT;
  //   }

  //   else if 
  // }

  // //retourner intersection
  
  // else { // aucun, 5 combinaison réduit à 3
  //   if (last_correction == SOFTRIGHT || last_correction == HARDRIGHT) {
  //     current = HARDRIGHT;
  //   } else if (last_correction == SOFTLEFT || last_correction == HARDLEFT) {
  //     current = HARDLEFT;
  //   } else {
  //     current = STRAIGHT;
  //   }
  // }

  // // Ajuster les vitesses selon le facteur de correction (k)
  // switch (current) {
  //   case SOFTLEFT:
  //     speed_left = speed - k;
  //     speed_right = speed + k;
  //     break;

  //   case SOFTRIGHT:
  //     speed_left = speed + k;
  //     speed_right = speed - k;
  //     break;

  //   case HARDLEFT:
  //     speed_left = 0;
  //     speed_right = speed + 3 * k;
  //     break;

  //   case HARDRIGHT:
  //     speed_left = speed + 3 * k;
  //     speed_right = 0;
  //     break;

  //   case STRAIGHT:
  //     driveStraight(speed);
  //     break;
  // }

  // if (last_correction != STRAIGHT) {
  //   last_correction = current;
  // }
  
  // if (current != STRAIGHT && current != INTERSECTION_L 
  //   && current =! INTERSECTION_R) {
  //   moveIndependant(speed_left, speed_right);
  // }