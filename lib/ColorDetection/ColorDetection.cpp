#include <Wire.h>
#include <LibRobus.h>
#include <Arduino.h>
#include <GroveColorSensor.h>


void detectColorInit() {
  Wire.begin();

}

void colorValues() {
  int red, green, blue;
    GroveColorSensor colorSensor;
    colorSensor.ledStatus = 1;	                // When turn on the color sensor LED, ledStatus = 1; When turn off the color sensor LED, ledStatus = 0.

    int sum_R=0;
    int sum_G=0;
    int sum_B=0;
    
    while (true) {
    
      for (int i=1 ; i <=5; i++ ) {               //Loop 5 times to sum the values of R, G, B
        colorSensor.readRGB(&red, &green, &blue);		//Read RGB values to variables.
        sum_R += red;
        sum_G += green;
        sum_B += blue;
      }
    
      int avg_R = sum_R/5;
      int avg_G = sum_G/5;
      int avg_B = sum_B/5;

      Serial.println(avg_R);
      Serial.println(avg_G);
      Serial.println(avg_B);

      sum_R = 0;
      sum_G = 0;
      sum_B = 0;
      avg_R = 0;
      avg_G = 0;
      avg_B = 0;
    }
}

void printRGB() {
  int red, green, blue;
    GroveColorSensor colorSensor;
    colorSensor.ledStatus = 1;	                // When turn on the color sensor LED, ledStatus = 1; When turn off the color sensor LED, ledStatus = 0.

    int sum_R=0;
    int sum_G=0;
    int sum_B=0;
    
    while (true) {
    
      for (int i=1 ; i <=5; i++ ) {               //Loop 5 times to sum the values of R, G, B
        colorSensor.readRGB(&red, &green, &blue);		//Read RGB values to variables.
        sum_R += red;
        sum_G += green;
        sum_B += blue;
      }
    
      int avg_R = sum_R/5;
      int avg_G = sum_G/5;
      int avg_B = sum_B/5;

      if (3<avg_R<5, 1<avg_G<3, 1<avg_B<3) //Red detection
      {
        Serial.println("Red patch");
        break;
      }
      else if (3<avg_R<5, 1<avg_G<3, 1<avg_B<3)//Orange detection
      {
        Serial.println("Orange patch");
        break;
      }
      else if (3<avg_R<5, 1<avg_G<3, 1<avg_B<3)//Blue detection
      {
        Serial.println("Blue patch");
        break;
      }
      else if (3<avg_R<5, 1<avg_G<3, 1<avg_B<3)//Green detection
      {
        Serial.println("Green patch");
        break;
      }
      else if (3<avg_R<5, 1<avg_G<3, 1<avg_B<3)//Yellow detection
      {
        Serial.println("Yellow patch");
        break;
      }
      else{
        sum_R = 0;
        sum_G = 0;
        sum_B = 0;
        avg_R = 0;
        avg_G = 0;
        avg_B = 0;
      }
    }
    
}
