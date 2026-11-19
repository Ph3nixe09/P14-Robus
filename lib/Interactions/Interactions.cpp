#include <LibRobus.h>
#include <Arduino.h>
#include <DriveCommands.h>
#include <rfid2.h>
#include <Distributor.h>
// Note : There is still some header files to add here

/*
    1. RFID : Returns the index of the patient
    2. Cardiac : Returns the patient's BPM
    3. Thermo : Returns the temperature of the patient
    4. Question : Returns a list of answers
    5. 180° Rotation (Might get included in Pills file)
    6. Pills : (To be determined by the time left) Depending on the answer and data collected, distribute different color tokens / pills
    */

// Variables :
int ID = 0;
int BPM = 0; // Beats per minute
int temperature = 0; // In Celsius ?
bool answer[3] = {false, false, false};
bool interactions_running = false;

enum Interactions_State {
    CALCUL,
    DISTRIBUTION,
    TOURNER,
};

Interactions_State state;

// These function are placebo function, they do nothing except fill the space before the real function get created
int Thermo(){}
int Cardiac(){}
bool Question1(){}
bool Question2(){}
bool Question3(){}

// These function are the ones that will actually be used
void dataExtraction(){
    ID = ReadRFID(); // True fonction
    BPM = Cardiac();
    temperature = Thermo();
    answer[0] = Question1();
    answer[1] = Question2();
    answer[2] = Question3();
}

/*void PillsOrganiser(bool answer[3], int BPM, int temperature){
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
        distribute(2); // Distribute a Blue pill
    }

    // To give red pills
    if (BPM > 100){ // If the heart rate is higher then 100 BPM then a RED pills is given
        distribute(1); // Distribute a Red pill
    }
    // To give green pills
    if (temperature > 37){ // If the temperature is higher the the normal temp of 37 Celsius, then a GREEN pill is given
        // Distribute a Blue and Red pill
        distribute(1);
        distribute(2);
    }
}*/

bool endInteractions(){
    return !interactions_running;
}

void interactionsControl(bool init = false){
    if (init){
        state = CALCUL;
        interactions_running = true;
    }

    if (interactions_running){
        switch (state) {
            case CALCUL:
                dataExtraction();
                tourner(180, 0.5);
                state = TOURNER;
                break;
            
            case TOURNER:
                tourner();
                if (endMoving()){
                    state = DISTRIBUTION;
                }
                break;

            case DISTRIBUTION:
                // Rotates the robot to face the patient (angle, speed)(180, 0.5)
                int trueSizeofAnswer = sizeof(answer) / sizeof(answer[0]);
                int trueAnswerCounter = 0;
                for(int i=0; i<(sizeof(answer)/sizeof(answer[0])); i++){
                    if (answer[i] == true){
                        trueAnswerCounter++;
                    }
                }
                
                if (trueAnswerCounter >= trueSizeofAnswer){
                    distribute(2); // Distribute a Blue pill
                }
                
                // To give red pills
                if (BPM > 100){ // If the heart rate is higher then 100 BPM then a RED pills is given
                    distribute(1); // Distribute a Red pill
                }
                // To give green pills
                if (temperature > 37){ // If the temperature is higher the the normal temp of 37 Celsius, then a GREEN pill is given
                    // Distribute a Blue and Red pill
                    distribute(1);
                    distribute(2);
                }
                
                if (endDistributor()){
                    state = CALCUL;
                    interactions_running = false;
                }
                break;
        }
    }
}