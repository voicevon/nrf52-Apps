#include <Arduino.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Hello NRF");
}

