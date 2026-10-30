#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <DriveCommands.h>

const int pinRecive = 37;

IRrecv irReceive(pinRecive);
decode_results results;
void InfraReceivInit(){
    irReceive.enableIRIn();
}

bool demarrage (){
    bool recieved =false;
    if(irReceive.decode(&results)){
        
        if (results.value == 0xA35C){
            Serial.print("Départ Reçu");
            beep(2);
            recieved = true;
        }else if (results.value == 0xB76D){
            beep(3);
        }
        irReceive.resume(); 
        return recieved;
    }
    
}
bool wakeUp (){
    bool recieved = false;
    if(irReceive.decode(&results)){
        if (results.value == 0xB76D){
            Serial.print("Wake Up Reçu");
            beep(3);
            recieved = true;
        }else if (results.value == 0xA35C){
            beep(2);
        }
        irReceive.resume(); 
        return recieved;
    }
}
/*
void setup(){ 
    Serial.begin(9600);
    delay(50);
    Serial.print("HEllo");
    irReceiv.enableIRIn();
    // BoardInit();
}

void loop(){
    
    if (irReceiv.decode(&results)){
            
        Serial.println(results.value, HEX);
        if (results.value == 0x01 or results.value == 0x02){
            Serial.print("ON AS REUSSI");
            
        }
        
        irReceiv.resume();
    }

    // uint32_t code = REMOTE_read(); //Recepteur sur pin 37 du arduino X
    // if (code != 0){
    //     Serial.print("Received 0x");
        
    //     Serial.println(code, HEX);
    //     if (code == 0x02 or code == 0x01) {
    //         Serial.print("REÇU!!!!!!!!!!!!");
    //     }
            

    // }
}
    */