#include <LibRobus.h>
#include <Arduino.h>

IRsend irSend;

bool BP1 = 0;
bool BP2 = 0;

bool Read_BP (int pin){
    bool BP = digitalRead(pin);
    delay(15);
    bool BP_2nd_Read = digitalRead(pin);
    if (BP == BP_2nd_Read){  
        return BP;
    }else{
        return 0;
    }
        
}


void setup(){ 
    Serial.begin(9600);
    delay(50);
    Serial.print("HEllo");
    pinMode(2, OUTPUT); // Commande envoyé LED Verte
    pinMode(4, INPUT); // Bouton 1 Départ
    pinMode(5, INPUT); // Bouton 2 Réveil
}

void loop(){
    BP1 = Read_BP(3);
    if(BP1 == true){
        Serial.print("TRUE BP1 ");
    }
    BP2 = Read_BP(4);
    if(BP1 == true){
        Serial.print("TRUE BP2 ");
    }

    uint16_t address = 0xA90;
    
    irSend.sendNEC(address, 32);
    delay(5000);

}