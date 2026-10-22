#include <LibRobus.h>
#include <Arduino.h>
#include <IRremote.h>

#define IR_SEND_PIN 2

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
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    IRsend.enableIROut();
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

    uint16_t address = 0x0102;
    uint8_t command = 0x34;
  
    IrSender.sendNEC(address, command, 0);
    delay(5000);

}