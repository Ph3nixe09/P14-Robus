#include <LibRobus.h>
#include <Arduino.h>
#include <DriveCommands.h>
// Note : There is still some header files to add here

// Variables :
int ID = 0;
int BPM = 0; // Beats per minute
int temperature = 0; // In Celsius ?
bool answer[3] = {false, false, false};
enum PillToGive{
    RED,
    BLUE,
    GREEN,
};

enum PillToGive color;

void dataExtractionOrganiser(){
    /*
    1. RFID : Returns the index of the patient
    2. Cardiac : Returns the patient's BPM
    3. Thermo : Returns the temperature of the patient
    4. Question : Returns a list of answers
    5. 180° Rotation (Might get included in Pills file)
    6. Pills : (To be determined by the time left) Depending on the answer and data collected, distribute different color tokens / pills
    */

    /*for(int i=0; i<4; i++){
        answer[i] = Question()[i];
    }*/
    
    ID = RFID();
    BPM = Cardiac();
    temperature = Thermo();
    answer[0] = Question1();
    answer[1] = Question2();
    answer[2] = Question3();

    
}

// These function are placebo function, they do nothing except fill the space before the real function get created
int RFID(){}
int Thermo(){}
int Cardiac(){}
bool Question1(){}
bool Question2(){}
bool Question3(){}
void Pills(enum PillToGive color){};

void PillsOrganiser(bool answer[3], int BPM, int temperature){
    // Rotates the robot to face the patient (angle, speed)(180, 0.5)
    tourner(180, 0.5);

    // To give BLUE pills
    int trueSizeofAnswer = sizeof(answer) / sizeof(answer[0]);
    int trueAnswerCounter = 0;
    for(int i=0; i<(sizeof(answer)/sizeof(answer[0])); i++){
        if (answer[i] == true){
            trueAnswerCounter++;
        }
    }

    if (trueAnswerCounter >= trueSizeofAnswer){
        Pills(BLUE);
    }

    // To give red pills
    if (BPM > 100){ // If the heart rate is higher then 100 BPM then a RED pills is given
        Pills(RED);
    }
    // To give green pills
    if (temperature > 37){ // If the temperature is higher the the normal temp of 37 Celsius, then a GREEN pill is given
        Pills(GREEN);
    }
}