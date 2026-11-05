#include <Wire.h>
#include <LibRobus.h>
#include <Arduino.h>
#include <Adafruit_TCS34725.h>
#include <Drivetrain.h>
#include <LED.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X);
int n=0;
int sum_R=0;
int sum_G=0;
int sum_B=0;
/*
bool inTol(int target, int value){
  int tolerance = 18;
  int min = target - (tolerance/2);
  int max = target + (tolerance/2);

  if (min < value && value < max) {
    return true;
  }
  else{
    return false;
  }
}

bool inTightTol(int target, int value){
  int tolerance = 14;
  int min = target - (tolerance/2);
  int max = target + (tolerance/2);

  if (min < value && value < max) {
    return true;
  }
  else{
    return false;
  }
}
*/
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
    if(n<10){          //Loop 5 times to sum the values of R, G, B
      tcs.getRawData(&r, &g, &b, &c);		//Read RGB values to variables.
      sum_R += r;   
         //Serial.println(sum_R);
      sum_G += g;
      sum_B += b;
      n++;
      return NONE;
      }
      else{
    
    int avg_R = sum_R/10;
    int avg_G = sum_G/10;
    int avg_B = sum_B/10;

    Serial.print("R: "); Serial.print(avg_R); Serial.print(" "); //réajustement des valeurs de couleur
    Serial.print("G: "); Serial.print(avg_G); Serial.print(" ");
    Serial.print("B: "); Serial.print(avg_B); Serial.print(" ");
    Serial.println(" ");
    
    sum_R = 0;
    sum_G = 0;
    sum_B = 0;
    n=0;

    /*if (inTol(90, avg_R) && inTol(86, avg_G) && inTol(88, avg_B)) //Red detection
      {
        Serial.println("Red patch");
        return RED;
      }
    else if (inTol(78, avg_R) && inTightTol(81, avg_G) && inTol(71, avg_B))//Orange detection
      {
        Serial.println("Orange patch");
        return ORANGE;
      }
    else if (inTol(50, avg_R) && inTol(82, avg_G) && inTol(92, avg_B))//Blue detection
      { 
        Serial.println("Blue patch");
        return BLUE;
      }
    else if (inTol(65, avg_R) && inTol(95, avg_G) && inTol(93, avg_B))//Green detection
      {
        Serial.println("Green patch");
        return GREEN;
      }
    else if (inTol(81, avg_R) && inTightTol(99, avg_G) && inTol(76, avg_B) && getIRFront() > 200)//Yellow detection // detect also the wall
      {
        Serial.println("Yellow patch");
        return YELLOW;
      }
      else{ 
        Serial.println("None");
        return NONE;
      }*/
if (avg_R == 9 && avg_G == 8 && avg_B == 8) //Red detection
      {
        Serial.println("Red patch");
        return RED;
      }
    else if (avg_R == 8 && avg_G == 8 && avg_B == 7)//Orange detection
      {
        Serial.println("Orange patch");
        return ORANGE;
      }
    else if (avg_R == 5 && avg_G == 8 && avg_B == 9)//Blue detection
      { 
        Serial.println("Blue patch");
        return BLUE;
      }
    else if (avg_R == 7 && avg_G == 9 && avg_B == 9)//Green detection
      {
        Serial.println("Green patch");
        return GREEN;
      }
    else if (avg_R == 8 && avg_G == 10 && avg_B == 7 && getIRFront() > 200)//Yellow detection // detect also the wall
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
  
