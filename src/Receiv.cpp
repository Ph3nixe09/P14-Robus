#include <LibRobus.h>
#include <Arduino.h>
const int pinReciv = 37;

IRrecv irReceiv(pinReciv);
decode_results results;

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