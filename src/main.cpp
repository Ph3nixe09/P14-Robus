#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // ceci est un test
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:test Rose!!!!
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}