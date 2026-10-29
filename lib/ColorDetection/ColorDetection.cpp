#include <Wire.h>
#include <LibRobus.h>
#include <Arduino.h>
#include <Adafruit_TCS34725.h>
#include <Drivetrain.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_4X);

bool inBetween(int min, int max, int value){
  if (min < value && value < max) {
    return true;
  }
  else{
    return false;
  }
}

bool inTol(int target, int value){
  int tolerance = 300;
  int min = target - (tolerance/2);
  int max = target + (tolerance/2);

  if (min < value && value < max) {
    return true;
  }
  else{
    return false;
  }
}

void detectColorInit() {
  //Wire.begin();
  while (true)
  {
    if (tcs.begin()) {
    Serial.println("Found sensor");
    break;

  } else {
    Serial.println("No TCS34725 found ... check your connections");
    delay(20);
  }
  
  } 
  
}

/*void printrgb() {
  while (true) {
    uint16_t r, g, b, c;
    
    tcs.getRawData(&r, &g, &b, &c);

    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    Serial.println(" ");
    
    if (getBumberAny()) {
      break;
    }
    delay(100);
  }
}*/

void colorValues() {
  int sum_R=0;
  int sum_G=0;
  int sum_B=0;

  while (true) {
    uint16_t r, g, b, c;

      for (int i=1 ; i <=5; i++ ) {       //Loop 5 times to sum the values of R, G, B
        tcs.getRawData(&r, &g, &b, &c);		//Read RGB values to variables.
        sum_R += r;
        sum_G += g;
        sum_B += b;
      }

    int avg_R = sum_R/5;
    int avg_G = sum_G/5;
    int avg_B = sum_B/5;

    Serial.print("R: "); Serial.print(avg_R); Serial.print(" ");
    Serial.print("G: "); Serial.print(avg_G); Serial.print(" ");
    Serial.print("B: "); Serial.print(avg_B); Serial.print(" ");
    Serial.println(" ");
    
    /*if (inTol(range, avg_R) && inTol(range, avg_G) && inTol(range, avg_B)) //Red detection
      {
        Serial.println("Red patch");
        break;
      }
    else if (inTol(range, avg_R) && inTol(range, avg_G) && inTol(range, avg_B))//Orange detection
      {
        Serial.println("Orange patch");
        break;
      }
    else if (inTol(range, avg_R) && inTol(range, avg_G) && inTol(range, avg_B))//Blue detection
      { 
        Serial.println("Blue patch");
        break;
      }
    else if (inTol(1771, avg_R) && inTol(2300, avg_G) && inTol(2059, avg_B))//Green detection
      {
        Serial.println("Green patch");
        break;
      }
    else if (inTol(range, avg_R) && inTol(range, avg_G) && inTol(range, avg_B))//Yellow detection
      {
        Serial.println("Yellow patch");
        break;
      }
      else{*/
        sum_R = 0;
        sum_G = 0;
        sum_B = 0;
        avg_R = 0;
        avg_G = 0;
        avg_B = 0;
      //}

    
    }
    
  }


/*void printRGB() {
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
*/