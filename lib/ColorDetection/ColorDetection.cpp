#include <Wire.h>
#include <LibRobus.h>
#include <Arduino.h>
#include <Adafruit_TCS34725.h>
#include <Drivetrain.h>
#include <LED.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_4X);
int n=0;
int sum_R=0;
int sum_G=0;
int sum_B=0;

bool inTol(int target, int value){
  int tolerance = 10;
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

Color colorValues() {
  uint16_t r, g, b, c;
    if(n<5){          //Loop 5 times to sum the values of R, G, B
      tcs.getRawData(&r, &g, &b, &c);		//Read RGB values to variables.
      sum_R += r;   
         //Serial.println(sum_R);
      sum_G += g;
      sum_B += b;
      n++;
      }
      else{
    
    int avg_R = sum_R/5;
    int avg_G = sum_G/5;
    int avg_B = sum_B/5;

    /*Serial.print("R: "); Serial.print(avg_R); Serial.print(" ");
    Serial.print("G: "); Serial.print(avg_G); Serial.print(" ");
    Serial.print("B: "); Serial.print(avg_B); Serial.print(" ");
    Serial.println(" ");*/
    
    sum_R = 0;
    sum_G = 0;
    sum_B = 0;
    n=0;

    if (inTol(96, avg_R) && inTol(77, avg_G) && inTol(77, avg_B)) //Red detection
      {
        Serial.println("Red patch");
        return RED;
      }
    else if (inTol(80, avg_R) && inTol(72, avg_G) && inTol(57, avg_B))//Orange detection
      {
        Serial.println("Orange patch");
        return ORANGE;
      }
    else if (inTol(41, avg_R) && inTol(73, avg_G) && inTol(83, avg_B))//Blue detection
      { 
        Serial.println("Blue patch");
        return BLUE;
      }
    else if (inTol(61, avg_R) && inTol(90, avg_G) && inTol(85, avg_B))//Green detection
      {
        Serial.println("Green patch");
        return GREEN;
      }
    else if (inTol(89, avg_R) && inTol(102, avg_G) && inTol(68, avg_B))//Yellow detection
      {
        Serial.println("Yellow patch");
        return YELLOW;
      }
      else{ 
        Serial.println("None");
        return NONE;
      }


     
    }
  }
  
