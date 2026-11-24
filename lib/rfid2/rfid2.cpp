#include <LibRobus.h>
#include <Arduino.h>
#include <Drivetrain.h>
#include <MFRC522.h>
#include <SPI.h>

const int SS_PIN=53; 
const int RST_PIN=9;
// SCK	D52
// MOSI	D51
// MISO	D50

MFRC522 rfid(SS_PIN, RST_PIN); 

void rfidInit(){
  SPI.begin(); //Initiate SPI Bus
  rfid.PCD_Init(); // Initiate MRFC522 (Lecteur)
  delay(4);
  rfid.PCD_DumpVersionToSerial();
}
bool cardPresent(){
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
    return true;
  }else{
    return false;
  }
}

int ReadRFID(){

    Serial.print("UID tag :");
    String codeRFID= "";   
    String numPatient= "";  
    for (byte i = 0; i < rfid.uid.size; i++){      
      Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");      
      Serial.print(rfid.uid.uidByte[i], HEX);      
      codeRFID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));      
      codeRFID.concat(String(rfid.uid.uidByte[i], HEX));   
    }  
    Serial.println();   
    codeRFID.toUpperCase(); 

    //rfid.PICC_DumpToSerial(&(rfid.uid)); ---- à voir si nécessaire
    
    if(codeRFID.substring(1)=="36 11 82 ED"){//Patient 1 36 11 82 ED
      return 1;   
    }else if(codeRFID.substring(1)=="66 0E 03 EE"){//Patient 2 : 66 0E 03 EE
      return 2; 
    }else if(codeRFID.substring(1)=="E6 99 C9 ED"){//Patient 3 : E6 99 C9 ED
      return 3; 
    }else if(codeRFID.substring(1)=="96 7D 06 EE"){//Patient 4 : 96 7D 06 EE
      return 4; 
    }else if(codeRFID.substring(1)=="86 51 DA ED"){//Patient 5 : 86 51 DA ED
      return 5; 
    }else if(codeRFID.substring(1)=="46 AF E3 ED"){//Master : 46 AF E3 ED
      return 0; 
    }else{
      return 999; // invalide RFID code 
    }
}

void ReadRFID2(){
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent()){
    return;
  }if (!rfid.PICC_ReadCardSerial()){
    return;
  }
    Serial.print("UID tag :");
    String codeRFID= "";
    for (byte i = 0; i < rfid.uid.size; i++){      
      Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");      
      Serial.print(rfid.uid.uidByte[i], HEX);      
      codeRFID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));      
      codeRFID.concat(String(rfid.uid.uidByte[i], HEX));   
    } 
    Serial.println();   
    codeRFID.toUpperCase();
}