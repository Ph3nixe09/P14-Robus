#include <librobus.h>
#include <LiquidCrystal.h>

// Broches utilisés pour les fonctions du LCD
// Broche 1 = GND / 2 = 5V / 3(VE) = GND / 4(RW) = GND
const int PIN_RS = 22;
const int PIN_E  = 23;
const int PIN_D4 = 24;
const int PIN_D5 = 25;
const int PIN_D6 = 26;
const int PIN_D7 = 27;

LiquidCrystal lcd(PIN_RS, PIN_E, PIN_D4, PIN_D5, PIN_D6, PIN_D7);


uint8_t charCheekL[8] = {
  0b00011,
  0b00110,
  0b00100,
  0b00100,
  0b01000,
  0b01000,
  0b01000,
  0b11000
};

// つ (reaching arm)
uint8_t charArm[8] = {
  0b00000,
  0b00000,
  0b01110,
  0b10001,
  0b00001,
  0b00010,
  0b01100,
  0b00000
};

// ◕ (round eye with upper-left highlight)
uint8_t charEye[8] = {
  0b00000,
  0b00000,
  0b01110,
  0b10111,
  0b11111,
  0b11111,
  0b01110,
  0b00000
};

// ༽ (right cheek)
uint8_t charCheekR[8] = {
  0b11000,
  0b01100,
  0b00100,
  0b00100,
  0b00010,
  0b00010,
  0b00010,
  0b00011
};

void startLCD() {
  delay(100);
  lcd.begin(16, 4); // Use (16, 4) if your screen has 16 columns

  lcd.createChar(1, charCheekL); // ༼
  lcd.createChar(2, charArm);    // つ
  lcd.createChar(3, charEye);    // ◕
  lcd.createChar(4, charCheekR); // ༽

  lcd.clear(); // Returns the LCD from CGRAM to normal display mode
  delay(5);
}

void LCD(const char* ligne1,const char* ligne2,const char* ligne3,const char* ligne4) {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.write(ligne1);
  delay(5);
  lcd.setCursor(0, 1);
  lcd.write(ligne2);
  delay(5);
  lcd.setCursor(0, 2);
  lcd.write(ligne3);
  delay(5);
  lcd.setCursor(0, 3);
  lcd.write(ligne4);
}

void clearLCD(){
  lcd.clear();
}
/*
void loop() {
  LCD("Entrez votre age","dans le clavier","numerique","Oui: 1    Non: 2");

  delay (1000);

  LCD("MERCI !","","","");

  delay(1000);
}
*/