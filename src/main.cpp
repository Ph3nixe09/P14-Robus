#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>
#include <LED.h>
#include <rfid2.h>
#include <InfraReceiv.h>
#include <LCD.h>

enum RobotState {
  WAITING_FOR_IR,
  TEST_COMMAND,
  FOLLOWING_LINE,
};

int count;
enum RobotState state;

bool ir_safety = true;
int buffer = 0;
bool bumper_safety = false;  //True sur le final
bool testing = true;

// Fonction actionner quand le robot s'allume
void setup() { 
  Serial.begin(9600);
  BoardInit();
  ledInit();
  drivetrainInit();
  rfidInit();
  startLCD();
  count = 0;

  state = WAITING_FOR_IR;
  beep(3);
  delay(200);  
}

// Boucle du robot
void loop() {
  if(cardPresent()){
    
    Serial.println(ReadRFID());
  }
  //Safety bumper et infrarouge pour la détection d'obstacle
  if (ir_safety){
    if (getIRFront() > 300){
      buffer ++;
      if (buffer >= 3){
        stop();
        state = WAITING_FOR_IR;
        bumper_safety = false;  //A enlever sur le final
      }
    }
    else{
      buffer = 0;
    }
  }
  if (bumper_safety){
    if (getBumberAny()){
      stop();
      state = WAITING_FOR_IR;
      bumper_safety = false;  //Pour pas arreter direct (a enlever sur le final)
      delay(200);  //Pour pas activer le test direct (a enlever sur le final)
    }
  }

  switch (state) {
    case WAITING_FOR_IR:
      if (getBumberRear() && testing) { // POUR TESTER
        state = TEST_COMMAND;
        delay(500);  //Pour pas activer le bumper_safety (a enlever sur le final)
        bumper_safety = true;  //Pour pas activer le bumper_safety (a enlever sur le final)
        driveDistance(100);   //Commande
      }
      if (testing){
        LCD("Scannez votre","bracelet sur le","capteur avant !"," ");
        delay (5000);
        LCD("Entrez votre age","sur le clavier","numerique svp.","");
        delay (5000);
        LCD("Merci",""," "," ");
        delay (5000);
        LCD("Avez-vous mal a","la gorge ou a la","tete ?","Oui: A    Non: B");
        delay (5000);
        LCD("Merci","Mettez votre","doigt sur le ca-", "pteur cardiaque.");
        delay (5000);
        LCD("\x01\x02 \x03_\x03\x04\x02","","       t(O_Ot)","");
        delay(10000);
      }
      break;
    
    case TEST_COMMAND:
      driveDistance();
      if (endMoving()){
        state = WAITING_FOR_IR;
      }
      break;
  }
  delay(20); // clock de la boucle
}