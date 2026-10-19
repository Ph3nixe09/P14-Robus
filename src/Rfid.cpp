#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

const int SS_PIN  = 10;  // SDA on the RC522
const int RST_PIN = 9;
// SCK	D13
// MOSI	D11
// MISO	D12

MFRC522 rfid(SS_PIN, RST_PIN);

void printUID(const MFRC522::Uid &uid) {
  for (byte i = 0; i < uid.size; i++) {
    if (uid.uidByte[i] < 0x10) Serial.print('0');
    Serial.print(uid.uidByte[i], HEX);
    if (i < uid.size - 1) Serial.print(':');
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  delay(50);

  // Sanity check: 0x91 or 0x92 means the reader is wired correctly
  byte v = rfid.PCD_ReadRegister(MFRC522::VersionReg);
  Serial.print(F("RC522 firmware version: 0x"));
  Serial.println(v, HEX);
  if (v == 0x00 || v == 0xFF) {
    Serial.println(F("No reader detected - check wiring and 3.3V power"));
  }
  Serial.println(F("Scan a card..."));
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print(F("UID: "));
  printUID(rfid.uid);

  MFRC522::PICC_Type type = rfid.PICC_GetType(rfid.uid.sak);
  Serial.print(F("Type: "));
  Serial.println(rfid.PICC_GetTypeName(type));

  rfid.PICC_HaltA();        // stop reading this card
  rfid.PCD_StopCrypto1();   // end encrypted session
}