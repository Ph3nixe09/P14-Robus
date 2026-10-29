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

enum Correction {
  STRAIGHT,
  SOFTLEFT,
  SOFTRIGHT,
  HARDLEFT,
  HARDRIGHT
};

enum Correction last_correction = STRAIGHT;

// Commands
// La commande change les vitesses de chaques roues selon les valeurs d'encodeurs pour aller droit
void driveStraight(float speed = 0.5) {
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

//================================================================================================
void tourner(float angle, float speed) {
  if (angle != 0){
    val_expected_turn = -22.05 * angle; //En nombre de degré dans les deux sens (+ droite et - gauche)
    
    turn_right = 0;
    turn_left = 0;
    
    base_speed = 0.3; 
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
//================================================================================================
// Avancer avec PID jusqu'à ce que un des capteurs voit un obstacle
// Retourne le nombre de metres parcourus
int driveToWall() { // NE PAS UTILISÉ
  ResetEncoderAll();

  move(0.3);
  delay(200);
  while (detectWall()) {
    driveStraight();
    delay(200);
  }
  move(0.3);
  delay(200);
  stop();

  float encoder = (getEncoderLeft() + getEncoderRight()) / 2;
  float meter_pulses = (1100 * PULSES_PER_ROTATION) / WHEEL_CIRCUMFERENCE; // 1.10m en pulses
  int count = 0;
  while (meter_pulses < encoder) {
    count++;
    encoder = encoder - meter_pulses;
  }
  return count;
}

// Distance en cm seulement en initialisant
void driveDistance(float distance, float vitesse) {
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
        driveStraight(0.2);
      } else if (encoder < 400) {
        driveStraight(0.4);
      } else if (encoder < distance_pulses - 1500) {
        driveStraight(0.6);
      } else {
        driveStraight(0.3);
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

float distanceAtteint(int type){    //0 est pour tourner et 1 pour avancer
  if (type == 0){
    return (encoder / 22.05);
  }
  else{
    return (encoder * WHEEL_CIRCUMFERENCE / 10 / PULSES_PER_ROTATION);
  }
}

// Suivre une ligne au sol
// La condition de fin sera dans le système de détection de couleurs
void followLine(float speed = 0.5) {
  bool left = getLineLeft();
  bool mid = getLineMid();
  bool right = getLineRight();
  enum Correction current;
  float speed_left = speed;
  float speed_right = speed;
  float k = 0.02; // facteur de correction


  // Choix du facteur de correction de vitesse
  // 8 combinaisons possibles
  if (left && mid && right) { // tous
    current = STRAIGHT;
  } else if (left && mid) {
    current = SOFTRIGHT;
  } else if (right && mid) {
    current = SOFTLEFT;
  } else if (left && right) { // ne devrait pas arriver
    current = STRAIGHT;
  } else if (left) {
    current = SOFTRIGHT;
  } else if (right) {
    current = SOFTLEFT;
  } else if (mid) {
    current = STRAIGHT;
  } else { // aucun, 5 combinaison réduit à 3
    if (last_correction == SOFTRIGHT || last_correction == HARDRIGHT) {
      current = HARDRIGHT;
    } else if (last_correction == SOFTLEFT || last_correction == HARDLEFT) {
      current = HARDLEFT;
    } else {
      current = STRAIGHT;
    }
  }

  // Ajusté les vitesses selon le facteur de correction (k)
  switch (current) {
    case SOFTLEFT:
      speed_right += k;
      break;

    case SOFTRIGHT:
      speed_left += k;
      break;

    case HARDLEFT:
      speed_right += (k * 3);
      break;

    case HARDRIGHT:
      speed_left += (k * 3);
      break;
  }

  last_correction = current;
  moveIndependant(speed_left, speed_right); 
}